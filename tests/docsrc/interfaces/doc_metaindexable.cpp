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

#include "testutil.h"

#include "metapp/allmetatypes.h"

/*desc
# MetaIndexable interface

## Overview

`MetaIndexable` is a meta interface to get and set element values by index.  

## Header

```c++
#include "metapp/interfaces/metaindexable.h"
```

## Get MetaIndexable interface

We can call `MetaType::getMetaIndexable()` to get the `MetaIndexable` interface. If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<std::vector<int> >();
const metapp::MetaIndexable * metaIndexable = metaType->getMetaIndexable();
```

## Implemented built-in meta types

`std::array` (tkStdArray)  
`std::deque` (tkStdDeque)  
`std::vector` (tkStdVector)  
`std::list` (tkStdList)  
`std::pair` (tkStdPair)  
`std::tuple` (tkStdTuple)  
`C style array` (tkArray)  

## MetaIndexable constructor

```c++
MetaIndexable(
	SizeInfo (*getSizeInfo)(const Variant & indexable),
	const MetaType * (*getValueType)(const Variant & indexable, const size_t index),
	void (*resize)(const Variant & indexable, const size_t size),
	Variant (*get)(const Variant & indexable, const size_t index),
	void (*set)(const Variant & indexable, const size_t index, const Variant & value)
);
```

All arguments are function pointers. All pointers must point to valid function, except that `resize` can be nullptr.  
The meaning of each functions are same as the member functions listed below.

## MetaIndexable member functions

The first parameter in all of the member functions is `const Variant & indexable`. It's the Variant which meta type implements `MetaIndexable`, and hold the proper data such as `std::vector`. The member functions operate on the data.  
We can treat `indexable` as the C++ object instance which class implements an interface called `MetaIndexable`.  
`indexable` can be a value, a reference, or a pointer.  

For the functions that have parameter `index`, the functions don't do bound checking on `index`. It's the caller's responsibility to be sure the `index` is valid.

#### getSizeInfo

```c++
MetaIndexable::SizeInfo getSizeInfo(const Variant & indexable);
```

Returns the SizeInfo of the indexable.  
If the number is unknown, `MetaIndexable::unknowSize` is returned  
For `std::pair`, the function returns 2.  
For `std::tuple`, the function returns the tuple size.  
For `T[]`, the function returns `MetaIndexable::unknowSize`.  
For `T[N]`, the function returns N.  
For other containers such as `std::vector`, the function returns the size of the container.  

#### getValueType

```c++
const MetaType * getValueType(const Variant & indexable, const size_t index);
```

Returns the meta type of the element at `index`.  

For `std::pair`, index 0 is the first value, index 1 is the second value.  
For `std::tuple`, index is the index in the tuple elements.  
For `T[]` and `T[N]`, the result is the meta type of T.  
For other containers, the result is the meta type of the `value_type` in the container.  

#### resize

```c++
void resize(const Variant & indexable, const size_t size);
```

Resizes the container in the Variant to contain `size` elements.  
For types that the size is fixed, such as `std::pair`, `std::tuple`, `T[]`, and `T[N]`, the function doesn't do anything.  
For other containers, the function resizes the container.  
If the `resize` argument in MetaIndexable constructor is nullptr, the function `resize` is still valid function, and it doesn't do any action.  

#### get

```c++
Variant get(const Variant & indexable, const size_t index);
```

Returns a reference to the element at `index`.  
For `std::pair`, index 0 returns the first value, index 1 returns the second value.  
For `std::tuple`, returns the tuple element at index.  
For `T[]` and `T[N]`, returns T[index].  
For other containers, returns the element at index in the container.  
Note, for non-random access container such as `std::list`, the function uses `std::advance(list.begin(), index)` to access the element, the time complexity is linear.  

#### set

```c++
void (*set)(const Variant & indexable, const size_t index, const Variant & value);
```

Sets the element at `index` with `value`. The `value` will be casted to the element type, if the cast fails, exception `metapp::BadCastException` is thrown.  

## Non-member utility functions

Below free functions are shortcut functions to use the member functions in `MetaIndexable`.  
Usually you should prefer the utility functions to calling `MetaIndexable` member function directly. However, if you need to call functions on a single `MetaIndexable` more than one times in a high performance application, you may store `indexable.getMetaType()->getMetaIndexable()` to a local variable, then use the variable to call the member functions. This is because `getMetaIndexable()` has slightly performance overhead (the overhead is neglect most time).

```c++
inline SizeInfo indexableGetSizeInfo(const Variant & indexable)
{
	return indexable.getMetaType()->getMetaIndexable()->getSizeInfo(indexable);
}

inline void indexableResize(const Variant & indexable, const size_t size)
{
	indexable.getMetaType()->getMetaIndexable()->resize(indexable, size);
}

inline Variant indexableGet(const Variant & indexable, const size_t index)
{
	return indexable.getMetaType()->getMetaIndexable()->get(indexable, index);
}

inline void indexableSet(const Variant & indexable, const size_t index, const Variant & value)
{
	indexable.getMetaType()->getMetaIndexable()->set(indexable, index, value);
}
```

desc*/
