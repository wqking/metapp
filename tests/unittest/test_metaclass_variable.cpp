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
				mc.registerVariable("a1", 1);
				mc.registerVariable("a2", "abc");
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
				mc.registerVariable("b1", nullptr);
				mc.registerVariable("b2", std::string("def"));
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
				mc.registerVariable("b22", 22);
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
				mc.registerVariable("c1", 38);
				mc.registerVariable("c2", 9);
				mc.registerVariable("c3", 19);
			}
		);
		return &metaClass;
	}

};


TEST_CASE("MetaClass, variable, struct B")
{
	auto metaTypeB = metapp::getMetaType<B>();
	auto metaClassB = metaTypeB->getMetaClass();
	
	REQUIRE(metaClassB->getVariable("notExist").isEmpty());
	
	const auto & b1= metaClassB->getVariable("b1");
	REQUIRE(b1.asVariable().get<void *>() == nullptr);

	const auto & a1 = metaClassB->getVariable("a1");
	REQUIRE(a1.asVariable().get<int>() == 1);
}

TEST_CASE("MetaClass, variable, struct C")
{
	auto metaTypeC = metapp::getMetaType<C>();
	auto metaClassC = metaTypeC->getMetaClass();

	REQUIRE(metaClassC->getVariable("notExist").isEmpty());

	const auto & c1 = metaClassC->getVariable("c1");
	REQUIRE(c1.asVariable().get<int>() == 38);
	const auto & c2 = metaClassC->getVariable("c2");
	REQUIRE(c2.asVariable().get<int>() == 9);
	const auto & c3 = metaClassC->getVariable("c3");
	REQUIRE(c3.asVariable().get<int>() == 19);
}

TEST_CASE("MetaClass, variable, struct C, getVariableView")
{
	auto metaTypeC = metapp::getMetaType<C>();
	auto metaClassC = metaTypeC->getMetaClass();

	std::map<std::string, int> variableNameMap;
	auto variableView = metaClassC->getVariableView();
	for(const auto & item : variableView) {
		++variableNameMap[item.getName()];
	}
	REQUIRE(variableNameMap["a1"] == 1);
	REQUIRE(variableNameMap["a2"] == 1);
	REQUIRE(variableNameMap["b1"] == 1);
	REQUIRE(variableNameMap["b2"] == 1);
	REQUIRE(variableNameMap["c1"] == 1);
	REQUIRE(variableNameMap["c2"] == 1);
	REQUIRE(variableNameMap["c3"] == 1);
}

