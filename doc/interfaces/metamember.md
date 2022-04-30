[//]: # (Auto generated file, don't modify this file.)

# MetaMember interface
<!--begintoc-->
* [Overview](#a2_1)
* [Header](#a2_2)
* [Get MetaMember interface](#a2_3)
* [Implemented built-in meta types](#a2_4)
* [MetaMember constructor](#a2_5)
* [MetaMember member functions](#a2_6)
  * [getClassType](#a4_1)
* [Non-member utility functions](#a2_7)
<!--endtoc-->

<a id="a2_1"></a>
## Overview

`MetaMember` is a meta interface to get the class type the meta type belongs to.  

<a id="a2_2"></a>
## Header

```c++
#include "metapp/interfaces/metamember.h"
```

<a id="a2_3"></a>
## Get MetaMember interface

We can call `MetaType::getMetaMember()` to get the `MetaMember` interface. If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<std::vector<int> >();
const metapp::MetaMember * metaMember = metaType->getMetaMember();
```

<a id="a2_4"></a>
## Implemented built-in meta types

Constructor (tkConstructor)  
Member data pointer, T C::* (tkMemberPointer)  
Member function pointer, T (C::*)(arguments...) (tkMemberPointer)  

<a id="a2_5"></a>
## MetaMember constructor

```c++
explicit MetaMember(
  const MetaType * (*getClassType)(const Variant & var)
);
```

Parameter `getClassType` is a function pointer. It must point to valid function.  
The meaning of `getClassType` is same as the member function listed below.

<a id="a2_6"></a>
## MetaMember member functions

<a id="a4_1"></a>
#### getClassType

```c++
const MetaType * getClassType(const Variant & var);
```
Returns the meta type of the class that the value in `var` belongs.  
For constructor, returns meta type of the class the constructor in.  
For member data pointer T C::*, returns the meta type of C.   
For member function pointer T (C::*)(arguments...), returns the meta type of C.   

The parameter `var` is the Variant which meta type implements `MetaMember`, and hold the proper data such as a constructor. The member functions operate on the data.  
We can treat `var` as the C++ object instance which class implements an interface called `MetaMember`.  
`var` can be a value, a reference, or a pointer.  

<a id="a2_7"></a>
## Non-member utility functions

Below free functions are shortcut functions to use the member functions in `MetaMember`.  
Usually you should prefer the utility functions to calling `MetaMember` member function directly. However, if you need to call functions on a single `MetaMember` more than one times in a high performance application, you may store `var.getMetaType()->getMetaMember()` to a local variable, then use the variable to call the member functions. This is because `getMetaMember()` has slightly performance overhead (the overhead is neglect most time).

```c++
inline const MetaType * memberGetClassType(const Variant & var)
{
  return var.getMetaType()->getMetaMember()->getClassType(var);
}
```
