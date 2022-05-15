// metapp library
// 
// Copyright (C) 2022 Wang Qi (wqking)
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "testutil.h"

/*desc
# Tutorial for invoking callables

`MetaCallable` is the core meta interface for invoking callables
and get type information from a callable.  
Use the member function `MetaType::getMetaCallable()` to get the meta interface.
If the meta type doesn't implement the interface, `nullptr` is returned.
desc*/

//desc ## Include headers

//code
//desc Header for MetaCallable
#include "metapp/interfaces/metacallable.h"
//desc Header for metapp::Variant
#include "metapp/variant.h"
// To use all declared meta types, include this header
#include "metapp/allmetatypes.h"
//code

namespace {

/*desc
## Invoke non-member function

Let's see how to invoke non-member (free) function.
func1 is the function we are going to invoke.
desc*/

//code
inline std::string func1(const int n)
{
	return std::to_string(n);
}
//code

TEST_CASE("tutorialCallable_freeFunction")
{
	//code

	//desc v is pointer to func1
	metapp::Variant v(&func1);

	//desc Prepare the arguments array
	metapp::Variant arguments[] { 5 };
	//desc Invoke the callable, the nullptr is the object instance, for free function, it's nullptr
	metapp::Variant result = v.getMetaType()->getMetaCallable()->invoke(v, nullptr, arguments, 1);
	ASSERT(result.get<std::string>() == "5");

	//desc Or we can use metapp::callableInvoke to pass the arguments directly
	result = metapp::callableInvoke(v, nullptr, 38);
	ASSERT(result.get<std::string>() == "38");

	//code
}

/*desc
## Invoke class member function

Now let's invoke class member function
desc*/

TEST_CASE("tutorialCallable_memberFunction")
{
	//code
	struct MyClass {
		int value;

		int add(const int delta1, const int delta2) const {
			return value + delta1 + delta2;
		}
	};

	metapp::Variant v(&MyClass::add);
	MyClass obj { 5 };
	// The second argument is the pointer to obj, it's required when invoking member function
	metapp::Variant result = metapp::callableInvoke(v, &obj, 3, 9);
	ASSERT(result.get<int>() == 17);
	//code
}

/*desc
## Invoke std::function

We can also invoke `std::function`
desc*/

TEST_CASE("tutorialCallable_stdFunction")
{
	//code
	metapp::Variant v(std::function<std::string (const std::string &, const std::string &)>(
		[](const std::string & a, const std::string & b) -> std::string {
			return a + b;
		}
	));
	metapp::Variant result = metapp::callableInvoke(v, nullptr, "Hello ", "world");
	ASSERT(result.get<const std::string &>() == "Hello world");
	//code
}

/*desc
## Use overloaded functions
desc*/

TEST_CASE("tutorialCallable_overloaded")
{
	//code
	metapp::Variant callable = metapp::OverloadedFunction();
	metapp::OverloadedFunction & overloadedFunction = callable.get<metapp::OverloadedFunction &>();
	overloadedFunction.addCallable(std::function<int ()>([]() {
		return 1;
		}));
	overloadedFunction.addCallable(std::function<int (int)>([](const int n) {
		return n * 2;
		}));
	overloadedFunction.addCallable(std::function<int (long)>([](const long n) {
		return (int)n * 3;
		}));

	ASSERT(metapp::callableInvoke(callable, nullptr).get<int>() == 1);

	ASSERT(metapp::callableInvoke(callable, nullptr, 5).get<int>() == 10);

	ASSERT(metapp::callableInvoke(callable, nullptr, 5L).get<int>() == 15);
	//code
}

/*desc
## Use default arguments

We also support default arguments.  
`myDefaultArgsFunc` is the function we are going to invoke with default arguments.  
The C++ function doesn't need to have default argument.
desc*/

//code
std::string myDefaultArgsFunc(const int a, const bool b, const std::string & c)
{
	return std::to_string(a) + (b ? "true" : "false") + c;
}
//code

TEST_CASE("tutorialCallable_defaultArgs")
{
	/*desc
	Create a metapp::DefaultArgsFunction<FT>, the FT is deduced by metapp::createDefaultArgsFunction.  
	The first argument is the function, it can be any meta callable, such as free function, member function, constructor, etc.  
	The second argument is a list of default arguments, it must be specified
	in the right to left order in the function prototype
	desc*/
	//code
	metapp::Variant v(metapp::createDefaultArgsFunction(&myDefaultArgsFunc, { "hello", true }));
	//code

	//code
	// Invoke the function, with 2 default arguments
	ASSERT(metapp::callableInvoke(v, nullptr, 5).get<const std::string &>() == "5truehello");

	// Invoke the function, with 1 default arguments
	ASSERT(metapp::callableInvoke(v, nullptr, 38, false).get<const std::string &>() == "38falsehello");

	// Invoke the function, with no default arguments
	ASSERT(metapp::callableInvoke(v, nullptr, 19, false, "GOOD").get<const std::string &>() == "19falseGOOD");

	//code
}

/*desc
## Use variadic function

We can also use variadic function.  
The function must accept two arguments, the first is a pointer to Variant, which contains
the arguments array. The second argument is the number of Variant in the array.
desc*/

//code
int myVariadicFunc(const metapp::Variant * arguments, const int argumentCount)
{
	int total = 0;
	for(int i = 0; i < argumentCount; ++i) {
		total += arguments[i].cast<int>().get<int>();
	}
	return total;
}
//code

TEST_CASE("tutorialCallable_variadic")
{
	//code

	metapp::Variant v(metapp::createVariadicFunction(&myVariadicFunc));

	// Invoke the function with no arguments.
	ASSERT(metapp::callableInvoke(v, nullptr).get<int>() == 0);

	// Invoke the function with 2 arguments.
	ASSERT(metapp::callableInvoke(v, nullptr, 2, 5).get<int>() == 7);

	// Invoke the function with 10 arguments, the arguments can have different type,
	// as long as the target function can process (cast) them.
	ASSERT(metapp::callableInvoke(v, nullptr,
		10, 9.1, 8LL, 7, 6, 5, 4, 3, 2, 1).get<int>() == 55);

	//code
}

} // namespace
