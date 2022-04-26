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

#include "tutorial.h"

/*desc
# Tutorial for invoking callables
desc*/

// code
// The header for MetaCallable
#include "metapp/interfaces/metacallable.h"
// The header for metapp::Variant
#include "metapp/variant.h"
// To use all declared meta types, include this header
#include "metapp/allmetatypes.h"
// code

namespace {

/*desc
Let's see how to invoke free function.
func1 is the function we are going to invoke.
desc*/

//code
inline std::string func1(const int n)
{
	return std::to_string(n);
}

void tutorialCallable_freeFunction()
{
	// v is pointer to func1
	metapp::Variant v(&func1);

	// Prepare the arguments array
	metapp::Variant arguments[] { 5 };
	// Invoke the callable, the nullptr is the object instance, for free function, it's nullptr
	metapp::Variant result = v.getMetaType()->getMetaCallable()->invoke(v, nullptr, arguments, 1);
	ASSERT(result.get<std::string>() == "5");

	// Or we can use metapp::callableInvoke to pass the arguments directly
	result = metapp::callableInvoke(v, nullptr, 38);
	ASSERT(result.get<std::string>() == "38");
}
//code

/*desc
Now let's invoke class member function
desc*/

//code
void tutorialCallable_memberFunction()
{
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
}
//code

/*desc
We can also invoke std::function
desc*/

//code
void tutorialCallable_stdFunction()
{
	metapp::Variant v(std::function<std::string (const std::string &, const std::string &)>(
		[](const std::string & a, const std::string & b) -> std::string {
			return a + b;
		}
	));
	metapp::Variant result = metapp::callableInvoke(v, nullptr, "Hello ", "world");
	ASSERT(result.get<const std::string &>() == "Hello world");
}
//code

/*desc
We also support default arguments
myDefaultArgsFunc is the function we are going to invoke with default arguments
The C++ function doesn't need to have default argument.
desc*/

//code
std::string myDefaultArgsFunc(const int a, const bool b, const std::string & c)
{
	return std::to_string(a) + (b ? "true" : "false") + c;
}

void tutorialCallable_defaultArgs()
{
	// Create a metapp::DefaultArgsFunction<FT>, the FT is deduced by metapp::createDefaultArgsFunction.
	// The first argument is the function, it can be any meta callable, such as free function, member function, constructor, etc.
	// The second argument is a list of default arguments, it must be specified
	// in the right to left order in the function prototype
	metapp::Variant v(metapp::createDefaultArgsFunction(&myDefaultArgsFunc, { "hello", true }));

	// Invoke the function, with 2 default arguments
	ASSERT(metapp::callableInvoke(v, nullptr, 5).get<const std::string &>() == "5truehello");

	// Invoke the function, with 1 default arguments
	ASSERT(metapp::callableInvoke(v, nullptr, 38, false).get<const std::string &>() == "38falsehello");

	// Invoke the function, with no default arguments
	ASSERT(metapp::callableInvoke(v, nullptr, 19, false, "GOOD").get<const std::string &>() == "19falseGOOD");
}
//code

/*desc
We can also use variadic function.
The function must accept two arguments, the first the an array of Variant, which contains
the arguments. The second argument is the number of Variant in the array.
desc*/

//code
int myVariadicFunc(const metapp::Variant * arguments, const size_t argumentCount)
{
	int total = 0;
	for(size_t i = 0; i < argumentCount; ++i) {
		total += arguments[i].cast<int>().get<int>();
	}
	return total;
}

void tutorialCallable_variadic()
{
	metapp::Variant v(metapp::createVariadicFunction(&myVariadicFunc));

	// Invoke the function with no arguments.
	ASSERT(metapp::callableInvoke(v, nullptr).get<int>() == 0);

	// Invoke the function with 2 arguments.
	ASSERT(metapp::callableInvoke(v, nullptr, 2, 5).get<int>() == 7);

	// Invoke the function with 10 arguments, the arguments can have different type,
	// as long as the target function can process (cast) them.
	ASSERT(metapp::callableInvoke(v, nullptr, 10, 9.1, 8LL, 7, 6, 5, 4, 3, 2, 1).get<int>() == 55);
}
//code

RUN_TUTORIAL(tutorialCallable_freeFunction)
RUN_TUTORIAL(tutorialCallable_memberFunction)
RUN_TUTORIAL(tutorialCallable_stdFunction)
RUN_TUTORIAL(tutorialCallable_defaultArgs)
RUN_TUTORIAL(tutorialCallable_variadic)

} // namespace
