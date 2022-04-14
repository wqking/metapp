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

#include "../test.h"

#include "metapp/variant.h"
#include "metapp/allmetatypes.h"

namespace {

struct Class1
{
	Class1() : data(), value(), constValue(5) {}

	const void * func(int, const std::vector<int> &) { return nullptr; }
	const std::array<int, 5> * data;
	int value;
	const int constValue;
};

} // namespace

template <>
struct metapp::DeclareMetaType <Class1> : metapp::DeclareMetaTypeBase<Class1>
{
	static constexpr metapp::TypeKind typeKind = 2000;
};

namespace {

TEST_CASE("metatypes, tkMemberPointer, types")
{
	metapp::Variant v(&Class1::data);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkMemberPointer);
	
	using namespace metapp;
	auto metaType = v.getMetaType();
	REQUIRE(matchUpTypeKinds(metaType->getUpType(0), { 2000 }));
	REQUIRE(matchUpTypeKinds(metaType->getUpType(1), { tkPointer, tkStdArray, tkInt }));
}

TEST_CASE("metatypes, tkMemberPointer, MetaAccessible, non-const")
{
	metapp::Variant v(&Class1::value);
	Class1 obj;
	auto metaType = v.getMetaType();
	auto metaAccessible = metaType->getMetaAccessible();
	REQUIRE(metaAccessible != nullptr);
	REQUIRE(metaAccessible->get(v, &obj).get<int>() == 0);
	obj.value = 5;
	REQUIRE(metaAccessible->get(v, &obj).get<int>() == 5);
	metaAccessible->set(v, &obj, 38);
	REQUIRE(obj.value == 38);
	REQUIRE(metaAccessible->get(v, &obj).get<int>() == 38);
}

TEST_CASE("metatypes, member data, MetaAccessible, const")
{
	metapp::Variant v(&Class1::constValue);
	Class1 obj;
	auto metaType = v.getMetaType();
	auto metaAccessible = metaType->getMetaAccessible();
	REQUIRE(metaAccessible != nullptr);
	REQUIRE(metaAccessible->get(v, &obj).get<int>() == 5);
	REQUIRE_THROWS(metaAccessible->set(v, &obj, 38));
	REQUIRE(metaAccessible->get(v, &obj).get<int>() == 5);
}



} // namespace