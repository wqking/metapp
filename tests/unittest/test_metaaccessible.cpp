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
#include "metapp/interfaces/metaaccessible.h"
#include "metapp/allmetatypes.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

TEST_CASE("MetaAccessible, pointer, int *")
{
	int n = 5;
	metapp::Variant v(&n);
	auto metaAccessible = v.getMetaType()->getMetaAccessible();
	REQUIRE(metaAccessible != nullptr);
	REQUIRE(! metaAccessible->isReadOnly(v));
	REQUIRE(metaAccessible->getValueType(v) == metapp::getMetaType<int>());
	REQUIRE(metaAccessible->get(v, nullptr).get<int>() == 5);

	n = 10;
	REQUIRE(metaAccessible->get(v, nullptr).get<int>() == 10);

	metaAccessible->set(v, nullptr, 38);
	REQUIRE(metaAccessible->get(v, nullptr).get<int>() == 38);
}

TEST_CASE("MetaAccessible, pointer, const int *")
{
	const int n = 5;
	metapp::Variant v(&n);
	auto metaAccessible = v.getMetaType()->getMetaAccessible();
	REQUIRE(metaAccessible != nullptr);
	REQUIRE(metaAccessible->isReadOnly(v));
	REQUIRE(metaAccessible->getValueType(v) == metapp::getMetaType<const int>());
	REQUIRE(metaAccessible->get(v, nullptr).get<int>() == 5);

	REQUIRE_THROWS(metaAccessible->set(v, nullptr, 38));
	REQUIRE(metaAccessible->get(v, nullptr).get<int>() == 5);
}

TEST_CASE("MetaAccessible, member pointer, std::string MyClass:: *")
{
	struct MyClass { std::string text; };
	MyClass obj { "hello"};
	metapp::Variant v(&obj.text);
	REQUIRE(! metapp::accessibleIsReadOnly(v));
	REQUIRE(metapp::accessibleGetValueType(v) == metapp::getMetaType<std::string>());
	REQUIRE(metapp::accessibleGet(v, &obj).get<const std::string &>() == "hello");

	obj.text = "world";
	REQUIRE(metapp::accessibleGet(v, &obj).get<const std::string &>() == "world");

	metapp::accessibleSet(v, &obj, "good");
	REQUIRE(metapp::accessibleGet(v, &obj).get<const std::string &>() == "good");
}

TEST_CASE("MetaAccessible, member pointer, const std::string MyClass:: *")
{
	struct MyClass { const std::string text; };
	MyClass obj { "hello"};
	metapp::Variant v(&obj.text);
	REQUIRE(metapp::accessibleIsReadOnly(v));
	REQUIRE(metapp::accessibleGetValueType(v) == metapp::getMetaType<const std::string>());
	REQUIRE(metapp::accessibleGet(v, &obj).get<const std::string &>() == "hello");

	REQUIRE_THROWS(metapp::accessibleSet(v, &obj, "good"));
	REQUIRE(metapp::accessibleGet(v, &obj).get<const std::string &>() == "hello");
}

TEST_CASE("MetaAccessible, Accessor, int")
{
	int n = 5;
	metapp::Variant v(metapp::createAccessor<int>(&n, &n));
	REQUIRE(! metapp::accessibleIsReadOnly(v));
	REQUIRE(metapp::accessibleGet(v, nullptr).get<int>() == 5);

	metapp::accessibleSet(v, nullptr, 38);
	REQUIRE(n == 38);
	REQUIRE(metapp::accessibleGet(v, nullptr).get<int>() == 38);

	metapp::accessibleSet(v, nullptr, 98.0);
	REQUIRE(n == 98);
	REQUIRE(metapp::accessibleGet(v, nullptr).get<int>() == 98);
}

TEST_CASE("MetaAccessible, Accessor, int, read only")
{
	int n = 5;
	metapp::Variant v(metapp::createReadOnlyAccessor<int>(&n));
	REQUIRE(metapp::accessibleIsReadOnly(v));
	REQUIRE(metapp::accessibleGet(v, nullptr).get<int>() == 5);

	REQUIRE_THROWS(metapp::accessibleSet(v, nullptr, 38));
	REQUIRE(metapp::accessibleGet(v, nullptr).get<int>() == 5);
}



} // namespace
