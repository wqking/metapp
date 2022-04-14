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

#include "metapp/variant.h"
#include "metapp/allmetatypes.h"

TEST_CASE("metatypes, std::set<int>")
{
	using Type = std::set<int>;
	Type container { 3, 2, 5 };
	metapp::Variant v(container);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdSet);
	REQUIRE(*v.get<Type &>().find(3) == 3);
	v.get<Type &>().insert(6);
	REQUIRE(*v.get<Type &>().find(6) == 6);
	REQUIRE(v.getMetaType()->getUpTypeCount() == 1);
	REQUIRE(v.getMetaType()->getUpType()->getTypeKind() == metapp::tkInt);
}

TEST_CASE("metatypes, std::multiset<int>")
{
	using Type = std::multiset<int>;
	Type container { 3, 2, 5 };
	metapp::Variant v(container);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdMultiset);
	REQUIRE(*v.get<Type &>().find(3) == 3);
	v.get<Type &>().insert(6);
	REQUIRE(*v.get<Type &>().find(6) == 6);
	REQUIRE(v.getMetaType()->getUpTypeCount() == 1);
	REQUIRE(v.getMetaType()->getUpType()->getTypeKind() == metapp::tkInt);
}

