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

