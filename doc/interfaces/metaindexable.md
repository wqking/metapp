[//]: # (Auto generated file, don't modify this file.)

# MetaIndexable interface
<!--begintoc-->
* [Overview](#a2_1)
* [Header](#a2_2)
* [Get MetaIndexable interface](#a2_3)
* [Implemented built-in meta types](#a2_4)
* [MetaIndexable constructor](#a2_5)
* [MetaIndexable member functions](#a2_6)
  * [getSize](#a4_1)
  * [getValueType](#a4_2)
  * [resize](#a4_3)
  * [get](#a4_4)
  * [set](#a4_5)
* [Non-member utility functions](#a2_7)
<!--endtoc-->

<a id="a2_1"></a>
## Overview

`MetaIndexable` is a meta interface to get and set element values by index.  

<a id="a2_2"></a>
## Header

```c++
#include "metapp/interfaces/metaindexable.h"
```

<a id="a2_3"></a>
## Get MetaIndexable interface

We can call `MetaType::getMetaIndexable()` to get the `MetaIndexable` interface. If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<std::vector<int> >();
const metapp::MetaIndexable * metaIndexable = metaType->getMetaIndexable();
```

<a id="a2_4"></a>
## Implemented built-in meta types

`std::array` (tkStdArray)  
`std::deque` (tkStdDeque)  
`std::vector` (tkStdVector)  
`std::list` (tkStdList)  
`std::pair` (tkStdPair)  
`std::tuple` (tkStdTuple)  
`C style array` (tkArray)  

<a id="a2_5"></a>
## MetaIndexable constructor

```c++
MetaIndexable(
  size_t (*getSize)(const Variant & var),
  const MetaType * (*getValueType)(const Variant & var, const size_t index),
  void (*resize)(const Variant & var, const size_t size),
  Variant (*get)(const Variant & var, const size_t index),
  void (*set)(const Variant & var, const size_t index, const Variant & value)
);
```

All arguments are function pointers. All pointers must point to valid function, except that `resize` can be nullptr.  
The meaning of each functions are same as the member functions listed below.

<a id="a2_6"></a>
## MetaIndexable member functions

The first parameter in all of the member functions is `const Variant & var`. It's the Variant which meta type implements `MetaIndexable`, and hold the proper data such as `std::vector`. The member functions operate on the data.  
We can treat `var` as the C++ object instance which class implements an interface called `MetaIndexable`.  
`var` can be a value, a reference, or a pointer.  

For the functions that have parameter `index`, the functions don't do bound checking on `index`. It's the caller's responsibility to be sure the `index` is valid.

<a id="a4_1"></a>
#### getSize

```c++
size_t getSize(const Variant & var);
```

Returns the number of elements in the Variant.  
If the number is unknown, `MetaIndexable::unknowSize` is returned  
For `std::pair`, the function returns 2.  
For `std::tuple`, the function returns the tuple size.  
For `T[]`, the function returns `MetaIndexable::unknowSize`.  
For `T[N]`, the function returns N.  
For other containers such as `std::vector`, the function returns the size of the container.  

<a id="a4_2"></a>
#### getValueType

```c++
const MetaType * getValueType(const Variant & var, const size_t index);
```

Returns the meta type of the element at `index`.  

For `std::pair`, index 0 is the first value, index 1 is the second value.  
For `std::tuple`, index is the index in the tuple elements.  
For `T[]` and `T[N]`, the result is the meta type of T.  
For other containers, the result is the meta type of the `value_type` in the container.  

<a id="a4_3"></a>
#### resize

```c++
void resize(const Variant & var, const size_t size);
```

Resizes the container in the Variant to contain `size` elements.  
For types that the size is fixed, such as `std::pair`, `std::tuple`, `T[]`, and `T[N]`, the function doesn't do anything.  
For other containers, the function resizes the container.  
If the `resize` argument in MetaIndexable constructor is nullptr, the function `resize` is still valid function, and it doesn't do any action.  

<a id="a4_4"></a>
#### get

```c++
Variant get(const Variant & var, const size_t index);
```

Returns a reference to the element at `index`.  
For `std::pair`, index 0 returns the first value, index 1 returns the second value.  
For `std::tuple`, returns the tuple element at index.  
For `T[]` and `T[N]`, returns T[index].  
For other containers, returns the element at index in the container.  
Note, for non-random access container such as `std::list`, the function uses `std::advance(list.begin(), index)` to access the element, the time complexity is linear.  

<a id="a4_5"></a>
#### set

```c++
void (*set)(const Variant & var, const size_t index, const Variant & value);
```

Sets the element at `index` with `value`. The `value` will be casted to the element type, if the cast fails, exception `metapp::BadCastException` is thrown.  

<a id="a2_7"></a>
## Non-member utility functions

Below free functions are shortcut functions to use the member functions in `MetaIndexable`.  
Usually you should prefer the utility functions to calling `MetaIndexable` member function directly. However, if you need to call functions on a single `MetaIndexable` more than one times in a high performance application, you may store `var.getMetaType()->getMetaIndexable()` to a local variable, then use the variable to call the member functions. This is because `getMetaIndexable()` has slightly performance overhead (the overhead is neglect most time).

```c++
inline size_t indexableGetSize(const Variant & var)
{
  return var.getMetaType()->getMetaIndexable()->getSize(var);
}

inline void indexableResize(const Variant & var, const size_t size)
{
  var.getMetaType()->getMetaIndexable()->resize(var, size);
}

inline Variant indexableGet(const Variant & var, const size_t index)
{
  return var.getMetaType()->getMetaIndexable()->get(var, index);
}

inline void indexableSet(const Variant & var, const size_t index, const Variant & value)
{
  var.getMetaType()->getMetaIndexable()->set(var, index, value);
}
```
