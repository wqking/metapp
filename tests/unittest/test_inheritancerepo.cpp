#include "test.h"

#include "metapp/inheritancerepo.h"
#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"
#include "metapp/metarepository.h"
#include "metapp/utils/metatypedumper.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

struct BaseFirst
{
	BaseFirst() : first(0) {
	}

	explicit BaseFirst(const int value) : first(value) {
	}

	int first;
};

struct BaseSecond
{
	BaseSecond() : second(0) {
	}

	explicit BaseSecond(const int value) : second(value) {
	}

	int second;
};

struct SonOfFirst : BaseFirst
{
};

struct SonOfFirstSecond : BaseFirst, BaseSecond
{
};

TEST_CASE("InheritanceRepo, basic")
{
	metapp::InheritanceRepo inheritanceRepo;
	auto * repo = &inheritanceRepo;
	repo->addBase<SonOfFirst, BaseFirst>();
	repo->addBase<SonOfFirstSecond, BaseFirst, BaseSecond>();

	REQUIRE(repo->getBaseCount(metapp::getMetaType<SonOfFirst>()) == 1);
	REQUIRE(repo->getBaseCount(metapp::getMetaType<const SonOfFirst>()) == 1);
	REQUIRE(repo->getBaseMetaType(metapp::getMetaType<SonOfFirst>(), 0) == metapp::getMetaType<BaseFirst>());
	REQUIRE(repo->getBaseMetaType(metapp::getMetaType<const volatile SonOfFirst>(), 0) == metapp::getMetaType<BaseFirst>());

	REQUIRE(repo->getBaseCount(metapp::getMetaType<SonOfFirstSecond>()) == 2);
	REQUIRE(repo->getBaseMetaType(metapp::getMetaType<SonOfFirstSecond>(), 0) == metapp::getMetaType<BaseFirst>());
	REQUIRE(repo->getBaseMetaType(metapp::getMetaType<SonOfFirstSecond>(), 1) == metapp::getMetaType<BaseSecond>());

	REQUIRE(repo->getDerivedCount(metapp::getMetaType<SonOfFirst>()) == 0);
	REQUIRE(repo->getDerivedCount(metapp::getMetaType<BaseFirst>()) == 2);
	REQUIRE(repo->getDerivedMetaType(metapp::getMetaType<BaseFirst>(), 0) == metapp::getMetaType<SonOfFirst>());
	REQUIRE(repo->getDerivedMetaType(metapp::getMetaType<BaseFirst>(), 1) == metapp::getMetaType<SonOfFirstSecond>());
}

TEST_CASE("InheritanceRepo, cast")
{
	metapp::InheritanceRepo inheritanceRepo;
	auto * repo = &inheritanceRepo;
	repo->addBase<SonOfFirst, BaseFirst>();
	repo->addBase<SonOfFirstSecond, BaseFirst, BaseSecond>();

	SECTION("nullptr") {
		REQUIRE(repo->castToBase(metapp::getMetaType<SonOfFirst>(), nullptr, 0) == nullptr);
		REQUIRE(repo->castToDerived(metapp::getMetaType<BaseFirst>(), nullptr, 0) == nullptr);
	}

	SECTION("single inheritance") {
		SonOfFirst obj;
		SonOfFirst * pObj = &obj;
		BaseFirst * pBase = pObj;
		REQUIRE(repo->castToBase(metapp::getMetaType<SonOfFirst>(), pObj, 0) == pBase);
		REQUIRE(repo->castToDerived(metapp::getMetaType<BaseFirst>(), pBase, 0) == pObj);
	}

	SECTION("multiple inheritance") {
		SonOfFirstSecond obj;
		SonOfFirstSecond * pObj = &obj;
		BaseFirst * pBaseFirst = pObj;
		BaseSecond * pBaseSecond = pObj;
		
		REQUIRE((void *)pBaseFirst != (void *)pBaseSecond);

		REQUIRE(repo->castToBase(metapp::getMetaType<SonOfFirstSecond>(), pObj, 0) == pBaseFirst);
		REQUIRE(repo->castToBase(metapp::getMetaType<SonOfFirstSecond>(), pObj, 1) == pBaseSecond);
		REQUIRE(repo->castToDerived(metapp::getMetaType<BaseFirst>(), pBaseFirst, 0) == pObj);
		REQUIRE(repo->castToDerived(metapp::getMetaType<BaseSecond>(), pBaseSecond, 0) == pObj);
	}
}


} // namespace
