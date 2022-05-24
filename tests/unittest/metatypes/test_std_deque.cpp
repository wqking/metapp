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

TEST_CASE("metatypes, std::deque<std::string>")
{
	using Type = std::deque<std::string>;
	Type container { "perfect" };
	metapp::Variant v(container);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdDeque);
	REQUIRE(v.get<Type>()[0] == "perfect");
	v.get<Type &>().push_back("good");
	REQUIRE(v.get<Type &>().back() == "good");
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkStdDeque, tkStdString }));
}

TEST_CASE("metatypes, std::deque<std::string>, MetaIndexable")
{
	metapp::Variant v(std::deque<std::string> {
		"good", "great", "perfect"
	});
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdDeque);
	REQUIRE(v.getMetaType()->getMetaIndexable() != nullptr);
	REQUIRE(v.getMetaType()->getMetaIndexable()->getSizeInfo(v).getSize() == 3);
	REQUIRE(v.getMetaType()->getMetaIndexable()->get(v, 0).get<const std::string &>() == "good");
	REQUIRE(v.getMetaType()->getMetaIndexable()->get(v, 1).get<const std::string &>() == "great");
	REQUIRE(v.getMetaType()->getMetaIndexable()->get(v, 2).get<const std::string &>() == "perfect");
}

TEST_CASE("metatypes, std::deque<std::string>, MetaIterable")
{
	std::deque<std::string> original {
		"good", "great", "perfect"
	};
	metapp::Variant v(original);
	REQUIRE(v.getMetaType()->getMetaIterable() != nullptr);

	std::deque<std::string> newDeque;
	v.getMetaType()->getMetaIterable()->forEach(v, [&newDeque](const metapp::Variant & value) {
		newDeque.push_back(value.get<std::string>());
		return true;
		});
	REQUIRE(original == newDeque);
}

