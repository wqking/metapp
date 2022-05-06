[//]: # (Auto generated file, don't modify this file.)

# Utility reference
<!--begintoc-->
* [Overview](#a2_1)
* [Header](#a2_2)
* [Functions](#a2_3)
  * [getNonReferenceMetaType](#a4_1)
  * [getReferredMetaType](#a4_2)
  * [typeKindIsIntegral](#a4_3)
  * [typeKindIsArithmetic](#a4_4)
  * [typeKindIsArithmetic](#a4_5)
  * [typeIsCharPtr](#a4_6)
  * [typeIsWideCharPtr](#a4_7)
  * [typeIsString](#a4_8)
  * [typeIsWideString](#a4_9)
  * [selectOverload](#a4_10)
  * [dumpMetaType](#a4_11)
  * [getNameByTypeKind](#a4_12)
<!--endtoc-->

<a id="a2_1"></a>
## Overview

`utility.h` provides some utility functions.

<a id="a2_2"></a>
## Header

```c++
#include "metapp/utilities/utility.h"
```

<a id="a2_3"></a>
## Functions

<a id="a4_1"></a>
#### getNonReferenceMetaType

```c++
const MetaType * getNonReferenceMetaType(const MetaType * metaType);
const MetaType * getNonReferenceMetaType(const Variant & value);
```

If the `metaType`, or meta type in the Variant `value`, is a reference, returns the meta type the reference refers to.  
If the meta type is not a reference, returns the meta type.

<a id="a4_2"></a>
#### getReferredMetaType

```c++
const MetaType * getReferredMetaType(const MetaType * metaType);
const MetaType * getReferredMetaType(const Variant & value);
```

If the `metaType`, or meta type in the Variant `value`, is a reference or pointer, returns the meta type the reference refers to, or the pointer points to.  
If the meta type is not a reference nor pointer, returns the meta type.

<a id="a4_3"></a>
#### typeKindIsIntegral

```c++
constexpr bool typeKindIsIntegral(const TypeKind typeKind);
```

Returns true if the type kind is integral type.  
Integral type is `bool`, `char`, `wchar_t`, `char8_t`, `char16_t`, `char32_t`, `signed char`, `unsigned char`,
`short`, `unsigned short`, `int`, `unsigned int`, `long`, `unsigned long`, `long long`, `unsigned long long`.

<a id="a4_4"></a>
#### typeKindIsArithmetic

```c++
constexpr bool typeKindIsReal(const TypeKind typeKind);
```

Returns true if the type kind is real (float point) type.  
Real type is `float`, `double`, `long double`.

<a id="a4_5"></a>
#### typeKindIsArithmetic

```c++
constexpr bool typeKindIsArithmetic(const TypeKind typeKind);
```

Returns true if the type kind is either integral or real type.  

<a id="a4_6"></a>
#### typeIsCharPtr

```c++
bool typeIsCharPtr(const MetaType * type);
```

Returns true if the type is a pointer to char, such as `char *`, `const char *`, etc.  

<a id="a4_7"></a>
#### typeIsWideCharPtr

```c++
bool typeIsWideCharPtr(const MetaType * type);
```

Returns true if the type is a pointer to wchar_t, such as `wchar_t *`, `const wchar_t *`, etc.  

<a id="a4_8"></a>
#### typeIsString

```c++
bool typeIsString(const MetaType * type);
```

Returns true if the type is either `std::string` or a pointer to char.  

<a id="a4_9"></a>
#### typeIsWideString

```c++
bool typeIsWideString(const MetaType * type);
```

Returns true if the type is either `std::wstring` or a pointer to wchar_t.  

<a id="a4_10"></a>
#### selectOverload

```c++
template <typename Signature, typename Class>
auto selectOverload(Signature (Class::*func)) -> decltype(func);

template <typename Signature>
auto selectOverload(Signature * func) -> decltype(func);
```

Helper function to select the overloaded function.

**Example**  

```c++
struct MyClass {
  void func() {}
  int func(int a) { return a; }
};
// This will cause compile error
//metapp::Variant v1(&MyClass::func);
metapp::Variant v1(metapp::selectOverload<int(int)>(&MyClass::func));
```

We also don't need selectOverload, just use static_cast

```c++
metapp::Variant v2(static_cast<int (MyClass::*)(int)>(&MyClass::func));
```

<a id="a4_11"></a>
#### dumpMetaType

```c++
void dumpMetaType(std::ostream & stream, const metapp::MetaType * metaType, const MetaRepo * metaRepo = nullptr);
```

Dump readable information of `metaType` to `stream`.  
The meta data is obtained from `MetaRepo` `metaRepo`. If `metaRepo` is nullptr, the global `MetaRepo` is used.  
This function is for test and learning purpose, you should not use it in any production code.  

<a id="a4_12"></a>
#### getNameByTypeKind

```c++
std::string getNameByTypeKind(const TypeKind typeKind);
```

Get the built-in name of `typeKind`. Note: the name doesn't depend on `MetaRepo`.
