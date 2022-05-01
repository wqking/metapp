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
# MetaType reference

## Overview

`metapp::MetaType` represents the meta information of arbitrary C++ type.  
We can obtain almost all information on any C++ type through `metapp::MetaType`, such as, but not limited to, pointer, reference, const-volatile qualifier, array, function parameters, etc.  

## Header

```c++
#include "metapp/metatype.h"
```

## Obtain the MetaType for given C++ type

### Use getMetaType at compile time

```c++
template <typename T>
const MetaType * getMetaType();
```

`getMetaType` is a non-member free function.  
Return a MetaType pointer of type T.  
`getMetaType` can be used on any C++ type, the type doesn't need any registering or preprocessing.  

The pointer returned by `getMetaType()` is always the same for the same T. For example,  
desc*/

UFN
{
	//code
	ASSERT(metapp::getMetaType<int>() == metapp::getMetaType<int>());
	ASSERT(metapp::getMetaType<std::string>() == metapp::getMetaType<std::string>());
	//code
}

/*desc
MetaType is CV-aware (CV means const-volatile). That's to say, for the same T, different CV qualified types will result different MetaType. For example,  
desc*/

UFN
{
	//code
	ASSERT(metapp::getMetaType<int>() != metapp::getMetaType<const int>());
	ASSERT(metapp::getMetaType<std::string>() != metapp::getMetaType<volatile std::string>());
	//code
}

/*desc
To identify CV-unaware meta type, use `MetaType::getUnifiedType()`.  

**Example**
desc*/

UFN
{
	//code
	ASSERT(metapp::getMetaType<int>()->getUnifiedType() == metapp::getMetaType<const int>()->getUnifiedType());
	ASSERT(metapp::getMetaType<std::string>()->getUnifiedType() == metapp::getMetaType<volatile std::string>()->getUnifiedType());
	//code
}

/*desc
### Use MetaRepo at runtime

The class `metapp::MetaRepo` holds all registered meta types.

## Member functions

### Not constructible

```c++
MetaType() = delete;
MetaType(const MetaType &) = delete;
MetaType(MetaType &&) = delete;
```

MetaType is not constructible. So the user can't construct a MetaType on the stack or heap.  

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

### getUpTypeCount

```c++
size_t getUpTypeCount() const noexcept;
```

Returns the count of UpType. The result can be 0, 1, or more.  

### getTypeKind

```c++
TypeKind getTypeKind() const noexcept;
```

Returns the TypeKind of the meta type.

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

### construct

```c++
void * construct() const;
```

Similar to C++ code `new T()`.  
Allocate and initialize an object on the heap, then returns the object pointer.  
The returned pointer can be freed using `destroy`.  

### copyConstruct

```c++
void * copyConstruct(const void * copyFrom) const;
```

Similar to C++ code `new T(anotherObject)`.  
Allocate and initialize an object on the heap, copy the object pointed by `copyFrom` to the object, then returns the object pointer.  
The returned pointer can be freed using `destroy`.  

### destroy

```c++
void destroy(void * instance) const;
```

Free an object constructed by `construct` or `copyConstruct`.  

desc*/
