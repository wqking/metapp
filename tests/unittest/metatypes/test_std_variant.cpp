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

#include "../test.h"

#include "metapp/compiler.h"

#ifdef METAPP_SUPPORT_STANDARD_17

#include "metapp/variant.h"
#include "metapp/allmetatypes.h"

TEST_CASE("metatypes, std::variant<std::string, int, long *>")
{
	using Type = std::variant<std::string, int, long *>;
	long n = 5;
	Type var("def");
	metapp::Variant v(var);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdVariant);
	REQUIRE(std::get<std::string>(v.get<Type &>()) == "def");
	v.get<Type &>() = 38;
	REQUIRE(std::get<int>(v.get<Type &>()) == 38);
	v.get<Type &>() = &n;
	REQUIRE(std::get<long *>(v.get<Type &>()) == &n);
	auto metaType = v.getMetaType();
	REQUIRE(metaType->getUpType(0)->getTypeKind() == metapp::tkStdString);
	REQUIRE(metaType->getUpType(1)->getTypeKind() == metapp::tkInt);
	REQUIRE(metaType->getUpType(2)->getTypeKind() == metapp::tkPointer);
}

#endif
