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
#include "metapp/interfaces/metaclass.h"
#include "metapp/allmetatypes.h"
#include "metapp/metarepo.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

struct A
{
};

struct B : A
{
};

struct B2
{
};

struct C : B, B2
{
};

metapp::MetaRepo tmvMetaRepo;

} // namespace

template <>
struct metapp::DeclareMetaType <A> : metapp::DeclareMetaTypeBase <A>
{
	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<A>(),
			[](metapp::MetaClass & mc) {
				mc.registerType("string", getMetaType<std::string>());
				mc.registerType("int", getMetaType<int>());
			}
		);
		return &metaClass;
	}

};

template <>
struct metapp::DeclareMetaType <B> : metapp::DeclareMetaTypeBase <B>
{
	static void setup()
	{
		tmvMetaRepo.registerBase<B, A>();
	}

	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<B>(),
			[](metapp::MetaClass & mc) {
				mc.registerType("string", getMetaType<const char *>());
				mc.registerType("vector<int>", getMetaType<std::vector<int> >());
			}
		);
		return &metaClass;
	}

};

template <>
struct metapp::DeclareMetaType <B2> : metapp::DeclareMetaTypeBase <B2>
{
	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<B2>(),
			[](metapp::MetaClass & mc) {
				mc.registerType("long", getMetaType<long>());
			}
		);
		return &metaClass;
	}

};

template <>
struct metapp::DeclareMetaType <C> : metapp::DeclareMetaTypeBase <C>
{
	static void setup()
	{
		tmvMetaRepo.registerBase<C, B, B2>();
	}

	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<C>(),
			[](metapp::MetaClass & mc) {
				mc.registerType("char", getMetaType<char>());
				mc.registerType("short", getMetaType<short>());
				mc.registerType("double", getMetaType<double>());
			}
		);
		return &metaClass;
	}

};


TEST_CASE("MetaClass, type, struct B")
{
	auto metaTypeB = metapp::getMetaType<B>();
	auto metaClassB = metaTypeB->getMetaClass();
	
	REQUIRE(metaClassB->getType("notExist").isEmpty());
	
	const auto & bString= metaClassB->getType("string");
	REQUIRE(bString.asMetaType() == metapp::getMetaType<const char *>());

	const auto & aInt = metaClassB->getType("int");
	REQUIRE(aInt.asMetaType() == metapp::getMetaType<int>());
}

TEST_CASE("MetaClass, type, struct C")
{
	auto metaTypeC = metapp::getMetaType<C>();
	auto metaClassC = metaTypeC->getMetaClass();

	REQUIRE(metaClassC->getType("notExist").isEmpty());

	const auto & cChar = metaClassC->getType("char");
	REQUIRE(&cChar == &metaClassC->getType(metapp::tkChar));
	REQUIRE(&cChar == &metaClassC->getType(metapp::getMetaType<char>()));
	REQUIRE(cChar.asMetaType() == metapp::getMetaType<char>());
	const auto & cShort = metaClassC->getType("short");
	REQUIRE(cShort.asMetaType() == metapp::getMetaType<short>());
	const auto & cDouble = metaClassC->getType("double");
	REQUIRE(cDouble.asMetaType() == metapp::getMetaType<double>());
}

TEST_CASE("MetaClass, type, struct C, getTypeView")
{
	auto metaTypeC = metapp::getMetaType<C>();
	auto metaClassC = metaTypeC->getMetaClass();

	std::map<std::string, int> typeNameMap;
	auto typeView = metaClassC->getTypeView();
	for(const auto & item : typeView) {
		++typeNameMap[item.getName()];
	}
	REQUIRE(typeNameMap["string"] == 2);
	REQUIRE(typeNameMap["int"] == 1);
	REQUIRE(typeNameMap["vector<int>"] == 1);
	REQUIRE(typeNameMap["long"] == 1);
	REQUIRE(typeNameMap["char"] == 1);
	REQUIRE(typeNameMap["short"] == 1);
	REQUIRE(typeNameMap["double"] == 1);
}

