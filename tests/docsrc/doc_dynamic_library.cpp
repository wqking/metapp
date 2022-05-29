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
# Use metapp in dynamic library

## Overview

We can use metapp in dynamic library. That's to say, we can expose meta data reflected in one dynamic library,
then use the meta data in the main program, or in another dynamic library.  

metapp is ready for use in dynamic library. The only thing we need to take care of is, we must give each meta type
a unique `typeKind`. When comparing meta types in different modules (different dynamic libraries or program),
metapp use `typeKind` to identify the meta type, so the `typeKind` must be unique.  
When we use metapp within a single module (a single program or a signle dynamic library, no crossing dynamic libraries),
metapp compares meta types using a unique pointer, so whether `typeKind` is unique doesn't matter.  

To assign each meta type a unique `typeKind`, we need to [DeclareMetaType](declaremetatype.md)

desc*/
