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
	TestClass_63575107() : value(0), text() {}
	explicit TestClass_63575107(const int value) : value(value), text() {}
	explicit TestClass_63575107(const int value, const std::string & text) : value(value), text(text) {}

	int value;

	const std::string & getText() const {
		return text;
	}
	void setText(const std::string & newText) {
		text = newText;
	}
	std::string text;

	int add(const int n) const {
		return n + value;
	}

	std::string concat(const int a, const std::string & b) {
		return std::to_string(value) + text + std::to_string(a) + b;
	}
};

template <>
struct metapp::DeclareMetaType <TestClass_63575107> : metapp::DeclareMetaTypeBase <TestClass_63575107>
{
	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<TestClass_63575107>(),
			[](metapp::MetaClass & mc) {
				REQUIRE_THROWS(mc.registerConstructor(5));
				REQUIRE_THROWS(mc.registerAccessible("x1", 5));
				REQUIRE_THROWS(mc.registerCallable("x2", 5));

				mc.registerConstructor(metapp::Constructor<TestClass_63575107()>());
				mc.registerConstructor(metapp::Constructor<TestClass_63575107(int)>());
				mc.registerConstructor(metapp::Constructor<TestClass_63575107(int, const std::string &)>());

				mc.registerAccessible("value", &TestClass_63575107::value);
				mc.registerAccessible("text", metapp::createAccessor(&TestClass_63575107::getText, &TestClass_63575107::setText));
				
				mc.registerCallable("add", &TestClass_63575107::add);
				mc.registerCallable("concat", &TestClass_63575107::concat);
				mc.registerCallable("getText", &TestClass_63575107::getText);
				
				mc.registerVariable("fakeVar", "ThisIsVariable");
			}
		);
		return &metaClass;
	}

};

TEST_CASE("MetaClass, TestClass_63575107, constructor")
{
	auto metaType = metapp::getMetaType<TestClass_63575107>();
	auto metaClass = metaType->getMetaClass();

	SECTION("default ctor") {
		std::unique_ptr<TestClass_63575107> obj(
			metapp::callableInvoke(metaClass->getConstructor(), nullptr).get<TestClass_63575107 *>()
		);
		REQUIRE(obj->value == 0);
		REQUIRE(obj->text == "");
	}

	SECTION("ctor(int)") {
		metapp::Variant obj(metapp::Variant::takeFrom(
			metapp::callableInvoke(metaClass->getConstructor(), nullptr, 5)
		));
		REQUIRE(obj.get<const TestClass_63575107 &>().value == 5);
		REQUIRE(obj.get<const TestClass_63575107 &>().text == "");
	}

	SECTION("ctor(int, std::string)") {
		metapp::Variant obj(metapp::Variant::takeFrom(
			metapp::callableInvoke(metaClass->getConstructor(), nullptr, 5, "good")
		));
		REQUIRE(obj.get<const TestClass_63575107 &>().value == 5);
		REQUIRE(obj.get<const TestClass_63575107 &>().text == "good");
	}
}

TEST_CASE("MetaClass, TestClass_63575107, getItem")
{
	auto metaType = metapp::getMetaType<TestClass_63575107>();
	auto metaClass = metaType->getMetaClass();
	TestClass_63575107 obj {6};
	auto metaItem = metaClass->getItem("add");
	REQUIRE(metapp::callableInvoke(metaItem, &obj, 5).get<int>() == 11);
}

