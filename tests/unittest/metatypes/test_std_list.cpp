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
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, std::list<std::string>")
{
	using Type = std::list<std::string>;
	Type list{ "hello", "world" };
	metapp::Variant v(list);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdList);
	REQUIRE(v.get<const Type &>().front() == "hello");
	REQUIRE(v.get<const Type &>().back() == "world");
	v.get<Type &>().push_back("good");
	REQUIRE(v.get<const Type &>().back() == "good");
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkStdList, tkStdString }));
}

