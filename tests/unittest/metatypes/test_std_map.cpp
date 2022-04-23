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

TEST_CASE("metatypes, std::map<int, std::string>")
{
	using Type = std::map<int, std::string>;
	Type container { { 1, "perfect" } };
	metapp::Variant v(container);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdMap);
	REQUIRE(v.get<Type &>()[1] == "perfect");
	v.get<Type &>().insert(std::make_pair(2, "good"));
	REQUIRE(v.get<Type &>()[2] == "good");
	REQUIRE(v.getMetaType()->getUpTypeCount() == 2);
	REQUIRE(v.getMetaType()->getUpType(0)->getTypeKind() == metapp::tkInt);
	REQUIRE(v.getMetaType()->getUpType(1)->getTypeKind() == metapp::tkStdString);
}

TEST_CASE("metatypes, std::multimap<int, std::string>")
{
	using Type = std::multimap<int, std::string>;
	Type container { { 1, "perfect" } };
	metapp::Variant v(container);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdMultimap);
	REQUIRE(v.get<Type &>().find(1)->second == "perfect");
	v.get<Type &>().insert(std::make_pair(2, "good"));
	REQUIRE(v.get<Type &>().find(2)->second == "good");
	REQUIRE(v.getMetaType()->getUpTypeCount() == 2);
	REQUIRE(v.getMetaType()->getUpType(0)->getTypeKind() == metapp::tkInt);
	REQUIRE(v.getMetaType()->getUpType(1)->getTypeKind() == metapp::tkStdString);
}

TEST_CASE("metatypes, std::map<int, std::string>, MetaMappable")
{
	using Type = std::map<int, std::string>;
	metapp::Variant v(Type { { 1, "perfect" } });
	auto metaType = v.getMetaType();
	auto metaMap = metaType->getMetaMappable();
	REQUIRE(metaMap != nullptr);
	REQUIRE(metaMap->get(v, 1).get<std::string &>() == "perfect");
}
