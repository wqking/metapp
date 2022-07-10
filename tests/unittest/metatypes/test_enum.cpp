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

enum EnumAnimal {
	cat = 5,
	dog = 38
};

TEST_CASE("metatypes, enum")
{
	auto metaType = metapp::getMetaType<EnumAnimal>();
	REQUIRE(metaType->getTypeKind() == metapp::tkEnum);
	// the underlying type may be different, on MSVC it's tkInt, on GCC it's tkUnsignedInt
	//REQUIRE(metaType->getUpType()->getTypeKind() == metapp::tkInt);
	metapp::Variant v(EnumAnimal::cat);
	REQUIRE(v.canCast<int>());
	REQUIRE(metapp::getTypeKind(v.cast<int>()) == metapp::tkInt);
	REQUIRE(v.cast<int>().canGet<int>());
	REQUIRE(v.cast<int>().get<int>() == 5);
}

TEST_CASE("metatypes, const enum")
{
	auto metaType = metapp::getMetaType<const EnumAnimal>();
	REQUIRE(metaType->getTypeKind() == metapp::tkEnum);
	// the underlying type may be different, on MSVC it's tkInt, on GCC it's tkUnsignedInt
	//REQUIRE(metaType->getUpType()->getTypeKind() == metapp::tkInt);
	metapp::Variant v(EnumAnimal::cat);
	REQUIRE(v.canCast<int>());
	REQUIRE(metapp::getTypeKind(v.cast<int>()) == metapp::tkInt);
	REQUIRE(v.cast<int>().canGet<int>());
	REQUIRE(v.cast<int>().get<int>() == 5);
}

enum class EnumHuman : char {
	male = 9,
	female = 10
};

template <>
struct metapp::DeclareMetaType <EnumHuman> : metapp::DeclareMetaTypeBase <EnumHuman>
{
	static constexpr metapp::TypeKind typeKind = 2000;

	static const metapp::MetaEnum * getMetaEnum() {
		static const metapp::MetaEnum metaEnum([](metapp::MetaEnum & me) {
			me.registerValue("male", EnumHuman::male);
			me.registerValue("female", EnumHuman::female);
		});
		return &metaEnum;
	}
};

TEST_CASE("metatypes, enum class")
{
	auto metaType = metapp::getMetaType<EnumHuman>();
	REQUIRE(metaType->getTypeKind() == 2000);
	REQUIRE(metaType->getUpType()->getTypeKind() == metapp::tkChar);
	const auto metaEnum = metaType->getMetaEnum();
	REQUIRE(metaEnum != nullptr);
	REQUIRE(metaEnum->getByName("male").asEnumValue().get<EnumHuman>() == EnumHuman::male);
	REQUIRE(metaEnum->getByName("female").asEnumValue().get<EnumHuman>() == EnumHuman::female);
	REQUIRE(metaEnum->getByValue(EnumHuman::male).getName() == "male");
	REQUIRE(metaEnum->getByValue(EnumHuman::female).getName() == "female");

	{
		metapp::Variant v(EnumHuman::female);
		REQUIRE(v.canCast<int>());
		REQUIRE(v.cast<int>().get<int>() == 10);
	}

	{
		metapp::Variant v(9);
		REQUIRE(v.canCast<EnumHuman>());
		REQUIRE(v.cast<EnumHuman>().get<EnumHuman>() == EnumHuman::male);
	}

}

