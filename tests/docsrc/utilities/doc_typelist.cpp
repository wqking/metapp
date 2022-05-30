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
# TypeList reference

## Overview

`metapp::TypeList` is a template that holds a list of types.  
For any end user, you only need to know how to construct a `TypeList`.  

## Header
desc*/

//code
#include "metapp/utilities/typelist.h"
//code

//desc ## Examples
ExampleFunc
{
	//code
	//desc TL1 is a type list of `int` and `char`.
	using TL1 = metapp::TypeList<int, char>;

	//desc TL2 is a type list of `std::string`, `std::vector<int>`, `bool`, `long`, and `double`.
	using TL2 = metapp::TypeList<std::string, std::vector<int>, bool, long, double>;
	//code
}

/*desc
## Use cases

`TypeList` is used when declaring meta type. The `UpType` can be a `TypeList`, and implementing `cast` and `castFrom`
can use `TypeList`.  
[See document of DeclareMetaType for more details](../declaremetatype.md)
desc*/