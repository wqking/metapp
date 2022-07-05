[//]: # (Auto generated file, don't modify this file.)

# Utility reference
<!--begintoc-->
- [Overview](#mdtoc_e7c3d1bb)
- [Header](#mdtoc_6e72a8c1)
- [Functions](#mdtoc_43ac2d0c)
  - [getNonReferenceMetaType](#mdtoc_4bdd99f)
  - [typeKindIsIntegral](#mdtoc_9d1d5f7b)
  - [typeKindIsSignedIntegral](#mdtoc_3bdfaa1c)
  - [typeKindIsUnsignedIntegral](#mdtoc_63dcacc5)
  - [typeKindIsReal](#mdtoc_5fce48b8)
  - [typeKindIsArithmetic](#mdtoc_e7c716be)
  - [isSameMetaType](#mdtoc_f760704)
  - [depointer](#mdtoc_d0e5dd0f)
  - [dereference](#mdtoc_f4eec6df)
  - [getPointer](#mdtoc_41379b28)
  - [getPointedType](#mdtoc_666234f9)
  - [getPointerAndType](#mdtoc_f8216508)
  - [selectOverload](#mdtoc_9637f10c)
  - [dumpMetaType](#mdtoc_556c8b)
  - [getNameByTypeKind](#mdtoc_ce758620)
<!--endtoc-->

<a id="mdtoc_e7c3d1bb"></a>
## Overview

`utility.h` provides some utility functions.

<a id="mdtoc_6e72a8c1"></a>
## Header

```c++
#include "metapp/utilities/utility.h"
```

<a id="mdtoc_43ac2d0c"></a>
## Functions

<a id="mdtoc_4bdd99f"></a>
#### getNonReferenceMetaType

```c++
const MetaType * getNonReferenceMetaType(const MetaType * metaType);
const MetaType * getNonReferenceMetaType(const Variant & var);
```

If the `metaType`, or meta type in the Variant `var`, is a reference, returns the meta type the reference refers to.  
If the meta type is not a reference, returns the meta type.

<a id="mdtoc_9d1d5f7b"></a>
#### typeKindIsIntegral

```c++
constexpr bool typeKindIsIntegral(const TypeKind typeKind);
```

Returns true if the type kind is integral type.  
Integral type is `bool`, `char`, `wchar_t`, `char8_t`, `char16_t`, `char32_t`, `signed char`, `unsigned char`,
`short`, `unsigned short`, `int`, `unsigned int`, `long`, `unsigned long`, `long long`, `unsigned long long`.

<a id="mdtoc_3bdfaa1c"></a>
#### typeKindIsSignedIntegral

```c++
constexpr bool typeKindIsSignedIntegral(const TypeKind typeKind);
```

Returns true if the type kind is signed integral type.  

<a id="mdtoc_63dcacc5"></a>
#### typeKindIsUnsignedIntegral

```c++
constexpr bool typeKindIsUnsignedIntegral(const TypeKind typeKind);
```

Returns true if the type kind is unsigned integral type.  

<a id="mdtoc_5fce48b8"></a>
#### typeKindIsReal

```c++
constexpr bool typeKindIsReal(const TypeKind typeKind);
```

Returns true if the type kind is real (float point) type.  
Real type is `float`, `double`, `long double`.

<a id="mdtoc_e7c716be"></a>
#### typeKindIsArithmetic

```c++
constexpr bool typeKindIsArithmetic(const TypeKind typeKind);
```

Returns true if the type kind is either integral or real type.  

<a id="mdtoc_f760704"></a>
#### isSameMetaType

```c++
template <typename ...Ts>
constexpr bool isSameMetaType(const MetaType * metaType);
```

Returns true if the `metaType` equals to any type in `Ts...`.  
The function is similar to the pseudo code  

```c++
return metaType->equal(metapp::getMetaType<Ts[0]>())
  || metaType->equal(metapp::getMetaType<Ts[1]>())
  || ...
  || metaType->equal(metapp::getMetaType<Ts[N]>());
```

This function is useful when check if a meta type is any of certain types,
for example, to check if a meta type is a C string (`const char *`) or `std::string`,
we can check `isSameMetaType<char *, std::string>(metaType)`.

<a id="mdtoc_d0e5dd0f"></a>
#### depointer
```c++
Variant depointer(const Variant & var);
```
Convert a pointer in `var` to its non-pointer equivalence.  
Return a Variant that,  
If `var` is a value or reference, `var` is returned.  
If `var` is a pointer, the function returns a reference that refers to the value that the pointer points to.  
If `var` is pointer wrapper such as `std::shared_ptr<T>` or `std::unique_ptr<T>`,
it returns a reference that refers to the value that the pointer points to.  

`depointer` only makes reference, it doesn't copy any underlying value.  

`depointer` is useful to write generic code. Assume a function accepts an argument of Variant.
Without `depointer`, the function either requires the Variant to be a value/reference, or a pointer,
but not both, or the function uses extra code to detect whether the argument is a value/reference or pointer.
With `depointer`, the argument can be value, reference, or pointer, then the function calls `depointer`
to normalize the argument, and use a single logic for all three kinds of Variants (value, reference pointer).

**Example**  

```c++
metapp::Variant v1(5); // value
ASSERT(v1.get<int>() == 5);
// r1 is a value too
metapp::Variant r1(depointer(v1));
ASSERT(r1.get<int &>() == 5);
r1.get<int &>() = 38;
// Assignment doesn't affect original value
ASSERT(v1.get<int>() == 5);
ASSERT(r1.get<int &>() == 38);

int n = 9;
// pointer, points to n;
metapp::Variant v2(&n);
ASSERT(n == 9);
ASSERT(*v2.get<int *>() == 9);
// r2 refers to n
metapp::Variant r2(depointer(v2)); 
ASSERT(r2.get<int &>() == 9);
r2.get<int &>() = 10;
ASSERT(n == 10);
ASSERT(*v2.get<int *>() == 10);

int m = 10;
// reference, refers to m;
metapp::Variant v3(metapp::Variant::reference(m));
ASSERT(m == 10);
ASSERT(v3.get<int &>() == 10);
// r3 refers to m
metapp::Variant r3(depointer(v3)); 
ASSERT(r3.get<int &>() == 10);
r3.get<int &>() = 15;
ASSERT(m == 15);
ASSERT(v3.get<int &>() == 15);
ASSERT(r3.get<int &>() == 15);
```

<a id="mdtoc_f4eec6df"></a>
#### dereference
```c++
Variant dereference(const Variant & var);
```
Return the value that the underlying pointer or reference points to.
This is the same semantic as the dereference operator * in C++.  
If `var` is a value, returns `var`.  
If `var` is a reference, returns the value that `var` refers to.  
If `var` is a pointer, returns the value that `var` points to.  
If `var` is pointer wrapper such as `std::shared_ptr<T>` or `std::unique_ptr<T>`,
it returns the value that `var` points to.  
Note: if `var` is a reference, pointer, or pointer wrapper, `dereference` will copy the underlying value to the result Variant,
that may be expensive.  

<a id="mdtoc_41379b28"></a>
#### getPointer

```c++
void * getPointer(const Variant & var);
```

Returns a pointer to the underlying value in `var`.  
If `var` is value or reference to value, returns the value address.  
If `var` is a pointer or reference to pointer, returns the pointer.  
If `var` is pointer wrapper such as `std::shared_ptr<T>` or `std::unique_ptr<T>`,
returns the stored pointer (`std::shared_ptr<T>::get()`, or `std::unique_ptr<T>::get()`).  

<a id="mdtoc_666234f9"></a>
#### getPointedType

```c++
const MetaType * getPointedType(const Variant & var);
```

Returns the meta type pointed by the pointer to the underlying value in `var`.  
If `var` is value or reference to value, returns the value type.  
If `var` is a pointer or reference to pointer, returns the type that the pointer points to.  
If `var` is pointer wrapper such as `std::shared_ptr<T>` or `std::unique_ptr<T>`, returns T.  

<a id="mdtoc_f8216508"></a>
#### getPointerAndType

```c++
std::pair<void *, const MetaType *> getPointerAndType(const Variant & var);
```

Returns both the pointer and pointed type in one function.  
It's slighter better performance than calling `getPointer` and `getPointedType` respectively.  

<a id="mdtoc_9637f10c"></a>
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

<a id="mdtoc_556c8b"></a>
#### dumpMetaType

```c++
void dumpMetaType(std::ostream & stream, const metapp::MetaType * metaType, const MetaRepo * metaRepo = nullptr);
```

Dump readable information of `metaType` to `stream`.  
The meta data is obtained from `MetaRepo` `metaRepo`. If `metaRepo` is nullptr, the global `MetaRepo` is used.  
This function is for test and learning purpose, you should not use it in any production code.  

<a id="mdtoc_ce758620"></a>
#### getNameByTypeKind

```c++
std::string getNameByTypeKind(const TypeKind typeKind);
```

Get the built-in name of `typeKind`. Note: the name doesn't depend on `MetaRepo`.

