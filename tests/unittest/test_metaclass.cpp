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

#include <string>
#include <iostream>
#include <climits>

struct TestClass_63575107
{
	TestClass_63575107() : value(0) {}
	explicit TestClass_63575107(const int value) : value(value) {}

	int value;

	int add(const int n) const {
		return n + value;
	}
};

template <>
struct metapp::DeclareMetaType <TestClass_63575107> : metapp::DeclareMetaTypeBase <TestClass_63575107>
{
	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<TestClass_63575107>(),
			[](metapp::MetaClass & mc) {
				mc.registerConstructor(metapp::Constructor<TestClass_63575107()>());
				mc.registerConstructor(metapp::Constructor<TestClass_63575107(int)>());
				mc.registerAccessible("value", &TestClass_63575107::value);
				mc.registerCallable("add", &TestClass_63575107::add);
				mc.registerVariable("fakeVar", "ThisIsVariable");
			}
		);
		return &metaClass;
	}

};

TEST_CASE("MetaClass, TestClass_63575107, getItem")
{
	auto metaType = metapp::getMetaType<TestClass_63575107>();
	auto metaClass = metaType->getMetaClass();
	TestClass_63575107 obj {6};
	auto metaItem = metaClass->getItem("add");
	REQUIRE(metapp::callableInvoke(metaItem, &obj, 5).get<int>() == 11);
}

