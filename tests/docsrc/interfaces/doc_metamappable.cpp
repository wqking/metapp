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
# MetaMappable interface

## Overview

`MetaMappable` is a meta interface to get and set elements in associative containers.  

## Header
desc*/

//code
#include "metapp/interfaces/metamappable.h"
//code

/*desc
## Get MetaMappable interface

We can call `MetaType::getMetaMappable()` to get the `MetaMappable` interface.
If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<std::vector<int> >();
const metapp::MetaMappable * metaMappable = metaType->getMetaMappable();
```

## Implemented built-in meta types

`std::map` (tkStdMap)  
`std::multimap` (tkStdMultimap)  
`std::unordered_map` (tkStdUnorderedMap)  
`std::unordered_multimap` (tkStdUnorderedMultimap)  

## MetaMappable constructor

```c++
MetaMappable(
	const MetaType * (*getValueType)(const Variant & mappable),
	Variant (*get)(const Variant & mappable, const Variant & key),
	void (*set)(const Variant & mappable, const Variant & key, const Variant & value)
);
```

All arguments are function pointers. All pointers must point to valid function.  
The meaning of each functions are same as the member functions listed below.  

## MetaMappable member functions

The first parameter in all of the member functions is `const Variant & mappable`.
It's the Variant which meta type implements `MetaMappable`, and hold the proper data such as `std::vector`.
The member functions operate on the data.  
We can treat `mappable` as the C++ object instance which class implements an interface called `MetaMappable`.  
Variant `mappable` can be value that implements `MetaMappable`, or reference that refers to value that implements `MetaMappable`.  

#### getValueType

```c++
const MetaType * getValueType(const Variant & mappable);
```

Returns the meta type of the value type. The returned meta type is a type of `std::pair`, the up type at 0 is the key type,
the up type at 1 is the value type. Example,  
desc*/

ExampleFunc
{
	//code
	metapp::Variant v{ std::unordered_map<std::string, int>() };
	const metapp::MetaMappable * metaMappable = metapp::getNonReferenceMetaType(v)->getMetaMappable();
	const metapp::MetaType * valueType = metaMappable->getValueType(v);
	ASSERT(valueType->getTypeKind() == metapp::tkStdPair);
	ASSERT(valueType->getUpType(0)->equal(metapp::getMetaType<std::string>()));
	ASSERT(valueType->getUpType(1)->equal(metapp::getMetaType<int>()));
	//code
}

/*desc
#### get

```c++
Variant get(const Variant & mappable, const Variant & key);
```

Returns a reference to the mapped value of the element with `key`.
If no such element exists, an empty Variant (Variant::isEmpty() is true) is returned.  
`key` is casted to the key type in the container.  

#### set

```c++
void set(const Variant & mappable, const Variant & key, const Variant & value);
```

Set the mapped value of the element with `key` with `value`.   
`key` is casted to the key type in the container.  
`value` is casted to the value type in the container.  

## Non-member utility functions

Below free functions are shortcut functions to use the member functions in `MetaMappable`.  
Usually you should prefer the utility functions to calling `MetaMappable` member function directly.
However, if you need to call functions on a single `MetaMappable` more than one times in a high performance application,
you may store `mappable.getMetaType()->getMetaMappable()` to a local variable, then use the variable to call the member functions.
This is because `getMetaMappable()` has slightly performance overhead (the overhead is neglect most time).

```c++
inline const MetaType * mappableGetValueType(const Variant & mappable)
{
	return mappable.getMetaType()->getMetaMappable()->getValueType(mappable);
}

inline Variant mappableGet(const Variant & mappable, const Variant & key)
{
	return mappable.getMetaType()->getMetaMappable()->get(mappable, key);
}

inline void mappableSet(const Variant & mappable, const Variant & key, const Variant & value)
{
	mappable.getMetaType()->getMetaMappable()->set(mappable, key, value);
}
```

desc*/
