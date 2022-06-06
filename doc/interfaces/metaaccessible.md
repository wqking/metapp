[//]: # (Auto generated file, don't modify this file.)

# MetaAccessible interface
<!--begintoc-->
- [Overview](#mdtoc_e7c3d1bb)
- [Header](#mdtoc_6e72a8c1)
- [Get MetaAccessible interface](#mdtoc_e35d724c)
- [Implemented built-in meta types](#mdtoc_ed7f0e2e)
- [MetaAccessible constructor](#mdtoc_ee0c31d2)
- [MetaAccessible member functions](#mdtoc_3a9e571a)
  - [getValueType](#mdtoc_8d778ce1)
  - [isReadOnly](#mdtoc_5e2aac4d)
  - [getClassType](#mdtoc_a80b7ef1)
  - [get](#mdtoc_fd3b2e70)
  - [set](#mdtoc_e61425dc)
  - [isStatic](#mdtoc_460427c9)
- [Non-member utility functions](#mdtoc_e4e47ded)
<!--endtoc-->

<a id="mdtoc_e7c3d1bb"></a>
## Overview

`MetaAccessible` is a meta interface to get and set value.  

<a id="mdtoc_6e72a8c1"></a>
## Header

```c++
#include "metapp/interfaces/metaaccessible.h"
```

<a id="mdtoc_e35d724c"></a>
## Get MetaAccessible interface

We can call `MetaType::getMetaAccessible()` to get the `MetaAccessible` interface. If the type doesn't implement the interface,
`nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<std::vector<int> >();
const metapp::MetaAccessible * metaAccessible = metaType->getMetaAccessible();
```

<a id="mdtoc_ed7f0e2e"></a>
## Implemented built-in meta types

Pointer, T * (tkPointer)  
Member data pointer, T C::* (tkMemberPointer)  
Accessor (tkAccessor)  

<a id="mdtoc_ee0c31d2"></a>
## MetaAccessible constructor

```c++
MetaAccessible(
  const MetaType * (*getValueType)(const Variant & accessible),
  bool (*isReadOnly)(const Variant & accessible),
  const MetaType * (*getClassType)(const Variant & accessible),
  Variant (*get)(const Variant & accessible, const Variant & instance),
  void (*set)(const Variant & accessible, const Variant & instance, const Variant & value)
);
```

All arguments are function pointers. All pointers must point to valid function.  
The meaning of each functions are same as the member functions listed below.

<a id="mdtoc_3a9e571a"></a>
## MetaAccessible member functions

The first parameter in all of the member functions is `const Variant & accessible`.
It's the Variant which meta type implements `MetaAccessible`, and hold the proper data such as `std::vector`.
The member functions operate on the data.  
We can treat `accessible` as the C++ object instance which class implements an interface called `MetaAccessible`.  
Variant `accessible` can be value that implements `MetaAccessible`, or reference that refers to value that implements `MetaAccessible`.  

<a id="mdtoc_8d778ce1"></a>
#### getValueType

```c++
const MetaType * getValueType(const Variant & accessible);
```

Returns the meta type of the value.  
For Accessor, returns the meta type of `Accessor::ValueType`.  

<a id="mdtoc_5e2aac4d"></a>
#### isReadOnly

```c++
bool isReadOnly(const Variant & accessible);
```

Returns `true` if the value is read only.  
For pointer `T *` and member data pointer `T C::*`, returns `true` if T is const.  
For Accessor, returns `Accessor::isReadOnly()`.  

<a id="mdtoc_a80b7ef1"></a>
#### getClassType

```c++
const MetaType * getClassType(const Variant & accessible);
```

Returns the meta type of the class that the accessible belongs to, or to say, the class declares the accessible. 
If the function returns meta type of `void` (MetaType::isVoid() is true), the accessible doesn't belong to any class,
or the accessible is a static member. When getting/setting value in the accessible, the `instance` can be nullptr.  
If the function returns non-void meta type, the accessible belongs to the class of the meta type.
When getting/setting value in the accessible, the `instance` must be pointer to a valid object.  

<a id="mdtoc_fd3b2e70"></a>
#### get

```c++
Variant get(const Variant & accessible, const Variant & instance);
```

Returns the value.  
The parameter `instance` is the object instance, if any.  
The returned Variant is a reference to the value.  
For pointer `T *`, `instance` is ignored. The returned Variant is a reference to the value.  
For member data pointer `T C::*`, `instance` is used to access the value in the object, and the returned Variant has the same
constness as the `instance`. For example, if `instance` is a const object, or pointer to const object, the returned Variant
is a referent to const value. The same for `volatile` and `const volatile`.  
For Accessor, `instance` is passed to the accessor. The returned Variant is the value get from the accessor.  
`instance` can be value, reference, pointer, `std::shared_ptr`, `std::unique_ptr`, etc.  

For implementor: we may get the actual pointer from `instance` by using `metapp::getPointer`, get the pointed type
by using `metapp::getPointedType`. See the document for `utility.h` for more details.  

For implementor: the returned Variant should be reference whenever possible to reduce performance and memory cost.

<a id="mdtoc_e61425dc"></a>
#### set

```c++
void set(const Variant & accessible, const Variant & instance, const Variant & value);
```

Set a new value.  

<a id="mdtoc_460427c9"></a>
#### isStatic

```c++
bool isStatic(const Variant & accessible) const;
```

Returns true if the accessible is static or non-member, false if the accessible is class member.  
The function is equivalent to `return getClassType(accessible)->isVoid();`.  

<a id="mdtoc_e4e47ded"></a>
## Non-member utility functions

Below free functions are shortcut functions to use the member functions in `MetaAccessible`.  
Usually you should prefer the utility functions to calling `MetaAccessible` member function directly. However, if you need to call functions on a single `MetaAccessible` more than one times in a high performance application, you may store `accessible.getMetaType()->getMetaAccessible()` to a local variable, then use the variable to call the member functions. This is because `getMetaAccessible()` has slightly performance overhead (the overhead is neglect most time).

```c++
inline const MetaType * accessibleGetValueType(const Variant & accessible)
{
  return accessible.getMetaType()->getMetaAccessible()->getValueType(accessible);
}

inline bool accessibleIsReadOnly(const Variant & accessible)
{
  return accessible.getMetaType()->getMetaAccessible()->isReadOnly(accessible);
}

inline const MetaType * accessibleGetClassType(const Variant & accessible)
{
  return accessible.getMetaType()->getMetaAccessible()->getClassType(accessible);
}

inline Variant accessibleGet(const Variant & accessible, const Variant & instance)
{
  return accessible.getMetaType()->getMetaAccessible()->get(accessible, instance);
}

inline void accessibleSet(const Variant & accessible, const Variant & instance, const Variant & value)
{
  accessible.getMetaType()->getMetaAccessible()->set(accessible, instance, value);
}

inline bool accessibleIsStatic(const Variant & accessible)
{
  return accessible.getMetaType()->getMetaAccessible()->isStatic(accessible);
}
```

