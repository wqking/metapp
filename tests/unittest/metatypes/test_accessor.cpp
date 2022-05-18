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

TEST_CASE("metatypes, Accessor")
{
	int n = 5;
	metapp::Variant v(metapp::createAccessor(&n));
	REQUIRE(v.getMetaType()->getMetaAccessible()->get(v, nullptr).get<int>() == 5);

	v.getMetaType()->getMetaAccessible()->set(v, nullptr, 38);
	REQUIRE(n == 38);
	REQUIRE(v.getMetaType()->getMetaAccessible()->get(v, nullptr).get<int>() == 38);

	v.getMetaType()->getMetaAccessible()->set(v, nullptr, 98.0);
	REQUIRE(n == 98);
	REQUIRE(v.getMetaType()->getMetaAccessible()->get(v, nullptr).get<int>() == 98);
}

TEST_CASE("metatypes, Accessor, member data, int Class1::*")
{
	struct Class1 { int value; };

	metapp::Variant v(metapp::createAccessor<int>(&Class1::value, &Class1::value));
	Class1 obj;
	obj.value = 5;
	REQUIRE(v.getMetaType()->getMetaAccessible()->get(v, &obj).get<int>() == 5);

	v.getMetaType()->getMetaAccessible()->set(v, &obj, 38);
	REQUIRE(obj.value == 38);
	REQUIRE(v.getMetaType()->getMetaAccessible()->get(v, &obj).get<int>() == 38);

	v.getMetaType()->getMetaAccessible()->set(v, &obj, 98.0);
	REQUIRE(obj.value == 98);
	REQUIRE(v.getMetaType()->getMetaAccessible()->get(v, &obj).get<int>() == 98);
}

TEST_CASE("metatypes, Accessor, member data, std::map<int, std::string> Class1::*")
{
	using Map = std::map<int, std::string>;
	struct Class1 { Map value; };

	metapp::Variant v(metapp::createAccessor<const Map &>(&Class1::value, &Class1::value));
	Class1 obj;
	obj.value = { { 1, "a" }, { 2, "b" } };
	metapp::Variant fieldValue = v.getMetaType()->getMetaAccessible()->get(v, &obj);
	REQUIRE(fieldValue.getMetaType()->getUpType(0)->getTypeKind() == metapp::tkStdMap);
	REQUIRE(fieldValue.getMetaType()->getUpType()->getUpType(0)->getTypeKind() == metapp::tkInt);
	REQUIRE(fieldValue.getMetaType()->getUpType()->getUpType(1)->getTypeKind() == metapp::tkStdString);
	REQUIRE(fieldValue.get<const Map &>() == obj.value);
}

TEST_CASE("metatypes, Accessor, MetaAccessible, int")
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

TEST_CASE("metatypes, Accessor, MetaAccessible, int, read only")
{
	int n = 5;
	metapp::Variant v(metapp::createReadOnlyAccessor<int>(&n));
	REQUIRE(metapp::accessibleIsReadOnly(v));
	REQUIRE(metapp::accessibleGet(v, nullptr).get<int>() == 5);

	REQUIRE_THROWS(metapp::accessibleSet(v, nullptr, 38));
	REQUIRE(metapp::accessibleGet(v, nullptr).get<int>() == 5);
}


