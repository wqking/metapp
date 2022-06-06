[//]: # (Auto generated file, don't modify this file.)

# MetaEnum interface
<!--begintoc-->
- [Overview](#mdtoc_e7c3d1bb)
- [Header](#mdtoc_6e72a8c1)
- [Get MetaEnum interface](#mdtoc_42ccb38f)
- [Implemented built-in meta types](#mdtoc_ed7f0e2e)
- [MetaEnum constructor](#mdtoc_348cb714)
- [MetaEnum member functions for registering meta data](#mdtoc_31633ac1)
  - [registerValue](#mdtoc_9435388f)
- [MetaEnum member functions for retrieving meta data](#mdtoc_9b450e78)
  - [getValue](#mdtoc_2d64f577)
  - [getValueView](#mdtoc_ff547046)
- [Non-member utility functions](#mdtoc_e4e47ded)
<!--endtoc-->

<a id="mdtoc_e7c3d1bb"></a>
## Overview

`MetaEnum` is a meta interface to obtain C++ `enum` name and values.  

<a id="mdtoc_6e72a8c1"></a>
## Header

```c++
#include "metapp/interfaces/metaenum.h"
```

<a id="mdtoc_42ccb38f"></a>
## Get MetaEnum interface

We can call `MetaType::getMetaEnum()` to get the MetaEnum interface. If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<MyEnum>();
const metapp::MetaEnum * metaEnum = metaType->getMetaEnum();
```

<a id="mdtoc_ed7f0e2e"></a>
## Implemented built-in meta types

None

<a id="mdtoc_348cb714"></a>
## MetaEnum constructor

```c++
template <typename FT>
explicit MetaEnum(FT callback);
```

`callback` is a callback function. MetaEnum invokes the callback in the constructor.
Since the interface is usually implemented as static variable inside static function, the "callback in constructor" mechanism
can guarantee thread safety.  

`callback` prototype,  
```c++
void callback(metapp::MetaEnum & me);
```
The MetaEnum instance under constructing is passed as the parameter. The callback should register all meta data to `me`.

<a id="mdtoc_31633ac1"></a>
## MetaEnum member functions for registering meta data

<a id="mdtoc_9435388f"></a>
#### registerValue

```c++
MetaItem & registerValue(const std::string & name, const Variant & value);
```

Register a enum `value` under `name`. `value` can be any type, not limited to enum value.  
The returned `MetaItem` can be used to add annotations to the meta data.  


<a id="mdtoc_9b450e78"></a>
## MetaEnum member functions for retrieving meta data

<a id="mdtoc_2d64f577"></a>
#### getValue

```c++
const MetaItem & getValue(const std::string & name) const;
```

Get a value of `name`. If the name is not registered, an empty MetaItem is returned (MetaItem::isEmpty() is true).  

<a id="mdtoc_ff547046"></a>
#### getValueView

```c++
MetaItemView getValueView() const;
```

Returns a MetaItemView for all registered names and values.  

<a id="mdtoc_e4e47ded"></a>
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

