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
# Meta type default arguments function (tkDefaultArgsFunction)

Template `metapp::DefaultArgsFunction` is used to create a defautl arguments function.  

```c++
template <typename FT>
struct DefaultArgsFunction
{
	DefaultArgsFunction(const Variant & func, std::vector<Variant> defaultArgs);
};
```
`FT` is the function prototype.  
The parameter `func` in the constructor is a callable Variant that implements `MetaCallable` interface.
It can be constructor, function pointer, member function pointer, etc. The C++ type of the callable doesn't
need to have default arguments. `DefaultArgsFunction` has no relationship to C++ default arguments function.  
The parameter `defaultArgs` is a vector of default arguments.  

`DefaultArgsFunction` is an adaptor to dedicate any invoking to `func`, and provide `func` any missing arguments from `defaultArgs`.

To deduce `FT`, we can use the non-member function to create `DefaultArgsFunction`,  

```c++
template <typename FT>
DefaultArgsFunction<FT> createDefaultArgsFunction(FT && func, std::vector<Variant> defaultArgs);
```

Now let's see how to use `DefaultArgsFunction`.
desc*/

namespace {

//code
//desc Assume we have this function
std::string docDefaultArgsFunc(const std::string & a, const int b)
{
	return a + std::to_string(b);
}
//code

TEST_CASE("docDefaultArgsFunction")
{
	//code
	//desc Then we can have,
	metapp::Variant func(metapp::createDefaultArgsFunction(&docDefaultArgsFunc, { 5 }));

	//desc Call the function with 1 argument, 1 default argument
	metapp::Variant result1 = metapp::callableInvoke(func, nullptr, "hello");
	ASSERT(result1.get<const std::string &>() == "hello5");

	//desc Call the function with 2 arguments, no default argument
	metapp::Variant result2 = metapp::callableInvoke(func, nullptr, "Good", 38);
	ASSERT(result2.get<const std::string &>() == "Good38");
	//code
}

} // namespace


