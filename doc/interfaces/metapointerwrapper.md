[//]: # (Auto generated file, don't modify this file.)

# MetaPointerWrapper interface
<!--begintoc-->
* [Overview](#a2_1)
* [Header](#a2_2)
* [Get MetaPointerWrapper interface](#a2_3)
* [Implemented built-in meta types](#a2_4)
* [MetaPointerWrapper constructor](#a2_5)
* [MetaPointerWrapper member functions](#a2_6)
  * [getPointer](#a4_1)
  * [setPointer](#a4_2)
* [Non-member utility functions](#a2_7)
<!--endtoc-->

<a id="a2_1"></a>
## Overview

`MetaPointerWrapper` is a meta interface to get and set pointer in pointer wrapper.  
Pointer wrapper is a pointer like object that behaves similar to a pointer.  
The smart pointers `std::shared_ptr` and `std::unique_ptr` are pointer wrappers.  

<a id="a2_2"></a>
## Header

```c++
#include "metapp/interfaces/metapointerwrapper.h"
```

<a id="a2_3"></a>
## Get MetaPointerWrapper interface

We can call `MetaType::getMetaPointerWrapper()` to get the `MetaPointerWrapper` interface. If the type doesn't implement the interface,
`nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<std::shared_ptr<int> >();
const metapp::MetaPointerWrapper * metaAccessible = metaType->getMetaPointerWrapper();
```

<a id="a2_4"></a>
## Implemented built-in meta types

std::shared_ptr<T> (tkStdSharedPtr)  
std::unique_ptr<T> (tkStdUniquePtr)  

<a id="a2_5"></a>
## MetaPointerWrapper constructor

```c++
MetaPointerWrapper(
  Variant (*getPointer)(const Variant & pointerWrapper),
  void (*setPointer)(const Variant & pointerWrapper, const Variant & pointer)
);
```

All arguments are function pointers. All pointers must point to valid function.  
The meaning of each functions are same as the member functions listed below.

<a id="a2_6"></a>
## MetaPointerWrapper member functions

The first parameter in all of the member functions is `const Variant & pointerWrapper`.
It's the Variant which meta type implements `MetaPointerWrapper`, and hold the proper data such as `std::shared_ptr`.
The member functions operate on the data.  
We can treat `pointerWrapper` as the C++ object instance which class implements an interface called `MetaPointerWrapper`.  
Variant `pointerWrapper` can be value or reference.  

<a id="a4_1"></a>
#### getPointer

```c++
Variant getPointer(const Variant & pointerWrapper);
```

Returns the underlying pointer. It's similar to the `.get()` function in `std::shared_ptr` and `std::unique_ptr`.  
The returned Variant holds the pointer, and the property meta type and const-volatile information for the pointer. 

<a id="a4_2"></a>
#### setPointer

```c++
void setPointer(const Variant & pointerWrapper, const Variant & pointer);
```

Sets the underlying pointer. It's similar to the `.reset()` function in `std::shared_ptr` and `std::unique_ptr`.  

<a id="a2_7"></a>
## Non-member utility functions

Below free functions are shortcut functions to use the member functions in `MetaPointerWrapper`.  
Usually you should prefer the utility functions to calling `MetaPointerWrapper` member function directly.
However, if you need to call functions on a single `MetaPointerWrapper` more than one times in a high performance application,
you may store `pointerWrapper.getMetaType()->getMetaPointerWrapper()` to a local variable, then use the variable
to call the member functions. This is because `getMetaPointerWrapper()` has slightly performance overhead (the overhead is neglect most time).

```c++
inline Variant pointerWrapperGetPointer(const Variant & pointerWrapper)
{
  return getNonReferenceMetaType(pointerWrapper)->getMetaPointerWrapper()->getPointer(pointerWrapper);
}

inline void pointerWrapperSetPointer(const Variant & pointerWrapper, const Variant & pointer)
{
  getNonReferenceMetaType(pointerWrapper)->getMetaPointerWrapper()->setPointer(pointerWrapper, pointer);
}
```
