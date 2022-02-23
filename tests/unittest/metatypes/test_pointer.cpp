#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, void *")
{
	metapp::Variant v((void *)0);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
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

TEST_CASE("metatypes, int **")
{
	metapp::Variant v((int **)0);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	REQUIRE(v.canGet<int **>(true));
	REQUIRE(v.canGet<int **>(false));
	REQUIRE(! v.canGet<int *>(true));
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkPointer, tkPointer, tkInt }));
}

TEST_CASE("metatypes, void ***")
{
	void *** p = nullptr;
	metapp::Variant v(p);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkPointer, tkPointer, tkPointer, tkVoid }));
}

TEST_CASE("metatypes, int *, accessible")
{
	int n = 5;
	metapp::Variant v(&n);
	REQUIRE(v.getMetaType()->accessibleGet(v, nullptr).get<int>() == 5);

	v.getMetaType()->accessibleSet(v, nullptr, 38);
	REQUIRE(n == 38);
	REQUIRE(v.getMetaType()->accessibleGet(v, nullptr).get<int>() == 38);

	v.getMetaType()->accessibleSet(v, nullptr, 98.0);
	REQUIRE(n == 98);
	REQUIRE(v.getMetaType()->accessibleGet(v, nullptr).get<int>() == 98);
}

