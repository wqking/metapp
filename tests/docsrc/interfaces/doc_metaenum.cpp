// metapp library
// 
// Copyright (C) 2022 Wang Qi (wqking)
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "testutil.h"

#include "metapp/allmetatypes.h"

/*desc
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

## MetaEnum member functions for registering meta data

#### registerValue

```c++
template <typename T>
RegisteredItem & registerValue(const std::string & name, const T value);
```

Register a enum `value` under `name`. `value` can be any type which can be casted to `ValueType`.  
The returned `RegisteredItem` can be used to add annotations to the meta data.  


## MetaEnum member functions for retrieving meta data

#### getValue

```c++
const RegisteredItem & getValue(const std::string & name) const;
```

Get a value of `name`. If the name is not registered, an empty RegisteredItem is returned (RegisteredItem::isEmpty() is true).  

#### getNameList

```c++
std::vector<std::string> getNameList() const;
```

Returns all registered names.  

## Non-member utility functions

Below free functions are shortcut functions to use the member functions in `RegisteredItem`.  

```c++
inline const RegisteredItem & enumGetValue(const Variant & var, const std::string & name)
{
	return var.getMetaType()->getMetaEnum()->getValue(name);
}

inline std::vector<std::string> enumGetNameList(const Variant & var)
{
	return var.getMetaType()->getMetaEnum()->getNameList();
}
```

desc*/
