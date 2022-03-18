# MetaType reference

## Overview

`metapp::MetaType` represents the meta information of arbitrary C++ type.  
We can obtain almost all information on any C++ type through `metapp::MetaType`, such as, but not limited to, pointer, reference, const-volatile qualifier, array, function parameters, etc.  

The pointer returned by `getMetaType()` is always the same for the same T. For example,  
```c++
getMetaType<int>() == getMetaType<int>();
getMetaType<std::string>() == getMetaType<std::string>();
```

MetaType is CV-aware (CV means const-volatile). That's to say, for the same T, different CV qualified types will result different MetaType. For example,  
```c++
getMetaType<int>() != getMetaType<const int>();
getMetaType<std::string>() != getMetaType<volatile std::string>();
```

To identify CV-unaware meta type, use `MetaType::getUnifiedType()`.  

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

**Example**

```c++
const metapp::MetaType * metaTypeInt = metapp::getMetaType<int>();
const metapp::MetaType * metaTypeStdString = metapp::getMetaType<std::string>();

struct MyClass { int whatEver; };
const metapp::MetaType * metaTypeStdString = metapp::getMetaType<MyClass>();
```

### Use MetaRepository at runtime

The class `metapp::MetaRepository` holds all registered meta types.

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
