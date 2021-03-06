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
#include "metapp/utilities/utility.h"

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
struct metapp::DeclareMetaType <A> : metapp::DeclareMetaTypeBase <A>
{
	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<A>(),
			[](metapp::MetaClass & mc) {
				mc.registerConstructor(metapp::Constructor<A ()>());
				// the return type can be pointer
				mc.registerConstructor(metapp::Constructor<const A * (const std::string &)>());
				// the return type can be reference, and argument types don't need to exactly match
				mc.registerConstructor(metapp::Constructor<const A & (std::string, int)>());
			}
		);
		return &metaClass;
	}

};

TEST_CASE("MetaClass, constructor")
{
	auto metaTypeA = metapp::getMetaType<A>();
	auto metaClassA = metaTypeA->getMetaClass();
	auto constructor = metaClassA->getConstructor();
	SECTION("()") {
		const metapp::Variant obj = metapp::Variant::takeFrom(metapp::callableInvoke(constructor, nullptr));
		REQUIRE(obj.get<const A &>().text == "default");
	}
	SECTION("(std::string)") {
		const metapp::Variant obj = metapp::Variant::takeFrom(metapp::callableInvoke(constructor, nullptr, "abc"));
		REQUIRE(obj.get<const A &>().text == "abc");
	}
	SECTION("(std::string, int)") {
		const metapp::Variant obj = metapp::Variant::takeFrom(metapp::callableInvoke(constructor, nullptr, "abc", 5));
		REQUIRE(obj.get<const A &>().text == "abc5");
	}
}

