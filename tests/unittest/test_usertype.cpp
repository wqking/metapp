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

#include "test.h"

#include "metapp/variant.h"
#include "metapp/allmetatypes.h"

#include <string>
#include <iostream>
#include <climits>

struct UseTypeMyClass
{
	int value;
};

template <>
struct metapp::DeclareMetaType <UseTypeMyClass> : metapp::DeclareMetaTypeBase <UseTypeMyClass>
{
	static constexpr metapp::TypeKind typeKind = 2000;

};

template <>
struct metapp::DeclareMetaType <UseTypeMyClass *> : metapp::DeclareMetaTypeBase <UseTypeMyClass *>
{
	static constexpr metapp::TypeKind typeKind = 2001;
};

TEST_CASE("user type")
{
	UseTypeMyClass obj{ 38 };
	
	metapp::Variant v(obj);
	REQUIRE(metapp::getTypeKind(v) == 2000);
	REQUIRE(v.get<UseTypeMyClass>().value == 38);
	REQUIRE(v.get<UseTypeMyClass &>().value == 38);
	REQUIRE(v.get<const UseTypeMyClass &>().value == 38);

	metapp::Variant v2(&obj);
	REQUIRE(metapp::getTypeKind(v2) == 2001);
	REQUIRE(v2.get<UseTypeMyClass *>() == &obj);
}


