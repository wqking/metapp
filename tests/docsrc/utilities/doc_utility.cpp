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
const MetaType * getNonReferenceMetaType(const MetaType * metaType);
const MetaType * getNonReferenceMetaType(const Variant & value);
```

If the `metaType`, or meta type in the Variant `value`, is a reference, returns the meta type the reference refers to.  
If the meta type is not a reference, returns the meta type.

#### getReferredMetaType

```c++
const MetaType * getReferredMetaType(const MetaType * metaType);
const MetaType * getReferredMetaType(const Variant & value);
```

If the `metaType`, or meta type in the Variant `value`, is a reference or pointer, returns the meta type the reference refers to, or the pointer points to.  
If the meta type is not a reference nor pointer, returns the meta type.

#### typeKindIsIntegral

```c++
constexpr bool typeKindIsIntegral(const TypeKind typeKind);
```

Returns true if the type kind is integral type.  
Integral type is `bool`, `char`, `wchar_t`, `char8_t`, `char16_t`, `char32_t`, `signed char`, `unsigned char`,
`short`, `unsigned short`, `int`, `unsigned int`, `long`, `unsigned long`, `long long`, `unsigned long long`.

#### typeKindIsArithmetic

```c++
constexpr bool typeKindIsReal(const TypeKind typeKind);
```

Returns true if the type kind is real (float point) type.  
Real type is `float`, `double`, `long double`.

#### typeKindIsArithmetic

```c++
constexpr bool typeKindIsArithmetic(const TypeKind typeKind);
```

Returns true if the type kind is either integral or real type.  

#### typeIsCharPtr

```c++
bool typeIsCharPtr(const MetaType * type);
```

Returns true if the type is a pointer to char, such as `char *`, `const char *`, etc.  

#### typeIsWideCharPtr

```c++
bool typeIsWideCharPtr(const MetaType * type);
```

Returns true if the type is a pointer to wchar_t, such as `wchar_t *`, `const wchar_t *`, etc.  

#### typeIsString

```c++
bool typeIsString(const MetaType * type);
```

Returns true if the type is either `std::string` or a pointer to char.  

#### typeIsWideString

```c++
bool typeIsWideString(const MetaType * type);
```

Returns true if the type is either `std::wstring` or a pointer to wchar_t.  

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

ExampleFunc
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
void dumpMetaType(std::ostream & stream, const metapp::MetaType * metaType, const MetaRepo * metaRepo = nullptr);
```

Dump readable information of `metaType` to `stream`.  
The meta data is obtained from `MetaRepo` `metaRepo`. If `metaRepo` is nullptr, the global `MetaRepo` is used.  
This function is for test and learning purpose, you should not use it in any production code.  

#### getNameByTypeKind

```c++
std::string getNameByTypeKind(const TypeKind typeKind);
```

Get the built-in name of `typeKind`. Note: the name doesn't depend on `MetaRepo`.

desc*/
