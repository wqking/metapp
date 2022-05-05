[//]: # (Auto generated file, don't modify this file.)

# Utility reference
<!--begintoc-->
* [Overview](#a2_1)
* [Header](#a2_2)
* [Functions](#a2_3)
  * [getNonReferenceMetaType](#a4_1)
  * [getReferredMetaType](#a4_2)
  * [selectOverload](#a4_3)
  * [dumpMetaType](#a4_4)
  * [getNameByTypeKind](#a4_5)
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
inline const MetaType * getNonReferenceMetaType(const MetaType * metaType)
{
  if(metaType->isReference()) {
    metaType = metaType->getUpType();
  }
  return metaType;
}

inline const MetaType * getNonReferenceMetaType(const Variant & value)
{
  return getNonReferenceMetaType(value.getMetaType());
}
```

If the `metaType`, or meta type in the Variant `value`, is a reference, returns the meta type the reference refers to.  
If the meta type is not a reference, returns the meta type.

<a id="a4_2"></a>
#### getReferredMetaType

```c++
inline const MetaType * getReferredMetaType(const MetaType * metaType)
{
  if(metaType->isPointer() || metaType->isReference()) {
    metaType = metaType->getUpType();
  }
  return metaType;
}

inline const MetaType * getReferredMetaType(const Variant & value)
{
  return getReferredMetaType(value.getMetaType());
}
```

If the `metaType`, or meta type in the Variant `value`, is a reference or pointer, returns the meta type the reference refers to, or the pointer points to.  
If the meta type is not a reference nor pointer, returns the meta type.

<a id="a4_3"></a>
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

<a id="a4_4"></a>
#### dumpMetaType

```c++
void dumpMetaType(std::ostream & stream, const metapp::MetaType * metaType, const MetaRepo * metaRepo = nullptr);
```

Dump readable information of `metaType` to `stream`.  
The meta data is obtained from `MetaRepo` `metaRepo`. If `metaRepo` is nullptr, the global `MetaRepo` is used.  
This function is for test and learning purpose, you should not use it in any production code.  

<a id="a4_5"></a>
#### getNameByTypeKind

```c++
std::string getNameByTypeKind(const TypeKind typeKind);
```

Get the built-in name of `typeKind`. Note: the name doesn't depend on `MetaRepo`.
