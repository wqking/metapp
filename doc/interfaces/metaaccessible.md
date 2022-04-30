# MetaAccessible interface
<!--begintoc-->
* [Overview](#a2_1)
* [Header](#a2_2)
* [Get MetaAccessible interface](#a2_3)
* [Implemented built-in meta types](#a2_4)
* [MetaAccessible constructor](#a2_5)
* [MetaIterable member functions](#a2_6)
  * [getValueType](#a4_1)
  * [isReadOnly](#a4_2)
  * [get](#a4_3)
  * [set](#a4_4)
* [Non-member utility functions](#a2_7)
<!--endtoc-->

<a id="a2_1"></a>
## Overview

`MetaAccessible` is a meta interface to get and set value.  

<a id="a2_2"></a>
## Header

```c++
#include "metapp/interfaces/metaaccessible.h"
```

<a id="a2_3"></a>
## Get MetaAccessible interface

We can call `MetaType::getMetaAccessible()` to get the `MetaAccessible` interface. If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<std::vector<int> >();
const metapp::MetaAccessible * metaAccessible = metaType->getMetaAccessible();
```

<a id="a2_4"></a>
## Implemented built-in meta types

Pointer, T * (tkPointer)  
Member data pointer, T C::* (tkMemberPointer)  
Accessor (tkAccessor)  

<a id="a2_5"></a>
## MetaAccessible constructor

```c++
MetaAccessible(
  const MetaType * (*getValueType)(const Variant & var),
  bool (*isReadOnly)(const Variant & var),
  Variant (*get)(const Variant & var, const void * instance),
  void (*set)(const Variant & var, void * instance, const Variant & value)
);
```

All arguments are function pointers. All pointers must point to valid function.  
The meaning of each functions are same as the member functions listed below.

<a id="a2_6"></a>
## MetaIterable member functions

The first parameter in all of the member functions is `const Variant & var`. It's the Variant which meta type implements `MetaIndexable`, and hold the proper data such as `std::vector`. The member functions operate on the data.  
We can treat `var` as the C++ object instance which class implements an interface called `MetaIndexable`.  
`var` can be a value, a reference, or a pointer.  

<a id="a4_1"></a>
#### getValueType

```c++
const MetaType * getValueType(const Variant & var);
```

Returns the meta type of the value.  
For pointer `T *` and member data pointer `T C::*`, returns the meta type of T.  
For Accessor, returns the meta type of `Accessor::ValueType`.  

<a id="a4_2"></a>
#### isReadOnly

```c++
bool isReadOnly(const Variant & var);
```

Returns `true` if the value is read only.  
For pointer `T *` and member data pointer `T C::*`, returns `true` if T is const.  
For Accessor, returns `Accessor::isReadOnly()`.  

<a id="a4_3"></a>
#### get

```c++
Variant get(const Variant & var, const void * instance);
```

Returns the value.  
The parameter `instance` is the object instance, if any.  
For pointer `T *`, `instance` is ignored. The returned Variant is a reference to the value.  
For member data pointer `T C::*`, `instance` is used to access the value in the object. The returned Variant is a reference to the value.  
For Accessor, `instance` is passed to the accessor. The returned Variant is the value get from the accessor.  

<a id="a4_4"></a>
#### set

```c++
void set(const Variant & var, void * instance, const Variant & value);
```

Set a new value.  

<a id="a2_7"></a>
## Non-member utility functions

Below free functions are shortcut functions to use the member functions in `MetaAccessible`.  
Usually you should prefer the utility functions to calling `MetaAccessible` member function directly. However, if you need to call functions on a single `MetaAccessible` more than one times in a high performance application, you may store `var.getMetaType()->getMetaAccessible()` to a local variable, then use the variable to call the member functions. This is because `getMetaAccessible()` has slightly performance overhead (the overhead is neglect most time).

```c++
inline const MetaType * accessibleGetValueType(const Variant & var)
{
  return var.getMetaType()->getMetaAccessible()->getValueType(var);
}

inline bool accessibleIsReadOnly(const Variant & var)
{
  return var.getMetaType()->getMetaAccessible()->isReadOnly(var);
}

inline Variant accessibleGet(const Variant & var, const void * instance)
{
  return var.getMetaType()->getMetaAccessible()->get(var, instance);
}

inline void accessibleSet(const Variant & var, void * instance, const Variant & value)
{
  var.getMetaType()->getMetaAccessible()->set(var, instance, value);
}
```
