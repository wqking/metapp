# Tutorial for using MetaType

`metapp::MetaType` represents the meta information of arbitrary C++ type.  
We can obtain almost all information on any C++ type through `metapp::MetaType`,
such as, but not limited to, pointer, reference, const-volatile qualifier, array, function parameters, etc.  

## Include headers
To use all declared meta types, include this header

```c++
#include "metapp/allmetatypes.h"
```

## Identify and compare MetaType

We can use metapp::getMetaType<T>() to get the MetaType for T at compile time.  
For the same type with same constness, metapp::getMetaType is always the same.

```c++
ASSERT(metapp::getMetaType<int>() == metapp::getMetaType<int>());
ASSERT(metapp::getMetaType<const std::string>() == metapp::getMetaType<const std::string>());
ASSERT(metapp::getMetaType<const volatile double>() == metapp::getMetaType<const volatile double>());
```

Different constness give different MetaType.

```c++
ASSERT(metapp::getMetaType<int>() != metapp::getMetaType<const int>());
ASSERT(metapp::getMetaType<const std::string>() != metapp::getMetaType<volatile std::string>());
ASSERT(metapp::getMetaType<const volatile double>() != metapp::getMetaType<volatile double>());
```

Different constness give same UnifiedType.

```c++
ASSERT(metapp::getMetaType<int>()->getUnifiedType() == metapp::getMetaType<const int>()->getUnifiedType());
ASSERT(metapp::getMetaType<const std::string>()->getUnifiedType() == metapp::getMetaType<volatile std::string>()->getUnifiedType());
ASSERT(metapp::getMetaType<const volatile double>()->getUnifiedType() == metapp::getMetaType<volatile double>()->getUnifiedType());
```

## Inspect MetaType attributes

int

```c++
const metapp::MetaType * metaType = metapp::getMetaType<int>();
ASSERT(metaType->getTypeKind() == metapp::tkInt);
```

constness

```c++
auto metaType = metapp::getMetaType<const int>();
ASSERT(metaType->getTypeKind() == metapp::tkInt);
ASSERT(metaType->isConst());
```

pointer

```c++
auto metaType = metapp::getMetaType<const int *>();
ASSERT(metaType->getTypeKind() == metapp::tkPointer);
ASSERT(metaType->getUpType()->getTypeKind() == metapp::tkInt);
```

"const int *" is not a const type.

```c++
ASSERT(! metaType->isConst());
```

The UpType "const int" is const.

```c++
ASSERT(metaType->getUpType()->isConst());
```

std::map

```c++
auto metaType = metapp::getMetaType<std::map<long, std::string> >();
ASSERT(metaType->getTypeKind() == metapp::tkStdMap);
```

it has two UpType, the first is long, the second is std::string.

```c++
ASSERT(metaType->getUpTypeCount() == 2);
ASSERT(metaType->getUpType(0)->getTypeKind() == metapp::tkLong);
ASSERT(metaType->getUpType(1)->getTypeKind() == metapp::tkStdString);
```

## Working with up type

MetaType `int` doesn't have any UpType.  

```c++
const metapp::MetaType * metaType = metapp::getMetaType<int>();
ASSERT(metaType->getTypeKind() == metapp::tkInt);
ASSERT(metaType->getUpTypeCount() == 0);
ASSERT(metaType->getUpType() == nullptr);
```

MetaType `int *` has one UpType.  

```c++
const metapp::MetaType * metaType = metapp::getMetaType<int *>();
ASSERT(metaType->getTypeKind() == metapp::tkPointer);
ASSERT(metaType->getUpTypeCount() == 1);
ASSERT(metaType->getUpType() == metapp::getMetaType<int>());
```
