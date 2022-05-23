[//]: # (Auto generated file, don't modify this file.)

# MetaEnum interface
<!--begintoc-->
* [Overview](#a2_1)
* [Header](#a2_2)
* [Get MetaEnum interface](#a2_3)
* [Implemented built-in meta types](#a2_4)
* [MetaEnum constructor](#a2_5)
* [MetaEnum member functions for registering meta data](#a2_6)
  * [registerValue](#a4_1)
* [MetaEnum member functions for retrieving meta data](#a2_7)
  * [getValue](#a4_2)
  * [getValueView](#a4_3)
* [Non-member utility functions](#a2_8)
<!--endtoc-->

<a id="a2_1"></a>
## Overview

`MetaEnum` is a meta interface to obtain C++ `enum` name and values.  

<a id="a2_2"></a>
## Header

```c++
#include "metapp/interfaces/metaenum.h"
```

<a id="a2_3"></a>
## Get MetaEnum interface

We can call `MetaType::getMetaEnum()` to get the MetaEnum interface. If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<MyEnum>();
const metapp::MetaEnum * metaEnum = metaType->getMetaEnum();
```

<a id="a2_4"></a>
## Implemented built-in meta types

None

<a id="a2_5"></a>
## MetaEnum constructor

```c++
template <typename FT>
explicit MetaEnum(FT callback);
```

`callback` is a callback function. MetaEnum invokes the callback in the constructor. Since the interface is usually implemented as static variable inside static function, the "callback in constructor" mechanism can guarantee thread safety.  

`callback` prototype,  
```c++
void callback(metapp::MetaEnum & me);
```
The MetaEnum instance under constructing is passed as the parameter. The callback should register all meta data to `me`.

<a id="a2_6"></a>
## MetaEnum member functions for registering meta data

<a id="a4_1"></a>
#### registerValue

```c++
MetaItem & registerValue(const std::string & name, const Variant & value);
```

Register a enum `value` under `name`. `value` can be any type, not limited to enum value.  
The returned `MetaItem` can be used to add annotations to the meta data.  


<a id="a2_7"></a>
## MetaEnum member functions for retrieving meta data

<a id="a4_2"></a>
#### getValue

```c++
const MetaItem & getValue(const std::string & name) const;
```

Get a value of `name`. If the name is not registered, an empty MetaItem is returned (MetaItem::isEmpty() is true).  

<a id="a4_3"></a>
#### getValueView

```c++
MetaItemView getValueView() const;
```

Returns a MetaItemView for all registered names and values.  

<a id="a2_8"></a>
## Non-member utility functions

Below free functions are shortcut functions to use the member functions in `MetaItem`.  

```c++
inline const MetaItem & enumGetValue(const Variant & var, const std::string & name)
{
  return var.getMetaType()->getMetaEnum()->getValue(name);
}

inline MetaItemView enumGetValueView(const Variant & var)
{
  return var.getMetaType()->getMetaEnum()->getValueView();
}
```
