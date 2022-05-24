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

TEST_CASE("metatypes, tkStdArray, std::array<int, 5>")
{
	using Type = std::array<int, 5>;
	Type container { 38, 98, 5, 16, 99 };
	metapp::Variant v(container);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdArray);
	REQUIRE(v.get<Type>()[0] == 38);
	REQUIRE(v.get<Type>()[1] == 98);
	REQUIRE(v.get<Type>()[2] == 5);
	REQUIRE(v.get<Type>()[3] == 16);
	REQUIRE(v.get<Type>()[4] == 99);
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkStdArray, tkInt }));
}

TEST_CASE("metatypes, tkStdArray, std::array<int, 5>, MetaIndexable")
{
	using Type = std::array<int, 5>;
	metapp::Variant v(Type { 38, 98, 5, 16, 99 });
	REQUIRE(v.getMetaType()->getMetaIndexable() != nullptr);
	REQUIRE(v.getMetaType()->getMetaIndexable()->getSizeInfo(v).getSize() == 5);
	REQUIRE(v.getMetaType()->getMetaIndexable()->get(v, 0).get<int>() == 38);
	REQUIRE(v.getMetaType()->getMetaIndexable()->get(v, 1).get<int>() == 98);
	REQUIRE(v.getMetaType()->getMetaIndexable()->get(v, 2).get<int>() == 5);
	REQUIRE(v.getMetaType()->getMetaIndexable()->get(v, 3).get<int>() == 16);
	REQUIRE(v.getMetaType()->getMetaIndexable()->get(v, 4).get<int>() == 99);

	REQUIRE_THROWS(v.getMetaType()->getMetaIndexable()->set(v, 5, 3));
	REQUIRE_THROWS(v.getMetaType()->getMetaIndexable()->set(v, 0, "abc"));
	REQUIRE(v.get<Type &>()[0] == 38);
	v.getMetaType()->getMetaIndexable()->set(v, 0, 3);
	REQUIRE(v.get<Type &>()[0] == 3);
}

TEST_CASE("metatypes, tkStdArray, const std::array<int, 5>, MetaIndexable, can't set to const std::array")
{
	using Type = const std::array<int, 5>;
	metapp::Variant v(metapp::Variant::create<Type>({ 38, 98, 5, 16, 99 }));
	REQUIRE(v.getMetaType()->getMetaIndexable() != nullptr);
	REQUIRE(v.getMetaType()->getMetaIndexable()->getSizeInfo(v).getSize() == 5);
	REQUIRE(v.getMetaType()->getMetaIndexable()->get(v, 0).get<int>() == 38);
	REQUIRE(v.getMetaType()->getMetaIndexable()->get(v, 1).get<int>() == 98);
	REQUIRE(v.getMetaType()->getMetaIndexable()->get(v, 2).get<int>() == 5);
	REQUIRE(v.getMetaType()->getMetaIndexable()->get(v, 3).get<int>() == 16);
	REQUIRE(v.getMetaType()->getMetaIndexable()->get(v, 4).get<int>() == 99);

	REQUIRE_THROWS(v.getMetaType()->getMetaIndexable()->set(v, 5, 3));
	REQUIRE_THROWS(v.getMetaType()->getMetaIndexable()->set(v, 0, "abc"));
	REQUIRE(v.get<Type &>()[0] == 38);
	REQUIRE_THROWS(v.getMetaType()->getMetaIndexable()->set(v, 0, 3));
	REQUIRE(v.get<Type &>()[0] == 38);
}

TEST_CASE("metatypes, tkStdArray, const std::array<int, 5> &, MetaIndexable, can't set to const std::array &")
{
	using Type = std::array<int, 5>;
	Type array { 38, 98, 5, 16, 99 };
	metapp::Variant v(metapp::Variant::create<const Type &>(array));
	auto metaIndexable = v.getMetaType()->getUpType()->getMetaIndexable();
	REQUIRE(metaIndexable != nullptr);
	REQUIRE(metaIndexable->getSizeInfo(v).getSize() == 5);
	REQUIRE(metaIndexable->get(v, 0).get<int>() == 38);
	REQUIRE(metaIndexable->get(v, 1).get<int>() == 98);
	REQUIRE(metaIndexable->get(v, 2).get<int>() == 5);
	REQUIRE(metaIndexable->get(v, 3).get<int>() == 16);
	REQUIRE(metaIndexable->get(v, 4).get<int>() == 99);

	REQUIRE_THROWS(metaIndexable->set(v, 5, 3));
	REQUIRE_THROWS(metaIndexable->set(v, 0, "abc"));
	REQUIRE(v.get<Type &>()[0] == 38);
	REQUIRE_THROWS(metaIndexable->set(v, 0, 3));
	REQUIRE(v.get<Type &>()[0] == 38);
}

TEST_CASE("metatypes, tkStdArray, std::array<int, 5>, MetaIterable")
{
	std::array<int, 5> original { 38, 98, 5, 16, 99 };
	metapp::Variant v(original);
	REQUIRE(v.getMetaType()->getMetaIterable() != nullptr);

	std::array<int, 5> newArray;
	size_t index = 0;
	v.getMetaType()->getMetaIterable()->forEach(v, [&newArray, &index](const metapp::Variant & value) {
		REQUIRE(index < newArray.size());
		newArray[index] = value.get<int>();
		++index;
		return true;
	});
	REQUIRE(original == newArray);
}

