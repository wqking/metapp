#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, int &") {
	int n = 5;
	metapp::Variant v;
	v.set<int &>(n);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
	REQUIRE(v.canGet<int &>());
	REQUIRE(v.get<int &>() == 5);
	using namespace metapp;
	REQUIRE(metapp::matchUpTypeKinds(v.getMetaType(), { tkReference, tkInt }));
}

TEST_CASE("metatypes, int &, accessible")
{
	int n = 5;
	metapp::Variant v;
	v.set<int &>(n);
	REQUIRE(v.getMetaType()->accessibleGet(v, nullptr).get<int>() == 5);

	v.getMetaType()->accessibleSet(v, nullptr, 38);
	REQUIRE(n == 38);
	REQUIRE(v.getMetaType()->accessibleGet(v, nullptr).get<int>() == 38);

	v.getMetaType()->accessibleSet(v, nullptr, 98.0);
	REQUIRE(n == 98);
	REQUIRE(v.getMetaType()->accessibleGet(v, nullptr).get<int>() == 98);
}

