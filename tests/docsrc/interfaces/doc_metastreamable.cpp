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
# MetaStreamable interface

## Overview

`MetaStreamable` is a meta interface to support input/output streaming operators `<<` and `>>`.  
A `Variant` that meta type implementing `MetaStreamable` can be used in I/O streaming operators.  

## Implement MetaStreamable

Usually you don't need to construct `MetaStreamable` directly. You should inherit the declared meta type from `MetaStreamableBase`.  
For example, assume `MyClass` supports I/O streaming, then we can `DeclareMetaType` MyClass as below,  

```c++
// the header for MetaStreamableBase
#include "metapp/interfaces/bases/metastreamablebase.h"

template <>
struct metapp::DeclareMetaType <MyClass> : metapp::MetaStreamableBase <MyClass>
{
};
```

desc*/
