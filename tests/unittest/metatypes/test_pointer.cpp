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

TEST_CASE("metatypes, void *")
{
	metapp::Variant v((void *)0);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkPointer, tkVoid }));
}

TEST_CASE("metatypes, void ***")
{
	void *** p = nullptr;
	metapp::Variant v(p);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkPointer, tkPointer, tkPointer, tkVoid }));
}

TEST_CASE("metatypes, nullptr")
{
	metapp::Variant v(nullptr);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	REQUIRE(v.getMetaType()->isPointer());
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkPointer, tkVoid }));
}

TEST_CASE("metatypes, const volatile void *")
{
	metapp::Variant v((const volatile void *)0);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkPointer, tkVoid }));
	REQUIRE(v.getMetaType()->getUpType()->isConst());
	REQUIRE(v.getMetaType()->getUpType()->isVolatile());
}

TEST_CASE("metatypes, const char *")
{
	REQUIRE(metapp::getMetaType<const char *>()->getTypeKind() == metapp::tkPointer);
}

TEST_CASE("metatypes, int **")
{
	metapp::Variant v((int **)0);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	REQUIRE(v.canGet<int **>());
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkPointer, tkPointer, tkInt }));
}

TEST_CASE("metatypes, std::string *, MetaAccessible")
{
	std::string text;
	metapp::Variant v(&text);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	REQUIRE(metapp::accessibleGetClassType(v)->isVoid());
	REQUIRE(metapp::accessibleGetValueType(v)->equal(metapp::getMetaType<std::string>()));
	REQUIRE(metapp::accessibleGetValueType(v) == metapp::getMetaType<std::string>());
	REQUIRE(! metapp::accessibleIsReadOnly(v));

	REQUIRE(metapp::accessibleGet(v, nullptr).getMetaType()->isReference());
	REQUIRE(metapp::accessibleGet(v, nullptr).get<const std::string &>() == "");
	metapp::accessibleSet(v, nullptr, "hello");
	REQUIRE(metapp::accessibleGet(v, nullptr).get<const std::string &>() == "hello");
}

TEST_CASE("metatypes, const std::string *, MetaAccessible")
{
	const std::string text("good");
	metapp::Variant v(&text);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	REQUIRE(metapp::accessibleGetClassType(v)->isVoid());
	REQUIRE(metapp::accessibleGetValueType(v)->equal(metapp::getMetaType<std::string>()));
	REQUIRE(metapp::accessibleGetValueType(v) == metapp::getMetaType<const std::string>());
	REQUIRE(metapp::accessibleIsReadOnly(v));

	REQUIRE(metapp::accessibleGet(v, nullptr).get<const std::string &>() == "good");
	REQUIRE_THROWS(metapp::accessibleSet(v, nullptr, "hello"));
	REQUIRE(metapp::accessibleGet(v, nullptr).get<const std::string &>() == "good");
}

