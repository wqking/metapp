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
# MetaPointerWrapper interface

## Overview

`MetaPointerWrapper` is a meta interface to get and set pointer in pointer wrapper.  
Pointer wrapper is a pointer like object that behaves similar to a pointer.  
The smart pointers `std::shared_ptr` and `std::unique_ptr` are pointer wrappers.  

## Header

```c++
#include "metapp/interfaces/metapointerwrapper.h"
```

## Get MetaPointerWrapper interface

We can call `MetaType::getMetaPointerWrapper()` to get the `MetaPointerWrapper` interface. If the type doesn't implement the interface,
`nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<std::shared_ptr<int> >();
const metapp::MetaPointerWrapper * metaAccessible = metaType->getMetaPointerWrapper();
```

## Implemented built-in meta types

std::shared_ptr<T> (tkStdSharedPtr)  
std::unique_ptr<T> (tkStdUniquePtr)  

## MetaPointerWrapper constructor

```c++
MetaPointerWrapper(
	Variant (*getPointer)(const Variant & pointerWrapper),
	void (*setPointer)(const Variant & pointerWrapper, const Variant & pointer)
);
```

All arguments are function pointers. All pointers must point to valid function.  
The meaning of each functions are same as the member functions listed below.

## MetaPointerWrapper member functions

The first parameter in all of the member functions is `const Variant & pointerWrapper`.
It's the Variant which meta type implements `MetaPointerWrapper`, and hold the proper data such as `std::shared_ptr`.
The member functions operate on the data.  
We can treat `pointerWrapper` as the C++ object instance which class implements an interface called `MetaPointerWrapper`.  
Variant `pointerWrapper` can be value that implements `MetaPointerWrapper`, or reference that refers to value that implements `MetaPointerWrapper`.  

#### getPointer

```c++
Variant getPointer(const Variant & pointerWrapper);
```

Returns the underlying pointer. It's similar to the `.get()` function in `std::shared_ptr` and `std::unique_ptr`.  
The returned Variant holds the pointer, and the property meta type and const-volatile information for the pointer. 

#### setPointer

```c++
void setPointer(const Variant & pointerWrapper, const Variant & pointer);
```

Sets the underlying pointer. It's similar to the `.reset()` function in `std::shared_ptr` and `std::unique_ptr`.  

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

desc*/
