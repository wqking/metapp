[//]: # (Auto generated file, don't modify this file.)

# MetaIndexable interface
<!--begintoc-->
- [Overview](#mdtoc_e7c3d1bb)
- [Header](#mdtoc_6e72a8c1)
- [Get MetaIndexable interface](#mdtoc_b9e3e5c5)
- [Implemented built-in meta types](#mdtoc_ed7f0e2e)
- [MetaIndexable constructor](#mdtoc_dd15cb12)
- [MetaIndexable member functions](#mdtoc_d0508714)
  - [getSizeInfo](#mdtoc_a5bec82e)
  - [getValueType](#mdtoc_8d778ce1)
  - [resize](#mdtoc_707e0e22)
  - [get](#mdtoc_fd3b2e70)
  - [set](#mdtoc_e61425dc)
- [Non-member utility functions](#mdtoc_e4e47ded)
<!--endtoc-->

<a id="mdtoc_e7c3d1bb"></a>
## Overview

`MetaIndexable` is a meta interface to get and set element values by index.  

<a id="mdtoc_6e72a8c1"></a>
## Header

```c++
#include "metapp/interfaces/metaindexable.h"
```

<a id="mdtoc_b9e3e5c5"></a>
## Get MetaIndexable interface

We can call `MetaType::getMetaIndexable()` to get the `MetaIndexable` interface. If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<std::vector<int> >();
const metapp::MetaIndexable * metaIndexable = metaType->getMetaIndexable();
```

<a id="mdtoc_ed7f0e2e"></a>
## Implemented built-in meta types

`std::array` (tkStdArray)  
`std::deque` (tkStdDeque)  
`std::vector` (tkStdVector)  
`std::list` (tkStdList)  
`std::pair` (tkStdPair)  
`std::tuple` (tkStdTuple)  
`C style array` (tkArray)  

<a id="mdtoc_dd15cb12"></a>
## MetaIndexable constructor

```c++
MetaIndexable(
  SizeInfo (*getSizeInfo)(const Variant & indexable),
  const MetaType * (*getValueType)(const Variant & indexable, const std::size_t index),
  void (*resize)(const Variant & indexable, const std::size_t size),
  Variant (*get)(const Variant & indexable, const std::size_t index),
  void (*set)(const Variant & indexable, const std::size_t index, const Variant & value)
);
```

All arguments are function pointers. All pointers must point to valid function, except that `resize` can be nullptr.  
The meaning of each functions are same as the member functions listed below.

<a id="mdtoc_d0508714"></a>
## MetaIndexable member functions

The first parameter in all of the member functions is `const Variant & indexable`.
It's the Variant which meta type implements `MetaIndexable`, and hold the proper data such as `std::vector`.
The member functions operate on the data.  
We can treat `indexable` as the C++ object instance which class implements an interface called `MetaIndexable`.  
Variant `indexable` can be value that implements `MetaIndexable`, or reference that refers to value that implements `MetaIndexable`.  

For the functions that have parameter `index`, the functions don't do bound checking on `index`.
It's the caller's responsibility to be sure the `index` is valid.

<a id="mdtoc_a5bec82e"></a>
#### getSizeInfo

```c++
MetaIndexable::SizeInfo getSizeInfo(const Variant & indexable);
```

Returns the SizeInfo of the indexable.  

SizeInfo class,

```c++
class MetaIndexable::SizeInfo
{
public:
  SizeInfo();
  explicit SizeInfo(const std::size_t size);

  std::size_t getSize() const;
  bool isResizable() const;
  bool isUnknownSize() const;

  void setResizable(const bool value);
  void setUnknowSize(const bool value);
};
```

The constructors, `setResizable`, `setUnknowSize` are used by `MetaIndexable` implementations.  
'getSize', `isResizable`, `isUnknownSize` are used by `MetaIndexable` users.  

`size` is the number of elements in the indexable.  
`resizable` means whether the indexable can be changed to another size by calling `resize`.  
`unknownSize` is false by default for most cases, exception the type `T[]` has true unknownSize. If unknownSize is true,
both `size` and `resizable` don't make sense.  

For `std::pair`, size is 2, resizable is false.  
For `std::tuple`, size is the tuple size, resizable is false.  
For `T[]`, unknownSize is true.  
For `T[N]`, size is N, resizable is false, unknownSize is false.  
For `std::array<T, N>`, size is N, resizable is false.  
For other containers such as `std::vector`, `std::deque`, `std::list`, the function returns the size of the container,
and resizable is true.  

<a id="mdtoc_8d778ce1"></a>
#### getValueType

```c++
const MetaType * getValueType(const Variant & indexable, const std::size_t index);
```

Returns the meta type of the element at `index`.  

For `std::pair`, index 0 is the first value, index 1 is the second value.  
For `std::tuple`, index is the index in the tuple elements.  
For `T[]` and `T[N]`, the result is the meta type of T.  
For other containers, the result is the meta type of the `value_type` in the container.  

<a id="mdtoc_707e0e22"></a>
#### resize

```c++
void resize(const Variant & indexable, const std::size_t size);
```

Resizes the container in the Variant to contain `size` elements.  
For types that the size is fixed, such as `std::pair`, `std::tuple`, `T[]`, and `T[N]`, the function doesn't do anything.  
For other containers, the function resizes the container.  
If the `resize` argument in MetaIndexable constructor is nullptr, the function `resize` is still valid function, and it doesn't do any action.  

<a id="mdtoc_fd3b2e70"></a>
#### get

```c++
Variant get(const Variant & indexable, const std::size_t index);
```

Returns a reference to the element at `index`.  
For `std::pair`, index 0 returns the first value, index 1 returns the second value.  
For `std::tuple`, returns the tuple element at index.  
For `T[]` and `T[N]`, returns T[index].  
For other containers, returns the element at index in the container.  
Note, for non-random access container such as `std::list`, the function uses `std::advance(list.begin(), index)` to access the element, the time complexity is linear.  

<a id="mdtoc_e61425dc"></a>
#### set

```c++
void (*set)(const Variant & indexable, const std::size_t index, const Variant & value);
```

Sets the element at `index` with `value`. The `value` will be casted to the element type, if the cast fails, exception `metapp::BadCastException` is thrown.  

<a id="mdtoc_e4e47ded"></a>
## Non-member utility functions

Below free functions are shortcut functions to use the member functions in `MetaIndexable`.  
Usually you should prefer the utility functions to calling `MetaIndexable` member function directly. However, if you need to call functions on a single `MetaIndexable` more than one times in a high performance application, you may store `indexable.getMetaType()->getMetaIndexable()` to a local variable, then use the variable to call the member functions. This is because `getMetaIndexable()` has slightly performance overhead (the overhead is neglect most time).

```c++
inline SizeInfo indexableGetSizeInfo(const Variant & indexable)
{
  return indexable.getMetaType()->getMetaIndexable()->getSizeInfo(indexable);
}

inline void indexableResize(const Variant & indexable, const std::size_t size)
{
  indexable.getMetaType()->getMetaIndexable()->resize(indexable, size);
}

inline Variant indexableGet(const Variant & indexable, const std::size_t index)
{
  return indexable.getMetaType()->getMetaIndexable()->get(indexable, index);
}

inline void indexableSet(const Variant & indexable, const std::size_t index, const Variant & value)
{
  indexable.getMetaType()->getMetaIndexable()->set(indexable, index, value);
}
```

