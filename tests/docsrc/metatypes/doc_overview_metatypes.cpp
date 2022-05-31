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
# Built-in meta types overview

metapp supports a large amount of ready to use built-in meta types, from fundamental types such as `int` or `double`
to STL containers such as `std::vector` or `std::map`, etc.  

## How to use the build-in meta types

To use the built-in meta types, the below header must be included.

```c++
#include "metapp/allmetatypes.h"
```

The header includes all built-in meta types. You don't need include separate headers of each meta types.
And even if you only need one meta type, you should still include `allmetatypes.h`, because it can handle pointer, reference,
and other cases.  

## List of meta types

In this section, only a few meta types are detailed documented, since most meta types are straightforward and easy to understand.  
To see the basic information, please see [List of all built-in meta types](list_all.md)

desc*/
