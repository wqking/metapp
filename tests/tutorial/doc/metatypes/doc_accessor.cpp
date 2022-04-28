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
# Meta type Accessor (tkAccessor)

Template `metapp::Accessor` is used to create accessible (MetaAccessible) with getter/setter or other advanced feature.  
If you don't need getter/setter, and only need variable pointer or member data pointer, you don't need `Accessor`.  

`metapp::Accessor` is an alias of `accessorpp::Accessor`. metapp supports `accessorpp::Accessor` directly.  
For more detailed documentations, please see documents in [accessorpp library](https://github.com/wqking/accessorpp).
accessorpp library is developed by the same developer (wqking) of metapp.  
Note you don't need install accessorpp to use metapp. metapp has bundled accessorpp source code.  

This document will show the basic usage of `Accessor`.  
We can use below functions to create `Accessor`,  

```c++
//form 1
template <typename T, typename G, typename S>
Accessor<T> createAccessor(G && getter, S && setter);

//form 2
template <typename G, typename S>
auto createAccessor(G && getter, S && setter);

//form 3
template <typename T, typename G>
Accessor<T> createReadOnlyAccessor(G && getter);

//form 4
template <typename G>
auto createReadOnlyAccessor(G && getter);
```

Form 1 creates an `Accessor` of `T`. Form 2 is similar to form 1, except `T` is deduced.
Form 3 creates a read-only `Accessor` of `T`. Form 4 is similar to form 3, except `T` is deduced.

Example code
desc*/

namespace {

//code
//desc We have this class
struct DaClass
{
	DaClass() : value() {}

	// getter
	int getValue() const {
		return value;
	}

	// setter
	void setValue(const int n) {
		value = n;
	}

	// member data
	int value;
};
//code

void docAccessor()
{
	//code

	DaClass obj;

	//desc Create an accessible using createAccessor
	metapp::Variant accessible1 = (metapp::createAccessor(&DaClass::getValue, &DaClass::setValue));
	ASSERT(metapp::accessibleGet(accessible1, &obj).get<int>() == 0);
	metapp::accessibleSet(accessible1, &obj, 5);
	ASSERT(metapp::accessibleGet(accessible1, &obj).get<int>() == 5);
	ASSERT(obj.value == 5);

	//code
}

} // namespace

RUN_TUTORIAL(docAccessor)
