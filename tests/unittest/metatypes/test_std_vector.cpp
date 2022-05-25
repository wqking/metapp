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

TEST_CASE("metatypes, std::vector<int>")
{
	using Type = std::vector<int>;
	Type vec{ 5 };
	metapp::Variant v(vec);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdVector);
	REQUIRE(v.get<Type>()[0] == 5);
	v.get<Type &>().push_back(10);
	REQUIRE(v.get<Type>().back() == 10);
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkStdVector, tkInt }));
}

TEST_CASE("metatypes, std::vector<std::string>, MetaIndexable")
{
	metapp::Variant v(std::vector<std::string> {
		"good", "great", "perfect"
	});
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdVector);
	REQUIRE(v.getMetaType()->getMetaIndexable() != nullptr);
	REQUIRE(metapp::indexableGetSizeInfo(v).getSize() == 3);
	REQUIRE(metapp::indexableGetSizeInfo(v).isResizable());
	REQUIRE(! metapp::indexableGetSizeInfo(v).isUnknownSize());
	REQUIRE(metapp::indexableGet(v, 0).getMetaType()->getTypeKind() == metapp::tkReference);
	REQUIRE(metapp::indexableGet(v, 0).get<const std::string &>() == "good");
	REQUIRE(metapp::indexableGet(v, 1).get<const std::string &>() == "great");
	REQUIRE(metapp::indexableGet(v, 2).get<const std::string &>() == "perfect");

	metapp::indexableResize(v, 38);
	REQUIRE(metapp::indexableGetSizeInfo(v).getSize() == 38);
	REQUIRE(metapp::indexableGet(v, 0).get<const std::string &>() == "good");
	REQUIRE(metapp::indexableGet(v, 1).get<const std::string &>() == "great");
	REQUIRE(metapp::indexableGet(v, 2).get<const std::string &>() == "perfect");
	REQUIRE(metapp::indexableGet(v, 3).get<const std::string &>().empty());
	REQUIRE(metapp::indexableGet(v, 37).get<const std::string &>().empty());
}

TEST_CASE("metatypes, std::vector<std::string>, MetaIterable")
{
	std::vector<std::string> original {
		"good", "great", "perfect"
	};
	metapp::Variant v(original);
	REQUIRE(v.getMetaType()->getMetaIterable() != nullptr);

	std::vector<std::string> newVector;
	metapp::iterableForEach(v, [&newVector](const metapp::Variant & value) {
		REQUIRE(metapp::getTypeKind(value) == metapp::tkReference);
		newVector.push_back(value.get<std::string>());
		return true;
	});
	REQUIRE(original == newVector);
}

