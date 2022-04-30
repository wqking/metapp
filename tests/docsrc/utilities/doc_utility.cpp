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

#define FN_PREFIX docUtility_

#include "metapp/allmetatypes.h"

/*desc
# Utility reference

## Overview

`utility.h` provides some utility functions.

## Header
desc*/

//code
#include "metapp/utilities/utility.h"
//code

/*desc
## Functions

#### getNonReferenceMetaType

```c++
inline const MetaType * getNonReferenceMetaType(const MetaType * metaType)
{
	if(metaType->isReference()) {
		metaType = metaType->getUpType();
	}
	return metaType;
}

inline const MetaType * getNonReferenceMetaType(const Variant & value)
{
	return getNonReferenceMetaType(value.getMetaType());
}
```

If the `metaType`, or meta type in the Variant `value`, is a reference, returns the meta type the reference refers to.  
If the meta type is not a reference, returns the meta type.

#### getReferredMetaType

```c++
inline const MetaType * getReferredMetaType(const MetaType * metaType)
{
	if(metaType->isPointer() || metaType->isReference()) {
		metaType = metaType->getUpType();
	}
	return metaType;
}

inline const MetaType * getReferredMetaType(const Variant & value)
{
	return getReferredMetaType(value.getMetaType());
}
```

If the `metaType`, or meta type in the Variant `value`, is a reference or pointer, returns the meta type the reference refers to, or the pointer points to.  
If the meta type is not a reference nor pointer, returns the meta type.

#### selectOverload

```c++
template <typename Signature, typename Class>
auto selectOverload(Signature (Class::*func)) -> decltype(func);

template <typename Signature>
auto selectOverload(Signature * func) -> decltype(func);
```

Helper function to select the overloaded function.

**Example**  
desc*/

UFN(FN_PREFIX)
{
	//code
	struct MyClass {
		void func() {}
		int func(int a) { return a; }
	};
	// This will cause compile error
	//metapp::Variant v1(&MyClass::func);
	metapp::Variant v1(metapp::selectOverload<int(int)>(&MyClass::func));

	//desc We also don't need selectOverload, just use static_cast
	metapp::Variant v2(static_cast<int (MyClass::*)(int)>(&MyClass::func));
	//code
}

/*desc
#### dumpMetaType

```c++
void dumpMetaType(std::ostream & stream, const metapp::MetaType * metaType, const MetaRepo * metaRepository = nullptr);
```

Dump readable information of `metaType` to `stream`.  
The meta data is obtained from `MetaRepo` `metaRepository`. If `metaRepository` is nullptr, the global `MetaRepo` is used.  
This function is for test and learning purpose, you should not use it in any production code.  

#### getNameByTypeKind

```c++
std::string getNameByTypeKind(const TypeKind typeKind);
```

Get the built-in name of `typeKind`. Note: the name doesn't depend on `MetaRepo`.

desc*/
