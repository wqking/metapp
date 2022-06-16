[//]: # (Auto generated file, don't modify this file.)

# MetaMappable interface
<!--begintoc-->
- [Overview](#mdtoc_e7c3d1bb)
- [Header](#mdtoc_6e72a8c1)
- [Get MetaMappable interface](#mdtoc_85ad9838)
- [Implemented built-in meta types](#mdtoc_ed7f0e2e)
- [MetaMappable constructor](#mdtoc_dc6ac553)
- [MetaMappable member functions](#mdtoc_26c6c0e9)
  - [getValueType](#mdtoc_8d778ce1)
  - [get](#mdtoc_fd3b2e70)
  - [set](#mdtoc_e61425dc)
- [Non-member utility functions](#mdtoc_e4e47ded)
<!--endtoc-->

<a id="mdtoc_e7c3d1bb"></a>
## Overview

`MetaMappable` is a meta interface to get and set elements in associative containers.  

<a id="mdtoc_6e72a8c1"></a>
## Header

```c++
#include "metapp/interfaces/metamappable.h"
```

<a id="mdtoc_85ad9838"></a>
## Get MetaMappable interface

We can call `MetaType::getMetaMappable()` to get the `MetaMappable` interface.
If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<std::vector<int> >();
const metapp::MetaMappable * metaMappable = metaType->getMetaMappable();
```

<a id="mdtoc_ed7f0e2e"></a>
## Implemented built-in meta types

`std::map` (tkStdMap)  
`std::multimap` (tkStdMultimap)  
`std::unordered_map` (tkStdUnorderedMap)  
`std::unordered_multimap` (tkStdUnorderedMultimap)  

<a id="mdtoc_dc6ac553"></a>
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

<a id="mdtoc_26c6c0e9"></a>
## MetaMappable member functions

The first parameter in all of the member functions is `const Variant & mappable`.
It's the Variant which meta type implements `MetaMappable`, and hold the proper data such as `std::vector`.
The member functions operate on the data.  
We can treat `mappable` as the C++ object instance which class implements an interface called `MetaMappable`.  
Variant `mappable` can be value that implements `MetaMappable`, or reference that refers to value that implements
`MetaMappable`.  

<a id="mdtoc_8d778ce1"></a>
#### getValueType

```c++
const MetaType * getValueType(const Variant & mappable);
```

Returns the meta type of the value type. The returned meta type is a type of `std::pair`, the up type at 0 is the key type,
the up type at 1 is the value type. Example,  

```c++
metapp::Variant v{ std::unordered_map<std::string, int>() };
const metapp::MetaMappable * metaMappable = metapp::getNonReferenceMetaType(v)->getMetaMappable();
const metapp::MetaType * valueType = metaMappable->getValueType(v);
ASSERT(valueType->getTypeKind() == metapp::tkStdPair);
ASSERT(valueType->getUpType(0)->equal(metapp::getMetaType<std::string>()));
ASSERT(valueType->getUpType(1)->equal(metapp::getMetaType<int>()));
```

<a id="mdtoc_fd3b2e70"></a>
#### get

```c++
Variant get(const Variant & mappable, const Variant & key);
```

Returns a reference to the mapped value of the element with `key`.
If no such element exists, an empty Variant (Variant::isEmpty() is true) is returned.  
`key` is casted to the key type in the container.  

<a id="mdtoc_e61425dc"></a>
#### set

```c++
void set(const Variant & mappable, const Variant & key, const Variant & value);
```

Set the mapped value of the element with `key` with `value`.   
`key` is casted to the key type in the container.  
`value` is casted to the value type in the container.  

<a id="mdtoc_e4e47ded"></a>
## Non-member utility functions

Below free functions are shortcut functions to use the member functions in `MetaMappable`.  
Usually you should prefer the utility functions to calling `MetaMappable` member function directly.
However, if you need to call functions on a single `MetaMappable` more than one times in a high performance
application, you may store `mappable.getMetaType()->getMetaMappable()` to a local variable, then use the variable
to call the member functions.
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

