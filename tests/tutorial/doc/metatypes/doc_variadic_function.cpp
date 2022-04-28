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

#include "metapp/allmetatypes.h"

/*desc
# Meta type variadic function (tkVariadicFunction)

Template `metapp::VariadicFunction` is used to create a variadic function.  

```c++
template <typename FT>
struct VariadicFunction
{
	explicit VariadicFunction(const Variant & func);
};
```
`FT` is the function prototype.  
The parameter `func` in the constructor is a callable Variant that implements `MetaCallable` interface.
It can be constructor, function pointer, member function pointer, etc.  
The callable must have the prototype as,  

```c++
ResultType callable(const metapp::Variant * arguments, const size_t argumentCount)
```

The function can have any result type.  
Parameter `arguments` is a pointer to Variant. It contains the arguments array.    
Parameter `argumentCount` is the number of Variant in the array.  

The arguments are passed from lef to right order.

`VariadicFunction` is an adaptor to dedicate any invoking to `func`, and convert the arguments to the `arguments` array.

To deduce `FT`, we can use the non-member function to create `createVariadicFunction`,  

```c++
template <typename FT>
VariadicFunction<FT> createVariadicFunction(FT && func);
```

Now let's see how to use `VariadicFunction`.
desc*/

namespace {

//code
//desc Assume we have this function
int sum(const metapp::Variant * arguments, const size_t argumentCount)
{
	int total = 0;
	for(size_t i = 0; i < argumentCount; ++i) {
		total += arguments[i].cast<int>().get<int>();
	}
	return total;
}
//code

TEST_CASE("docVariadicFunction")
{
	//code
	//desc Create the VariadicFunction Variant
	metapp::Variant func = (metapp::createVariadicFunction(&sum));

	//desc Invoke func without arguments
	ASSERT(metapp::callableInvoke(func, nullptr).get<int>() == 0);

	//desc Invoke func with 2 arguments
	ASSERT(metapp::callableInvoke(func, nullptr, 6, 3).get<int>() == 9);

	//desc Invoke the function with 10 arguments, the arguments can have different type,
	//desc as long as the target function can process (cast) them.
	ASSERT(metapp::callableInvoke(func, nullptr,
		10, 9.1, 8LL, 7, 6, 5, 4, 3, 2, 1).get<int>() == 55);
	//code
}

} // namespace

