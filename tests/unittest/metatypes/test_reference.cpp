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
	REQUIRE(v.canGet<int>());
	REQUIRE(v.get<int>() == 5);
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkReference, tkInt }));
}

