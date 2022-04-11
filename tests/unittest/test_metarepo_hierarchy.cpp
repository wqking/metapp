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

#include "metapp/metarepo.h"
#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"
#include "metapp/utilities/metatypedumper.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

TEST_CASE("MetaRepo, hierarchy, basic")
{
	struct BaseFirst { int first; };
	struct BaseSecond { int second; };
	struct SonOfFirst : BaseFirst {};
	struct SonOfFirstSecond : BaseFirst, BaseSecond {};

	metapp::MetaRepo metaRepo;
	auto * repo = &metaRepo;
	repo->registerBase<SonOfFirst, BaseFirst>();
	repo->registerBase<SonOfFirstSecond, BaseFirst, BaseSecond>();

	REQUIRE(repo->getBases(metapp::getMetaType<SonOfFirst>()).getCount() == 1);
	REQUIRE(repo->getBases(metapp::getMetaType<const SonOfFirst>()).getCount() == 1);
	REQUIRE(repo->getBases(metapp::getMetaType<SonOfFirst>()).get(0) == metapp::getMetaType<BaseFirst>());
	REQUIRE(repo->getBases(metapp::getMetaType<const volatile SonOfFirst>()).get(0) == metapp::getMetaType<BaseFirst>());

	REQUIRE(repo->getBases(metapp::getMetaType<SonOfFirstSecond>()).getCount() == 2);
	REQUIRE(repo->getBases(metapp::getMetaType<SonOfFirstSecond>()).get(0) == metapp::getMetaType<BaseFirst>());
	REQUIRE(repo->getBases(metapp::getMetaType<SonOfFirstSecond>()).get(1) == metapp::getMetaType<BaseSecond>());

	REQUIRE(repo->getDerives(metapp::getMetaType<SonOfFirst>()).getCount() == 0);
	REQUIRE(repo->getDerives(metapp::getMetaType<BaseFirst>()).getCount() == 2);
	REQUIRE(repo->getDerives(metapp::getMetaType<BaseFirst>()).get(0) == metapp::getMetaType<SonOfFirst>());
	REQUIRE(repo->getDerives(metapp::getMetaType<BaseFirst>()).get(1) == metapp::getMetaType<SonOfFirstSecond>());
}

TEST_CASE("MetaRepo, hierarchy, add duplicated bases")
{
	struct A1 { int a1; };
	struct A2 { int a2; };
	struct B1 : A1 {};
	struct B2 : A1, A2 {};

	metapp::MetaRepo metaRepo;
	auto * repo = &metaRepo;

	repo->registerBase<B1, A1>();
	REQUIRE(repo->getBases(metapp::getMetaType<B1>()).getCount() == 1);
	REQUIRE(repo->getDerives(metapp::getMetaType<A1>()).getCount() == 1);
	repo->registerBase<B1, A1>();
	REQUIRE(repo->getBases(metapp::getMetaType<B1>()).getCount() == 1);

	repo->registerBase<B2, A1, A2>();
	REQUIRE(repo->getBases(metapp::getMetaType<B2>()).getCount() == 2);
	REQUIRE(repo->getDerives(metapp::getMetaType<A1>()).getCount() == 2);
	REQUIRE(repo->getDerives(metapp::getMetaType<A2>()).getCount() == 1);
	repo->registerBase<B2, A1, A2>();
	REQUIRE(repo->getBases(metapp::getMetaType<B2>()).getCount() == 2);
	REQUIRE(repo->getDerives(metapp::getMetaType<A1>()).getCount() == 2);
	REQUIRE(repo->getDerives(metapp::getMetaType<A2>()).getCount() == 1);
}

TEST_CASE("MetaRepo, hierarchy, castToBase and castToDerived")
{
	struct BaseFirst { int first; };
	struct BaseSecond { int second; };
	struct SonOfFirst : BaseFirst {};
	struct SonOfFirstSecond : BaseFirst, BaseSecond {};

	metapp::MetaRepo metaRepo;
	auto * repo = &metaRepo;
	repo->registerBase<SonOfFirst, BaseFirst>();
	repo->registerBase<SonOfFirstSecond, BaseFirst, BaseSecond>();

	SECTION("nullptr") {
		REQUIRE(repo->castToBase(nullptr, metapp::getMetaType<SonOfFirst>(), 0) == nullptr);
		REQUIRE(repo->castToDerived(nullptr, metapp::getMetaType<BaseFirst>(), 0) == nullptr);
	}

	SECTION("single inheritance") {
		SonOfFirst obj;
		SonOfFirst * pObj = &obj;
		BaseFirst * pBase = pObj;
		REQUIRE(repo->castToBase(pObj, metapp::getMetaType<SonOfFirst>(), 0) == pBase);
		REQUIRE(repo->castToDerived(pBase, metapp::getMetaType<BaseFirst>(), 0) == pObj);
	}

	SECTION("multiple inheritance") {
		SonOfFirstSecond obj;
		SonOfFirstSecond * pObj = &obj;
		BaseFirst * pBaseFirst = pObj;
		BaseSecond * pBaseSecond = pObj;
		
		REQUIRE((void *)pBaseFirst != (void *)pBaseSecond);

		REQUIRE(repo->castToBase(pObj, metapp::getMetaType<SonOfFirstSecond>(), 0) == pBaseFirst);
		REQUIRE(repo->castToBase(pObj, metapp::getMetaType<SonOfFirstSecond>(), 1) == pBaseSecond);
		REQUIRE(repo->castToDerived(pBaseFirst, metapp::getMetaType<BaseFirst>(), 0) == pObj);
		REQUIRE(repo->castToDerived(pBaseSecond, metapp::getMetaType<BaseSecond>(), 0) == pObj);
	}
}

TEST_CASE("MetaRepo, hierarchy, virtual inheritance castToBase and castToDerived")
{
	struct BaseFirst { int first; virtual ~BaseFirst() {}  virtual void x(){} };
	struct BaseSecond { int second; };
	struct SonOfFirstSecond : virtual BaseFirst, virtual BaseSecond { };

	metapp::MetaRepo metaRepo;
	auto * repo = &metaRepo;
	repo->registerBase<SonOfFirstSecond, BaseFirst, BaseSecond>();

	SECTION("multiple inheritance") {
		SonOfFirstSecond obj;
		SonOfFirstSecond * pObj = &obj;
		BaseFirst * pBaseFirst = pObj;
		BaseSecond * pBaseSecond = pObj;
		
		REQUIRE((void *)pBaseFirst != (void *)pBaseSecond);
		REQUIRE(dynamic_cast<SonOfFirstSecond *>(pBaseFirst) == pObj);

		REQUIRE(repo->castToBase(pObj, metapp::getMetaType<SonOfFirstSecond>(), 0) == pBaseFirst);
		REQUIRE(repo->castToBase(pObj, metapp::getMetaType<SonOfFirstSecond>(), 1) == pBaseSecond);
		REQUIRE(repo->castToDerived(pBaseFirst, metapp::getMetaType<BaseFirst>(), 0) == pObj);
		// Can't cast from BaseSecond because it doesn't have RTTI
		REQUIRE_THROWS(repo->castToDerived(pBaseSecond, metapp::getMetaType<BaseSecond>(), 0));
	}
}

TEST_CASE("MetaRepo, hierarchy, cast between ancestors")
{
	struct A1 { int a1; virtual ~A1() {} virtual void x() {} };
	struct A2 { int a2; virtual ~A2() {} virtual void y() {} };
	struct B1 : virtual A1 {};
	struct B2 : virtual A1, A2 {};
	struct C1 : B1 {};
	struct C2 : B1, B2 {};

	metapp::MetaRepo metaRepo;
	auto * repo = &metaRepo;
	repo->registerBase<B1, A1>();
	repo->registerBase<B2, A1, A2>();
	repo->registerBase<C1, B1>();
	repo->registerBase<C2, B1, B2>();

	SECTION("single inheritance, cast to base") {
		C1 c1;
		C1 * pc1 = &c1;
		B1 * pb1 = pc1;
		A1 * pa1 = pc1;

		REQUIRE(repo->cast(pc1, metapp::getMetaType<C1>(), metapp::getMetaType<B1>()) == pb1);
		REQUIRE(repo->cast(pc1, metapp::getMetaType<C1>(), metapp::getMetaType<A1>()) == pa1);
	}

	SECTION("single inheritance, cast to derived") {
		C1 c1;
		C1 * pc1 = &c1;
		B1 * pb1 = pc1;
		A1 * pa1 = pc1;

		REQUIRE(repo->cast(pa1, metapp::getMetaType<A1>(), metapp::getMetaType<B1>()) == pb1);
		REQUIRE(repo->cast(pa1, metapp::getMetaType<A1>(), metapp::getMetaType<C1>()) == pc1);
	}

	SECTION("multiple inheritance, cast to base") {
		C2 c2;
		C2 * pc2 = &c2;
		B1 * pb1 = pc2;
		B2 * pb2 = pc2;
		A1 * pa1 = pc2;
		A2 * pa2 = pc2;

		REQUIRE(repo->cast(pc2, metapp::getMetaType<C2>(), metapp::getMetaType<B1>()) == pb1);
		REQUIRE(repo->cast(pc2, metapp::getMetaType<C2>(), metapp::getMetaType<B2>()) == pb2);
		REQUIRE(repo->cast(pc2, metapp::getMetaType<C2>(), metapp::getMetaType<A1>()) == pa1);
		REQUIRE(repo->cast(pc2, metapp::getMetaType<C2>(), metapp::getMetaType<A2>()) == pa2);
	}

	SECTION("multiple inheritance, cast to derived") {
		C2 c2;
		C2 * pc2 = &c2;
		B1 * pb1 = pc2;
		B2 * pb2 = pc2;
		A1 * pa1 = pc2;
		A2 * pa2 = pc2;

		REQUIRE(repo->cast(pa1, metapp::getMetaType<A1>(), metapp::getMetaType<B1>()) == pb1);
		REQUIRE(repo->cast(pa1, metapp::getMetaType<A1>(), metapp::getMetaType<B2>()) == pb2);
		REQUIRE(repo->cast(pa1, metapp::getMetaType<A1>(), metapp::getMetaType<C2>()) == pc2);

		REQUIRE(repo->cast(pa2, metapp::getMetaType<A2>(), metapp::getMetaType<B2>()) == pb2);
		REQUIRE(repo->cast(pa2, metapp::getMetaType<A2>(), metapp::getMetaType<C2>()) == pc2);
		REQUIRE(repo->cast<A2, C2>(pa2) == pc2);

		REQUIRE(repo->cast(pa2, metapp::getMetaType<A2>(), metapp::getMetaType<B1>()) == nullptr);
	}

}

TEST_CASE("MetaRepo, hierarchy, relationship")
{
	struct A1 { int a1; virtual ~A1() {} virtual void x() {} };
	struct A2 { int a2; virtual ~A2() {} virtual void y() {} };
	struct B1 : virtual A1 {};
	struct B2 : virtual A1, A2 {};
	struct C1 : B1 {};
	struct C2 : B1, B2 {};

	metapp::MetaRepo metaRepo;
	auto * repo = &metaRepo;
	repo->registerBase<B1, A1>();
	repo->registerBase<B2, A1, A2>();
	repo->registerBase<C1, B1>();
	repo->registerBase<C2, B1, B2>();

	REQUIRE(repo->getRelationship(metapp::getMetaType<A2>(), metapp::getMetaType<B1>()) == metapp::InheritanceRelationship::none);
	REQUIRE(repo->getRelationship(metapp::getMetaType<A1>(), metapp::getMetaType<B1>()) == metapp::InheritanceRelationship::derived);
	REQUIRE(repo->getRelationship<A1, C1>() == metapp::InheritanceRelationship::derived);
	REQUIRE(repo->getRelationship<A1, C2>() == metapp::InheritanceRelationship::derived);
}


} // namespace
