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
#include "metapp/metatypes/metatypes.h"
#include "metapp/inheritancerepo.h"
#include "metapp/utils/utility.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

struct A
{
	A() : text("default") {}
	explicit A(const std::string & text) : text(text) {}
	explicit A(const std::string & text, const int n) : text(text + std::to_string(n)) {}

	std::string text;
};

} // namespace

template <>
struct metapp::DeclareMetaType <A> : public metapp::DeclareMetaTypeBase <A>
{
	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<A>(),
			[](metapp::MetaClass & mc) {
				mc.addConstructor(metapp::Constructor<A ()>());
				// the return type can be pointer
				mc.addConstructor(metapp::Constructor<const A * (const std::string &)>());
				// the return type can be reference, and argument types don't need to exactly match
				mc.addConstructor(metapp::Constructor<const A & (std::string, int)>());
			}
		);
		return &metaClass;
	}

};

TEST_CASE("MetaClass, constructor")
{
	auto metaTypeA = metapp::getMetaType<A>();
	auto metaClassA = metaTypeA->getMetaClass();
	auto constructorList = metaClassA->getConstructorList();
	SECTION("()") {
		auto it = metapp::findCallable(constructorList.begin(), constructorList.end(), nullptr, 0);
		REQUIRE(it == constructorList.begin());
		const metapp::Variant obj = metapp::Variant::takeFrom(metapp::invokeCallable(*it, nullptr));
		REQUIRE(obj.get<const A &>().text == "default");
	}
	SECTION("(std::string)") {
		const metapp::Variant arguments[] = { "abc" };
		auto it = metapp::findCallable(constructorList.begin(), constructorList.end(), arguments, 1);
		REQUIRE(it == constructorList.begin() + 1);
		const metapp::Variant obj = metapp::Variant::takeFrom(metapp::invokeCallable(*it, nullptr, "abc"));
		REQUIRE(obj.get<const A &>().text == "abc");
	}
	SECTION("(std::string, int)") {
		const metapp::Variant arguments[] = { "abc", 5 };
		auto it = metapp::findCallable(constructorList.begin(), constructorList.end(), arguments, 2);
		REQUIRE(it == constructorList.begin() + 2);
		const metapp::Variant obj = metapp::Variant::takeFrom(metapp::invokeCallable(*it, nullptr, "abc", 5));
		REQUIRE(obj.get<const A &>().text == "abc5");
	}
}

