[//]: # (Auto generated file, don't modify this file.)

# MetaType reference
<!--begintoc-->
- [Overview](#mdtoc_e7c3d1bb)
- [Header](#mdtoc_6e72a8c1)
- [Obtain the MetaType for given C++ type](#mdtoc_d959a9aa)
  - [Use getMetaType at compile time](#mdtoc_fe7cfc87)
  - [Use MetaType at runtime](#mdtoc_ad0f02a7)
- [Member functions](#mdtoc_9ab1cb86)
  - [Not constructible](#mdtoc_609b9aed)
  - [equal](#mdtoc_2eab15f7)
  - [compare](#mdtoc_bdc9085d)
  - [getUpType](#mdtoc_518056ac)
  - [getUpTypeCount](#mdtoc_9cfd111e)
  - [getTypeKind](#mdtoc_9973f311)
  - [isVoid](#mdtoc_d37463dc)
  - [Get type attributes](#mdtoc_4cda321c)
  - [Get meta interfaces](#mdtoc_cb9fad12)
  - [Check meta interfaces](#mdtoc_777f4af8)
  - [construct](#mdtoc_5af55aed)
  - [copyConstruct](#mdtoc_ae09b189)
  - [placementConstruct](#mdtoc_3c48c135)
  - [placementCopyConstruct](#mdtoc_49156376)
  - [destroy](#mdtoc_7c4a867b)
  - [dtor](#mdtoc_9a9618bc)
  - [canCast](#mdtoc_f164fa3f)
  - [getModule](#mdtoc_e3c125af)
<!--endtoc-->

<a id="mdtoc_e7c3d1bb"></a>
## Overview

`metapp::MetaType` represents the meta information of arbitrary C++ type.  
We can obtain almost all information on any C++ type through `metapp::MetaType`, such as,
but not limited to, pointer, reference, const-volatile qualifier, array, function parameters, etc.  

<a id="mdtoc_6e72a8c1"></a>
## Header

```c++
#include "metapp/metatype.h"
```

<a id="mdtoc_d959a9aa"></a>
## Obtain the MetaType for given C++ type

<a id="mdtoc_fe7cfc87"></a>
### Use getMetaType at compile time

```c++
template <typename T>
const MetaType * getMetaType();
```

`getMetaType` is a non-member free function.  
`getMetaType` returns a MetaType pointer of type T.  
`getMetaType` can be used on any C++ type, the type doesn't need any registering or preprocessing.  

The pointer returned by `getMetaType()` is always the same for the same T. For example,  

```c++
ASSERT(metapp::getMetaType<int>() == metapp::getMetaType<int>());
ASSERT(metapp::getMetaType<std::string>() == metapp::getMetaType<std::string>());
```

MetaType is CV-aware (CV means const-volatile). That's to say, for the same T,
different CV qualified types will result different MetaType. For example,  

```c++
ASSERT(metapp::getMetaType<int>() != metapp::getMetaType<const int>());
ASSERT(metapp::getMetaType<std::string>() != metapp::getMetaType<volatile std::string>());
```

To identify CV-unaware meta type, use `MetaType::equal()` or `MetaType::compare()`.  

<a id="mdtoc_ad0f02a7"></a>
### Use MetaType at runtime

The class `metapp::MetaRepo` is used to register and retrieve meta types at runtime.  
Please refer to the document for `metapp::MetaRepo` for details.

<a id="mdtoc_9ab1cb86"></a>
## Member functions

<a id="mdtoc_609b9aed"></a>
#### Not constructible

```c++
MetaType() = delete;
MetaType(const MetaType &) = delete;
MetaType(MetaType &&) = delete;
```

MetaType is not constructible. So the user can't construct a MetaType on the stack or heap.  

<a id="mdtoc_2eab15f7"></a>
#### equal

```c++
bool equal(const MetaType * other) const;
```

Returns true if `this` equals to `other`.  
The comparison ignores any CV qualifiers in the types, include top level CV, or CV in the pointer or reference.  
The CV qualifiers in template arguments, or function arguments, are not ignored.

```c++
// A type equals to itself.
ASSERT(metapp::getMetaType<int>()->equal(metapp::getMetaType<int>()));
// Any top level CV qualifiers are ignored.
ASSERT(metapp::getMetaType<int>()->equal(metapp::getMetaType<const int>()));

// Any top level CV qualifiers are ignored.
ASSERT(metapp::getMetaType<int *>()->equal(metapp::getMetaType<int * const>()));
// CV inside pointers or references are ignored.
ASSERT(metapp::getMetaType<int *>()->equal(metapp::getMetaType<const int *>()));
ASSERT(metapp::getMetaType<int const * volatile *>()->equal(metapp::getMetaType<int volatile * const *>()));
ASSERT(metapp::getMetaType<int &>()->equal(metapp::getMetaType<const int &>()));
ASSERT(metapp::getMetaType<int &&>()->equal(metapp::getMetaType<const int &&>()));

// Any top level CV qualifiers are ignored.
ASSERT(metapp::getMetaType<int[]>()->equal(metapp::getMetaType<const int[]>()));

// Different types don't equal.
ASSERT(! metapp::getMetaType<int>()->equal(metapp::getMetaType<long>()));
// Different pointers are different types.
ASSERT(! metapp::getMetaType<int *>()->equal(metapp::getMetaType<int **>()));

// Different CV in template argument causes different meta type.
ASSERT(! metapp::getMetaType<std::map<int, long> >()->equal(metapp::getMetaType<std::map<const int, long> >()));
```

<a id="mdtoc_bdc9085d"></a>
#### compare

```c++
int compare(const MetaType * other) const;
```

Compares `this` with `other`.  
Returns negative value if `this` is before `other`.  
Returns zero if `this` equals to `other`.  
Returns positive value if `this` is after `other`.  
The rule on CV qualifiers is same as `equal`.  
This function is useful when putting `MetaType` in ordered containers, such as `std::map`, `std::set`, etc.

<a id="mdtoc_518056ac"></a>
#### getUpType

```c++
const MetaType * getUpType() const; // #1
const MetaType * getUpType(const int i) const; // #2
```

Returns the pointer to UpType.  
The first form is same to `getUpType(0)`.  
A MetaType has 0, 1, or multiple UpTypes. An UpType is a pointer to MetaType object.  
A MetaType only has one TypeKind, so it can represent only one type information,
it can't represent compound information. For example, for type `int *`, the TypeKind of the MetaType is `tkPointer`,
the `int` type is missed. For `std::vector<char>`, the TypeKind of the MetaType is `tkStdVector`, the `char` type is missed.  
UpType is used to represent the missed type information.  
The actual types of UpType depends on how the meta type defines them. For example, for pointer (tkPointer),
it has only one UpType, which is the type it points to. Reference is similar to pointer.  

<a id="mdtoc_9cfd111e"></a>
#### getUpTypeCount

```c++
int getUpTypeCount() const noexcept;
```

Returns the count of UpType. The result can be 0, 1, or more.  

<a id="mdtoc_9973f311"></a>
#### getTypeKind

```c++
TypeKind getTypeKind() const noexcept;
```

Returns the TypeKind of the meta type.  

Note: whenever possible, you should prefer the type attributes and meta interfaces to TypeKind
when detecting the features of a MetaType.  
For example, instead of `metaType->getTypeKind() == metapp::tkPointer`, you should use `metaType->isPointer()`.  
Using type attributes and meta interfaces are more flexible and less error prone.  

<a id="mdtoc_d37463dc"></a>
#### isVoid

```c++
bool isVoid() const noexcept;
```

Returns true if the meta type is for `void` (tkVoid).  
`metapp` doesn't use nullptr meta type. When there is no appropriate underlying type, `void` is used.  
For example, the default constructed `Variant` (i.e, `Variant v`), has meta type of void.  

<a id="mdtoc_4cda321c"></a>
#### Get type attributes

```c++
constexpr bool isConst() const noexcept;
constexpr bool isVolatile() const noexcept;
constexpr bool isPointer() const noexcept;
constexpr bool isReference() const noexcept;
constexpr bool isClass() const noexcept;
constexpr bool isArray() const noexcept;
constexpr bool isEnum() const noexcept;
constexpr bool isMemberPointer() const noexcept;
constexpr bool isIntegral() const noexcept;
constexpr bool isFloat() const noexcept;
constexpr bool isArithmetic() const noexcept;
```

`isConst` returns true if the type is const qualified. It uses `std::is_const` to detect it.  
`isVolatile` returns true if the type is volatile qualified. It uses `std::is_volatile` to detect it.  
`isPointer` returns true if the type is a pointer or nullptr. It uses `std::is_pointer` and checking nullptr_t to detect it.  
`isReference` returns true if the type is a reference. It uses `std::is_reference` to detect it.  
`isClass` returns true if the type is a C++ class or struct.  It uses `std::is_class` to detect it. 
`isArray` returns true if the type is a raw array (int[3], std::string[2][3], etc). It uses `std::is_array` to detect it.  
`isEnum` returns true if the type is a C++ enum. It uses `std::is_enum` to detect it.    
`isMemberPointer` returns true if the type is a class member data or member function. It uses `std::is_member_pointer` to detect it.
To check if a MetaType is static member, check `! isMemberPointer()`.  
`isIntegral` returns true if the type is integral type. It uses `std::is_integral` to detect it.  
`isFloat` returns true if the type is float point type. It uses `std::is_floating_point` to detect it.  
`isArithmetic` returns true if the type is arithmetic type. It equals to `isIntegral() || isFloat()`.  

Note: the attributes are C++ type traits. They don't have connection to meta interface or other features in MetaType.
That's to say, `isClass()` returning true doesn't mean the MetaType implements `MetaClass` interface, etc.  

<a id="mdtoc_cb9fad12"></a>
#### Get meta interfaces

```c++
const MetaClass * getMetaClass() const;
const MetaCallable * getMetaCallable() const;
const MetaAccessible * getMetaAccessible() const;
const MetaEnum * getMetaEnum() const;
const MetaIndexable * getMetaIndexable() const;
const MetaIterable * getMetaIterable() const;
const MetaStreamable * getMetaStreamable() const;
const MetaMappable * getMetaMappable() const;
const void * getMetaUser() const;
```

The functions return the meta interfaces if they are implemented.
If any interface is not implemented by the meta type, `nullptr` is returned.  
MetaType has very few public functions, most functions are in the meta interfaces.  
Please see the document for each meta interface for more detailed information.  

<a id="mdtoc_777f4af8"></a>
#### Check meta interfaces

```c++
bool hasMetaClass() const;
bool hasMetaCallable() const;
bool hasMetaAccessible() const;
bool hasMetaEnum() const;
bool hasMetaIndexable() const;
bool hasMetaIterable() const;
bool hasMetaStreamable() const;
bool hasMetaMappable() const;
bool hasMetaUser() const;
```

Each function returns true if the MetaType implements the interface.  

<a id="mdtoc_5af55aed"></a>
#### construct

```c++
void * construct() const;
```

Similar to C++ code `new T()`.  
Allocate and initialize an object on the heap, then returns the object pointer.  
The returned pointer can be freed using `destroy`.  

<a id="mdtoc_ae09b189"></a>
#### copyConstruct

```c++
void * copyConstruct(const void * copyFrom) const;
```

Similar to C++ code `new T(anotherObject)`.  
Allocate and initialize an object on the heap, copy the object pointed by `copyFrom` to the object,
then returns the object pointer.  
The returned pointer can be freed using `destroy`.  

<a id="mdtoc_3c48c135"></a>
#### placementConstruct

```c++
void * placementConstruct(void * memory) const;
```

Similar to C++ code `new (memory) T()`.  
Initialize an object on the memory pointed by `memory`, then returns the object pointer.  
The returned pointer can be freed using `dtor`.  

<a id="mdtoc_49156376"></a>
#### placementCopyConstruct

```c++
void * placementCopyConstruct(void * memory, const void * copyFrom) const;
```

Similar to C++ code `new (memory) T(anotherObject)`.  
Initialize an object on the memory pointed by `memory`, copy the object pointed by `copyFrom` to the object,
then returns the object pointer.  
The returned pointer can be freed using `dtor`.  

<a id="mdtoc_7c4a867b"></a>
#### destroy

```c++
void destroy(void * instance) const;
```

Invoke the destructor then free the memory.  
`instance` must have the type of `this` MetaType.  
It's similar to C++ `delete (Cast to proper type *)instance`.  
The instance can be constructed by `construct` or `copyConstruct`, or constructed with `new` operator.  

<a id="mdtoc_9a9618bc"></a>
#### dtor

```c++
void dtor(void * instance) const;
```

Invoke the destructor but don't free the memory.  
This is useful to destruct the object constructed by `placementConstruct` or `placementCopyConstruct`.  

<a id="mdtoc_f164fa3f"></a>
#### canCast

```c++
bool canCast(const MetaType * toMetaType) const;
```

Return true if `this` MetaType can cast to `toMetaType`. This is similar to `Variant::canCast(const MetaType * toMetaType)`
with special case.  
The special case is, since `MetaType::canCast` can't provide any `Variant` for test, the meta types that depend on the
`Variant` value to test cast-ability will return inaccurate result. `Variant::canCast` doesn't have such problem because
it can provide the `Variant` value.  
In the built-in meta types, it will always return true in `MetaType::canCast`
if `this` MetaType is `std::function` (tkStdFunction), or `toMetaType` is `Variant` (tkVariant).

Usually `MetaType::canCast` should not be used unless you can't have the `Variant` value available, otherwise, always use
`Variant::canCast`.

<a id="mdtoc_e3c125af"></a>
#### getModule

```c++
const void * getModule() const noexcept;
```

Returns a pointer to represent which module the `MetaType` is from.  
Usually you don't need to use this function.  
"Module" means a dynamic library, or the main program.  
For the same module, the function always returns the same pointer for all meta types.  
For different modules, the pointers are different.  

