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

#define FN_PREFIX docDeclareMetaType_

#include "metapp/allmetatypes.h"

/*desc
# Use DeclareMetaType to reflect meta types at compile time

## Overview

Even though metapp works on any C++ type that are not known to metapp, it's useful to provide metapp more information on a certain type. The template `DeclareMetaType` is used for providing such information.  
To provide more information of a type to metapp, specialize template `metapp::DeclareMetaType` with the type. The specialization must be in the global namespace. The specialization should inherit from `metapp::DeclareMetaTypeBase`.   
Only the raw type should be specialized. That's to say, the type should not contain any const-volatile qualifiers, reference, pointer, array, or any containers. metapp will handle all the other cases for you. For example, if the raw type is T, metapp will handle `const T`, `T &`, `const T *`, `std::vector<T>`, etc.  

Function `metapp::getMetaType` gets information declared by `metapp::DeclareMetaType` as non-template class `metapp::MetaType`.  

For example,  
desc*/

//code
// Assume the type is MyClass
class MyClass {};
//code

#if 0
//code
template <>
struct metapp::DeclareMetaType <MyClass>
	: metapp::DeclareMetaTypeBase <MyClass>
{
};
//code
#endif

/*desc
The members in DeclareMetaType that will be used by metapp are static constants, type definition, or static functions. All members are optional. If any member is missed, metapp will use default implementation.  

## Members in DeclareMetaType

#### Constant typeKind
desc*/

#if 0
//code
template <>
struct metapp::DeclareMetaType <MyClass>
	: metapp::DeclareMetaTypeBase <MyClass>
{
	static constexpr metapp::TypeKind typeKind = metapp::tkUser + 1;
};
//code
#endif

/*desc
The default value of `typeKind` is `metapp::tkObject`.  

Each `MetaType` always has one `TypeKind`. TypeKind is a 16 bit integer that represents the meta type. For any user defined TypeKind, the value must be equal or greater than `metapp::tkUser`.  

#### Function cast

```c++
static bool cast(Variant * result, const Variant & value, const MetaType * toMetaType);
```

Function `cast` return true if the `value` can be type casted to `toMetaType`, otherwise returns false.  
If `result` is not nullptr and if the `value` can be casted, set `result` with the casted value.  
If the implementation can't cast the value, it should return the value of default implementation, `CommonDeclareMetaType<T>::cast(result, value, toMetaType)`.  
desc*/

#if 0
//code
template <>
struct metapp::DeclareMetaType <MyClass>
	: metapp::DeclareMetaTypeBase <MyClass>
{
	static bool cast(Variant * result, const Variant & value, const MetaType * toMetaType)
	{
		if(can cast MyClass to toMetaType) {
			*result = casted value;
			return true;
		}
		return metapp::CommonDeclareMetaType<MyClass>::cast(result, value, toMetaType);
	}
};
//code
#endif

/*desc
#### Function setup
```c++
static void setup();
```

Function `setup` is invoked on the first time when the MetaType is initialized. It will be called only once for one MetaType.  
`setup` is useful when the `DeclareMetaType` needs to do initialize work. One use case is to register inheritance relationship.  

**Example** 
desc*/

#if 0
//code
struct A {};
struct B {};
struct C : A, B {};

template <>
struct metapp::DeclareMetaType <C>
	: metapp::DeclareMetaTypeBase <C>
{
	static void setup() {
		metapp::getMetaRepo()->registerBase<C, A, B>();
	}
};
//code
#endif

/*desc
## Members in DeclareMetaType, usually don't need to be re-implemented

#### Type UpType
desc*/

#if 0
//code
template <>
struct metapp::DeclareMetaType <MyClass>
	: metapp::DeclareMetaTypeBase <MyClass>
{
	using UpType = TheUpTypeOfMyClass;
};
//code
#endif

/*desc
desc*/
