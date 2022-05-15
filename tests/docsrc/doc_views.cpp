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
# MetaItemView and BaseView

## Overview

`MetaItemView` and `BaseView` are STL-like containers that are used to view the underlying data.  
`MetaItemView` is used when retrieve multiple `MetaItem`s from `MetaRepo` or `MetaClass`.  
`BaseView` is used when retrieve base or derived classes from `MetaRepo`.  

## Header

There are no special headers for `MetaItemView` and `BaseView`.  
Whenever they are returned from the library, they are already included and ready to use.  

Both `MetaItemView` and `BaseView` have the same API interface.
Below document will use both MetaItemView/BaseView as example in the pseudo code.  

## Member types

```c++
using value_type = const MetaItemView or BaseView;
using size_type = int;
using difference_type = typename Iterator::difference_type;
using reference = const value_type &;
using const_reference = const value_type &;
using pointer = const value_type *;
using const_pointer = const value_type *;
using iterator = Iterator;
using const_iterator = Iterator;
using reverse_iterator = std::reverse_iterator<iterator>;
using const_reverse_iterator = std::reverse_iterator<const_iterator>;
```

All value types, reference, and pointer are constants. The containers are designed to read only.  

## Member functions

```c++
iterator begin() const;
iterator end() const;

bool empty() const ;
size_type size() const;

reference at(const size_type index) const;
reference operator [] (const size_type index) const;
```

Note the views have copy/move constructors and copy/move assignment operators. They are copyable and movable.

## iterator

The iterator is a forward_iterator.

desc*/
