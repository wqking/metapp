# MetaEnum interface

## Overview

`MetaEnum` is a meta interface to obtain C++ `enum` name and values.  

## Header

```c++
#include "metapp/interfaces/metaenum.h"
```

## Get MetaEnum interface

We can call `MetaType::getMetaEnum()` to get the MetaEnum interface. If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<MyEnum>();
const metapp::MetaEnum * metaEnum = metaType->getMetaEnum();
```

## Implemented built-in meta types

None

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

## MetaEnum member types

#### ValueType
```
using ValueType = long long;
```

Represents the `enum` value type.

## MetaEnum member functions for registration meta data

#### registerValue

```c++
template <typename T>
RegisteredEnumValue & registerValue(const std::string & name, const T value);
```

Register a enum `value` under `name`. `value` can be any type which can be casted to `ValueType`.  
The returned `RegisteredEnumValue` can be used to add annotations to the meta data.  


## MetaEnum member functions for retrieving meta data

#### getValue

```c++
const RegisteredEnumValue & getValue(const std::string & name) const;
```

Get a value of `name`. If the name is not registered, an empty RegisteredEnumValue is returned (RegisteredEnumValue::isEmpty() is true).  

#### getNameList

```c++
std::vector<std::string> getNameList() const;
```

Returns all registered names.  

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
