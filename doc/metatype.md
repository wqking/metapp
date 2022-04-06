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
```c++
metapp::getMetaType<int>() == metapp::getMetaType<int>();
metapp::getMetaType<std::string>() == metapp::getMetaType<std::string>();
```

MetaType is CV-aware (CV means const-volatile). That's to say, for the same T, different CV qualified types will result different MetaType. For example,  
```c++
metapp::getMetaType<int>() != metapp::getMetaType<const int>();
metapp::getMetaType<std::string>() != metapp::getMetaType<volatile std::string>();
```

To identify CV-unaware meta type, use `MetaType::getUnifiedType()`.  

**Example**

```c++
const metapp::MetaType * metaTypeInt = metapp::getMetaType<int>();
const metapp::MetaType * metaTypeStdString = metapp::getMetaType<std::string>();

struct MyClass { int whatEver; };
const metapp::MetaType * metaTypeStdString = metapp::getMetaType<MyClass>();
```

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
```

`isConst` returns true if the type is const qualified.  
`isVolatile` returns true if the type is volatile qualified.  
`isPointer` returns true if the type is a pointer or nullptr.  
`isReference` returns true if the type is a reference.  
`isClass` returns true if the type is a C++ class or struct.  
`isArray` returns true if the type is a raw array (int[3], std::string[2][3], etc).  

### Get meta interfaces

```c++
const MetaClass * getMetaClass() const;
const MetaCallable * getMetaCallable() const;
const MetaAccessible * getMetaAccessible() const;
const MetaEnum * getMetaEnum() const;
const MetaIndexable * getMetaIndexable() const;
const MetaIterable * getMetaIterable() const;
const MetaStreaming * getMetaStreaming() const;
const MetaMap * getMetaMap() const;
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
