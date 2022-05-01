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
# MetaIterable interface

## Overview

`MetaIterable` is a meta interface to loop through a container.  
It's the metapp version of C++ "range-based for loop".  

## Header

```c++
#include "metapp/interfaces/metaiterable.h"
```

## Get MetaIterable interface

We can call `MetaType::getMetaIterable()` to get the `MetaIterable` interface. If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<std::vector<int> >();
const metapp::MetaIterable * metaIterable = metaType->getMetaIterable();
```

## Implemented built-in meta types

`std::array` (tkStdArray)  
`std::deque` (tkStdDeque)  
`std::vector` (tkStdVector)  
`std::forward_list` (tkStdForwardList)  
`std::list` (tkStdList)  
`std::map` (tkStdMap)  
`std::multimap` (tkStdMultimap)  
`std::set` (tkStdSet)  
`std::multiset` (tkStdMultiset)  
`std::unordered_map` (tkStdUnorderedMap)  
`std::unordered_multimap` (tkStdUnorderedMultimap)  
`std::unordered_set` (tkStdUnorderedSet)  
`std::unordered_multiset` (tkStdUnorderedMultiset)  
`std::tuple` (tkStdTuple)  

## MetaIterable constructor

```c++
explicit MetaIterable(
	void (*forEach)(const Variant & var, Callback callback)
);
```

Parameter `forEach` is a function pointer. It must point to valid function.  
The meaning of `forEach` is same as the member function listed below.

## MetaIterable member functions

#### forEach

```c++
void forEach(const Variant & var, MetaIterable::Callback callback);

using Callback = std::function<bool (const Variant &)>;
```

The first parameter `var` is the Variant which meta type implements `MetaIndexable`, and hold the proper data such as `std::vector`. The member functions operate on the data.  
We can treat `var` as the C++ object instance which class implements an interface called `MetaIndexable`.  
`var` can be a value, a reference, or a pointer.  
The second parameter `callback` is a callback function. Its prototype is `std::function<bool (const Variant &)>`.  

When `forEach` is invoked, `callback` is called for every element in `var`, and the referent to the element is passed as the parameter of the `callback`. If `callback` returns true, `forEach` will continue on next element, until there is no more elements. If `callback` returns false, `forEach` will stop the loop and return.  
Note: for STL containers, the element is the `value_type` in the container. That means for associative containers such as `std::map`, the element is a `std::pair` of the key and value.  

## Non-member utility functions

Below free functions are shortcut functions to use the member functions in `MetaIterable`.  
Usually you should prefer the utility functions to calling `MetaIterable` member function directly. However, if you need to call functions on a single `MetaIterable` more than one times in a high performance application, you may store `var.getMetaType()->getMetaIterable()` to a local variable, then use the variable to call the member functions. This is because `MetaIterable()` has slightly performance overhead (the overhead is neglect most time).

```c++
inline void iterableForEach(const Variant & var, MetaIterable::Callback callback)
{
	var.getMetaType()->getMetaIterable()->forEach(var, callback);
}
```

desc*/
