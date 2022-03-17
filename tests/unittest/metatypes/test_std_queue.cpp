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

TEST_CASE("metatypes, std::queue<std::string>")
{
	using Type = std::queue<std::string>;
	Type container;
	metapp::Variant v(container);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdQueue);
	v.get<Type &>().push("good");
	v.get<Type &>().push("great");
	REQUIRE(v.get<Type &>().front() == "good");
	REQUIRE(v.get<Type &>().back() == "great");
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkStdQueue, tkStdString }));
}

TEST_CASE("metatypes, std::priority_queue<int>")
{
	using Type = std::priority_queue<int>;
	Type container;
	metapp::Variant v(container);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdPriorityQueue);
	v.get<Type &>().push(1);
	v.get<Type &>().push(5);
	REQUIRE(v.get<Type &>().top() == 5);
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkStdPriorityQueue, tkInt }));
}

