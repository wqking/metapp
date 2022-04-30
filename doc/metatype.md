[//]: # (Auto generated file, don't modify this file.)

# MetaType reference
<!--begintoc-->
* [Overview](#a2_1)
* [Header](#a2_2)
* [Obtain the MetaType for given C++ type](#a2_3)
  * [Use getMetaType at compile time](#a3_1)
  * [Use MetaRepo at runtime](#a3_2)
* [Member functions](#a2_4)
  * [Not constructible](#a3_3)
  * [getUnifiedType](#a3_4)
  * [getUpType](#a3_5)
  * [getUpTypeCount](#a3_6)
  * [getTypeKind](#a3_7)
  * [Get type attributes](#a3_8)
  * [Get meta interfaces](#a3_9)
  * [construct](#a3_10)
  * [copyConstruct](#a3_11)
  * [destroy](#a3_12)
<!--endtoc-->

<a id="a2_1"></a>
## Overview

`metapp::MetaType` represents the meta information of arbitrary C++ type.  
We can obtain almost all information on any C++ type through `metapp::MetaType`, such as, but not limited to, pointer, reference, const-volatile qualifier, array, function parameters, etc.  

<a id="a2_2"></a>
## Header

```c++
#include "metapp/metatype.h"
```

<a id="a2_3"></a>
## Obtain the MetaType for given C++ type

<a id="a3_1"></a>
### Use getMetaType at compile time

```c++
template <typename T>
const MetaType * getMetaType();
```

`getMetaType` is a non-member free function.  
Return a MetaType pointer of type T.  
`getMetaType` can be used on any C++ type, the type doesn't need any registering or preprocessing.  

The pointer returned by `getMetaType()` is always the same for the same T. For example,  

```c++
ASSERT(metapp::getMetaType<int>() == metapp::getMetaType<int>());
ASSERT(metapp::getMetaType<std::string>() == metapp::getMetaType<std::string>());
```

MetaType is CV-aware (CV means const-volatile). That's to say, for the same T, different CV qualified types will result different MetaType. For example,  

```c++
ASSERT(metapp::getMetaType<int>() != metapp::getMetaType<const int>());
ASSERT(metapp::getMetaType<std::string>() != metapp::getMetaType<volatile std::string>());
```

To identify CV-unaware meta type, use `MetaType::getUnifiedType()`.  

**Example**

```c++
ASSERT(metapp::getMetaType<int>()->getUnifiedType() == metapp::getMetaType<const int>()->getUnifiedType());
ASSERT(metapp::getMetaType<std::string>()->getUnifiedType() == metapp::getMetaType<volatile std::string>()->getUnifiedType());
```

<a id="a3_2"></a>
### Use MetaRepo at runtime

The class `metapp::MetaRepo` holds all registered meta types.

<a id="a2_4"></a>
## Member functions

<a id="a3_3"></a>
### Not constructible

```c++
MetaType() = delete;
MetaType(const MetaType &) = delete;
MetaType(MetaType &&) = delete;
```

MetaType is not constructible. So the user can't construct a MetaType on the stack or heap.  

<a id="a3_4"></a>
### getUnifiedType

```c++
const void * getUnifiedType() const noexcept;
```

Returns the pointer to unified type.  
Each MetaType has one and only one UnifiedType. UnifiedType is similar to MetaType, except that UnifiedType is CV-unaware. That's to say, for the same T, no matter it's qualified with const or volatile, the UnifiedType is always the same. So,  
```c++
metapp::getMetaType<int>()->getUnifiedType() == metapp::getMetaType<const int>()->getUnifiedType();
metapp::getMetaType<std::string>()->getUnifiedType() == metapp::getMetaType<volatile std::string>()->getUnifiedType();
```
`UnifiedType` is an opaque type, so it's a `const void *`. Its only function is to identify or compare types.  

<a id="a3_5"></a>
### getUpType

```c++
const MetaType * getUpType() const; // #1
const MetaType * getUpType(const size_t i) const; // #2
```

Returns the pointer to UpType.  
The first form is same to `getUpType(0)`.  
A MetaType has 0, 1, or multiple UpTypes. An UpType is a pointer to MetaType object.  
A MetaType only has one TypeKind, so it can represent only one type information, it can't represent compound information. For example, for type `int *`, the TypeKind of the MetaType is `tkPointer`, the `int` type is missed. For `std::vector<char>`, the TypeKind of the MetaType is `tkStdVector`, the `char` type is missed.  
UpType is used to represent the missed type information.  
The actual types of UpType depends on how the meta type defines them. For example, for pointer (tkPointer), it has only one UpType, which is the type it points to. Reference is similar to pointer.  

<a id="a3_6"></a>
### getUpTypeCount

```c++
size_t getUpTypeCount() const noexcept;
```

Returns the count of UpType. The result can be 0, 1, or more.  

<a id="a3_7"></a>
### getTypeKind

```c++
TypeKind getTypeKind() const noexcept;
```

Returns the TypeKind of the meta type.

<a id="a3_8"></a>
### Get type attributes

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
`isIntegral` returns true if the type is integral type. It uses `std::is_integral` to detect it.  
`isFloat` returns true if the type is float point type. It uses `std::is_floating_point` to detect it.    
`isArithmetic` returns true if the type is arithmetic type. It equals to `isIntegral() || isFloat()`.  

Note: the attributes are C++ type traits. They don't have connection to meta interface or other features in MetaType. That's to say, `isClass()` returning true doesn't mean the MetaType implements `MetaClass` interface, etc.  

<a id="a3_9"></a>
### Get meta interfaces

```c++
const MetaClass * getMetaClass() const;
const MetaCallable * getMetaCallable() const;
const MetaAccessible * getMetaAccessible() const;
const MetaEnum * getMetaEnum() const;
const MetaIndexable * getMetaIndexable() const;
const MetaIterable * getMetaIterable() const;
const MetaStreaming * getMetaStreaming() const;
const MetaMappable * getMetaMappable() const;
const MetaMember * getMetaMember() const;
const void * getMetaUser() const;
```

The functions return the meta interfaces if they are implemented. If any interface is not implemented by the meta type, `nullptr` is returned.  
MetaType has very few public functions, most functions are in the meta interfaces.  
Please see the document for each meta interface for more detailed information.  

<a id="a3_10"></a>
### construct

```c++
void * construct() const;
```

Similar to C++ code `new T()`.  
Allocate and initialize an object on the heap, then returns the object pointer.  
The returned pointer can be freed using `destroy`.  

<a id="a3_11"></a>
### copyConstruct

```c++
void * copyConstruct(const void * copyFrom) const;
```

Similar to C++ code `new T(anotherObject)`.  
Allocate and initialize an object on the heap, copy the object pointed by `copyFrom` to the object, then returns the object pointer.  
The returned pointer can be freed using `destroy`.  

<a id="a3_12"></a>
### destroy

```c++
void destroy(void * instance) const;
```

Free an object constructed by `construct` or `copyConstruct`.  
