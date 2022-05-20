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

#include "metapp/allmetatypes.h"

/*desc
# Meta type overloaded function (tkOverloadedFunction)

`metapp` supports overloaded function.  
When registering callable using function `registerCallable` in MetaRepo or MetaClass, if more than one callables
have same name, all callables of the same name are combine to an overload function automatically.  
Class `metapp::OverloadedFunction` is used to create overloaded function, though usually you don't need to use the
class directly, since MetaRepo and MetaClass will do it for you.  

```c++
class OverloadedFunction
{
public:
	OverloadedFunction();
	const std::deque<Variant> & getCallableList() const;
	void addCallable(const Variant & callable);
};
```
The meta type for `OverloadedFunction` implements meta interface `MetaCallable`.  
`OverloadedFunction` behaves as if it's a single function. When calling `MetaCallable::invoke` on an `OverloadedFunction`,
it tries to find the best matched function and invoke it. If no function matched, exception `metapp::IllegalArgumentException`
is raised.  

Note: when registering overload functions in MetaRepo or MetaClass, the functions are not necessary C++ overloaded function.
The functions can be in different name, or in different type, such as one function is a free function, the other is `std::function`.   
Any functions under the same name in MetaRepo or MetaClass are treated as overloaded function.

Now let's see how to use overload function in MetaRepo.
desc*/

namespace {

//code
//desc A free function.
std::string f1(int a)
{
	return std::to_string(a);
}
//code

ExampleFunc
{
	//code
	metapp::MetaRepo metaRepo;
	//desc Register the free function under name "func".
	metaRepo.registerCallable("func", &f1);
	//desc Register std::function under the same name "func".
	metaRepo.registerCallable("func", std::function<std::string (int, const std::string &)>(
		[](int a, const std::string & b) {
			return std::to_string(a) + b;
		}
	));
	//desc Get the overload function.
	const metapp::MetaItem & func = metaRepo.getCallable("func");
	//desc Now let's call the overload function.
	// Invoke f1
	ASSERT(metapp::callableInvoke(func, nullptr, 5).get<const std::string &>() == "5");
	// Invoke std::function
	ASSERT(metapp::callableInvoke(func, nullptr, 6, "good").get<const std::string &>() == "6good");
	//code
}

} // namespace {
