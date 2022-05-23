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

## MetaEnum member functions for registering meta data

#### registerValue

```c++
MetaItem & registerValue(const std::string & name, const Variant & value);
```

Register a enum `value` under `name`. `value` can be any type, not limited to enum value.  
The returned `MetaItem` can be used to add annotations to the meta data.  


## MetaEnum member functions for retrieving meta data

#### getValue

```c++
const MetaItem & getValue(const std::string & name) const;
```

Get a value of `name`. If the name is not registered, an empty MetaItem is returned (MetaItem::isEmpty() is true).  

#### getValueView

```c++
MetaItemView getValueView() const;
```

Returns a MetaItemView for all registered names and values.  

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

desc*/
