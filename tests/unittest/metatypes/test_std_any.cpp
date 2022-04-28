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

#include "metapp/compiler.h"

#ifdef METAPP_SUPPORT_STANDARD_17

#include "metapp/variant.h"
#include "metapp/allmetatypes.h"

TEST_CASE("metatypes, tkStdAny, std::any")
{
	std::any var;
	var = std::string("perfect");
	metapp::Variant v(var);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdAny);
	REQUIRE(std::any_cast<std::string &>(v.get<std::any &>()) == "perfect");

	var = 5;
	v = var;
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdAny);
	REQUIRE(std::any_cast<int>(v.get<std::any &>()) == 5);
}

#endif
