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
#include "metapp/interfaces/metaiterable.h"

TEST_CASE("metatypes, std::pair<int, std::string>")
{
	using Type = std::pair<int, std::string>;
	Type container { 1, "perfect" };
	metapp::Variant v(container);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdPair);
	REQUIRE(v.get<Type &>().first == 1);
	REQUIRE(v.get<Type &>().second == "perfect");
	v.get<Type &>().first = 2;
	v.get<Type &>().second = "good";
	REQUIRE(v.get<Type &>().first == 2);
	REQUIRE(v.get<Type &>().second == "good");
	REQUIRE(v.getMetaType()->getUpTypeCount() == 2);
	REQUIRE(v.getMetaType()->getUpType(0)->getTypeKind() == metapp::tkInt);
	REQUIRE(v.getMetaType()->getUpType(1)->getTypeKind() == metapp::tkStdString);
}

TEST_CASE("metatypes, std::pair, MetaIndexable")
{
	metapp::Variant v(std::make_pair(5, std::string("Hello")));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdPair);
	auto metaType = v.getMetaType();
	auto metaIndexable = metaType->getMetaIndexable();
	REQUIRE(metaIndexable != nullptr);
	REQUIRE(metaIndexable->getSizeInfo(v).getSize() == 2);
	REQUIRE(! metaIndexable->getSizeInfo(v).isResizable());
	REQUIRE(! metaIndexable->getSizeInfo(v).isUnknownSize());
	REQUIRE(metaIndexable->get(v, 0).getMetaType()->isReference());
	REQUIRE(metaIndexable->get(v, 1).getMetaType()->isReference());
	REQUIRE(metaIndexable->get(v, 0).get<int>() == 5);
	REQUIRE(metaIndexable->get(v, 1).get<const std::string &>() == "Hello");
}

TEST_CASE("metatypes, std::pair, MetaIndexable, reference and set")
{
	auto pair = std::make_pair(5, std::string("Hello"));
	metapp::Variant v(metapp::Variant::reference(pair));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
	REQUIRE(metapp::indexableGetSizeInfo(v).getSize() == 2);
	REQUIRE(! metapp::indexableGetSizeInfo(v).isResizable());
	REQUIRE(! metapp::indexableGetSizeInfo(v).isUnknownSize());
	REQUIRE(metapp::indexableGet(v, 0).get<int>() == 5);
	REQUIRE(metapp::indexableGet(v, 1).get<const std::string &>() == "Hello");

	metapp::indexableSet(v, 0, 38);
	REQUIRE(metapp::indexableGet(v, 0).get<int>() == 38);
	REQUIRE(pair.first == 38);

	metapp::indexableSet(v, 1, "world");
	REQUIRE(metapp::indexableGet(v, 1).get<const std::string &>() == "world");
	REQUIRE(pair.second == "world");
}

TEST_CASE("metatypes, std::pair, MetaIterable")
{
	metapp::Variant v(std::make_pair(5, std::string("Hello")));
	int index = 0;
	metapp::iterableForEach(v, [&index](const metapp::Variant & value) {
		REQUIRE(value.getMetaType()->isReference());
		REQUIRE(index < 2);
		if(index == 0) {
			REQUIRE(value.get<int>() == 5);
		}
		else {
			REQUIRE(value.get<const std::string &>() == "Hello");
		}
		++index;
		return true;
	});
}
