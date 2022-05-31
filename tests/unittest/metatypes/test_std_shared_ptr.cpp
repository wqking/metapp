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

TEST_CASE("metatypes, std::shared_ptr<int>")
{
	std::shared_ptr<int> sp = std::make_shared<int>(38);
	metapp::Variant v(sp);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdSharedPtr);
	REQUIRE(v.canGet<std::shared_ptr<int> >());
	REQUIRE(*(v.get<std::shared_ptr<int> >()) == 38);
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkStdSharedPtr, tkInt }));
}

TEST_CASE("metatypes, std::shared_ptr<int>, cast to pointer")
{
	std::shared_ptr<int> sp = std::make_shared<int>(5);
	metapp::Variant v(sp);
	REQUIRE(*sp == 5);
	REQUIRE(v.cast<const int *>().get<const int *>() == sp.get());
	REQUIRE(*v.cast<const int *>().get<const int *>() == 5);
	*v.cast<int *>().get<int *>() = 38;
	REQUIRE(*sp == 38);
	REQUIRE(*v.cast<const int *>().get<const int *>() == 38);
}

TEST_CASE("metatypes, std::shared_ptr<int>, cast to std::weak_ptr")
{
	std::shared_ptr<int> sp = std::make_shared<int>(9);
	metapp::Variant v(sp);
	REQUIRE(*sp == 9);
	std::weak_ptr<int> wp = v.cast<std::weak_ptr<int> &>().get<std::weak_ptr<int> &>();
	std::shared_ptr<int> wsp = wp.lock();
	REQUIRE(*wsp == 9);
}

TEST_CASE("metatypes, std::shared_ptr<std::string>, MetaAccessible")
{
	std::shared_ptr<std::string> sp = std::make_shared<std::string>("good");
	metapp::Variant v(sp);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdSharedPtr);
	REQUIRE(metapp::accessibleGetClassType(v)->isVoid());
	REQUIRE(metapp::accessibleGetValueType(v)->equal(metapp::getMetaType<std::string>()));
	REQUIRE(metapp::accessibleGetValueType(v) == metapp::getMetaType<std::string>());
	REQUIRE(! metapp::accessibleIsReadOnly(v));

	REQUIRE(metapp::accessibleGet(v, nullptr).getMetaType()->isReference());
	REQUIRE(metapp::accessibleGet(v, nullptr).get<const std::string &>() == "good");
	metapp::accessibleSet(v, nullptr, "hello");
	REQUIRE(metapp::accessibleGet(v, nullptr).get<const std::string &>() == "hello");
}

TEST_CASE("metatypes, std::shared_ptr<const std::string>, MetaAccessible")
{
	std::shared_ptr<const std::string> sp = std::make_shared<const std::string>("good");
	metapp::Variant v(sp);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdSharedPtr);
	REQUIRE(metapp::accessibleGetClassType(v)->isVoid());
	REQUIRE(metapp::accessibleGetValueType(v)->equal(metapp::getMetaType<std::string>()));
	REQUIRE(metapp::accessibleGetValueType(v) == metapp::getMetaType<const std::string>());
	REQUIRE(metapp::accessibleIsReadOnly(v));

	REQUIRE(metapp::accessibleGet(v, nullptr).get<const std::string &>() == "good");
	REQUIRE_THROWS(metapp::accessibleSet(v, nullptr, "hello"));
	REQUIRE(metapp::accessibleGet(v, nullptr).get<const std::string &>() == "good");
}

