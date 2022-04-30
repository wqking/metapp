# MetaEnum interface
<!--begintoc-->
* [Overview](#a2_1)
* [Header](#a2_2)
* [Get MetaEnum interface](#a2_3)
* [Implemented built-in meta types](#a2_4)
* [MetaEnum constructor](#a2_5)
* [MetaEnum member types](#a2_6)
  * [ValueType](#a4_1)
* [MetaEnum member functions for registering meta data](#a2_7)
  * [registerValue](#a4_2)
* [MetaEnum member functions for retrieving meta data](#a2_8)
  * [getValue](#a4_3)
  * [getNameList](#a4_4)
* [Non-member utility functions](#a2_9)
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
## MetaEnum member types

<a id="a4_1"></a>
#### ValueType
```
using ValueType = long long;
```

Represents the `enum` value type.

<a id="a2_7"></a>
## MetaEnum member functions for registering meta data

<a id="a4_2"></a>
#### registerValue

```c++
template <typename T>
RegisteredEnumValue & registerValue(const std::string & name, const T value);
```

Register a enum `value` under `name`. `value` can be any type which can be casted to `ValueType`.  
The returned `RegisteredEnumValue` can be used to add annotations to the meta data.  


<a id="a2_8"></a>
## MetaEnum member functions for retrieving meta data

<a id="a4_3"></a>
#### getValue

```c++
const RegisteredEnumValue & getValue(const std::string & name) const;
```

Get a value of `name`. If the name is not registered, an empty RegisteredEnumValue is returned (RegisteredEnumValue::isEmpty() is true).  

<a id="a4_4"></a>
#### getNameList

```c++
std::vector<std::string> getNameList() const;
```

Returns all registered names.  

<a id="a2_9"></a>
## Non-member utility functions

Below free functions are shortcut functions to use the member functions in `RegisteredEnumValue`.  

```c++
inline const RegisteredEnumValue & enumGetValue(const Variant & var, const std::string & name)
{
  return var.getMetaType()->getMetaEnum()->getValue(name);
}

inline std::vector<std::string> enumGetNameList(const Variant & var)
{
  return var.getMetaType()->getMetaEnum()->getNameList();
}
```
