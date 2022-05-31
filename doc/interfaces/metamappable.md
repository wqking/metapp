[//]: # (Auto generated file, don't modify this file.)

# MetaMappable interface
<!--begintoc-->
* [Overview](#a2_1)
* [Header](#a2_2)
* [Get MetaMappable interface](#a2_3)
* [Implemented built-in meta types](#a2_4)
* [MetaMappable constructor](#a2_5)
* [MetaMappable member functions](#a2_6)
  * [getValueType](#a4_1)
  * [get](#a4_2)
  * [set](#a4_3)
* [Non-member utility functions](#a2_7)
<!--endtoc-->

<a id="a2_1"></a>
## Overview

`MetaMappable` is a meta interface to get and set elements in associative containers.  

<a id="a2_2"></a>
## Header

```c++
#include "metapp/interfaces/metamappable.h"
```

<a id="a2_3"></a>
## Get MetaMappable interface

We can call `MetaType::getMetaMappable()` to get the `MetaMappable` interface.
If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<std::vector<int> >();
const metapp::MetaMappable * metaMappable = metaType->getMetaMappable();
```

<a id="a2_4"></a>
## Implemented built-in meta types

`std::map` (tkStdMap)  
`std::multimap` (tkStdMultimap)  
`std::unordered_map` (tkStdUnorderedMap)  
`std::unordered_multimap` (tkStdUnorderedMultimap)  

<a id="a2_5"></a>
## MetaMappable constructor

```c++
MetaMappable(
  std::pair<const MetaType *, const MetaType *> (*getValueType)(const Variant & mappable),
  Variant (*get)(const Variant & mappable, const Variant & key),
  void (*set)(const Variant & mappable, const Variant & key, const Variant & value)
);
```

All arguments are function pointers. All pointers must point to valid function.  
The meaning of each functions are same as the member functions listed below.  

<a id="a2_6"></a>
## MetaMappable member functions

The first parameter in all of the member functions is `const Variant & mappable`.
It's the Variant which meta type implements `MetaMappable`, and hold the proper data such as `std::vector`.
The member functions operate on the data.  
We can treat `mappable` as the C++ object instance which class implements an interface called `MetaMappable`.  
Variant `mappable` can be value that implements `MetaMappable`, or reference that refers to value that implements `MetaMappable`.  

<a id="a4_1"></a>
#### getValueType

```c++
std::pair<const MetaType *, const MetaType *> getValueType(const Variant & mappable);
```

Returns the meta type of key/value. The `first` in the returned `std::pair` is the meta type for the key,
the `second` in the returned `std::pair` is the meta type for the value.  

<a id="a4_2"></a>
#### get

```c++
Variant get(const Variant & mappable, const Variant & key);
```

Returns a reference to the mapped value of the element with `key`.
If no such element exists, an empty Variant (Variant::isEmpty() is true) is returned.  
`key` is casted to the key type in the container.  

<a id="a4_3"></a>
#### set

```c++
void set(const Variant & mappable, const Variant & key, const Variant & value);
```

Set the mapped value of the element with `key` with `value`.   
`key` is casted to the key type in the container.  
`value` is casted to the value type in the container.  

<a id="a2_7"></a>
## Non-member utility functions

Below free functions are shortcut functions to use the member functions in `MetaMappable`.  
Usually you should prefer the utility functions to calling `MetaMappable` member function directly.
However, if you need to call functions on a single `MetaMappable` more than one times in a high performance application,
you may store `mappable.getMetaType()->getMetaMappable()` to a local variable, then use the variable to call the member functions.
This is because `getMetaMappable()` has slightly performance overhead (the overhead is neglect most time).

```c++
inline std::pair<const MetaType *, const MetaType *> mappableGetValueType(const Variant & mappable)
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
