#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, tkPointer, void *")
{
	metapp::Variant v((void *)0);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkPointer, tkVoid }));
}

TEST_CASE("metatypes, tkPointer, const volatile void *")
{
	metapp::Variant v((const volatile void *)0);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkPointer, tkVoid }));
	REQUIRE(v.getMetaType()->getUpType()->isConst());
	REQUIRE(v.getMetaType()->getUpType()->isVolatile());
}

TEST_CASE("metatypes, tkPointer, int **")
{
	metapp::Variant v((int **)0);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	REQUIRE(v.canGet<int **>());
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkPointer, tkPointer, tkInt }));
}

TEST_CASE("metatypes, tkPointer, void ***")
{
	void *** p = nullptr;
	metapp::Variant v(p);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkPointer, tkPointer, tkPointer, tkVoid }));
}

