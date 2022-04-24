# MetaMappable interface

## Overview

`MetaMappable` is a meta interface to get and set elements in associative containers.  

## Header

```c++
#include "metapp/interfaces/metamappable.h"
```

## Get MetaMappable interface

We can call `MetaType::getMetaMappable()` to get the `MetaMappable` interface. If the type doesn't implement the interface, `nullptr` is returned.

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
	std::pair<const MetaType *, const MetaType *> (*getValueType)(const Variant & var),
	Variant (*get)(const Variant & var, const Variant & key),
	void (*set)(const Variant & var, const Variant & key, const Variant & value)
);
```

All arguments are function pointers. All pointers must point to valid function.  
The meaning of each functions are same as the member functions listed below.  

## MetaMappable member functions

The first parameter in all of the member functions is `const Variant & var`. It's the Variant which meta type implements `MetaMappable`, and hold the proper data such as `std::vector`. The member functions operate on the data.  
We can treat `var` as the C++ object instance which class implements an interface called `MetaMappable`.  
`var` can be a value, a reference, or a pointer.  

#### getValueType

```c++
std::pair<const MetaType *, const MetaType *> getValueType(const Variant & var);
```

Returns the meta type of key/value. The `first` in the returned `std::pair` is the meta type for the key, the `second` in the returned `std::pair` is the meta type for the value.  

#### get

```c++
Variant get(const Variant & var, const Variant & key);
```

Returns a reference to the mapped value of the element with `key`. If no such element exists, an empty Variant (Variant::isEmpty() is true) is returned.  
`key` is casted to the key type in the container.  

#### set

```c++
void set(const Variant & var, const Variant & key, const Variant & value);
```

Set the mapped value of the element with `key` with `value`.   
`key` is casted to the key type in the container.  
`value` is casted to the value type in the container.  

## Non-member utility functions

Below free functions are shortcut functions to use the member functions in `MetaMappable`.  
Usually you should prefer the utility functions to calling `MetaMappable` member function directly. However, if you need to call functions on a single `MetaMappable` more than one times in a high performance application, you may store `var.getMetaType()->getMetaMappable()` to a local variable, then use the variable to call the member functions. This is because `getMetaMappable()` has slightly performance overhead (the overhead is neglect most time).

```c++
inline std::pair<const MetaType *, const MetaType *> mappableGetValueType(const Variant & var)
{
	return var.getMetaType()->getMetaMappable()->getValueType(var);
}

inline Variant mappableGet(const Variant & var, const Variant & key)
{
	return var.getMetaType()->getMetaMappable()->get(var, key);
}

inline void mappableSet(const Variant & var, const Variant & key, const Variant & value)
{
	var.getMetaType()->getMetaMappable()->set(var, key, value);
}
```
