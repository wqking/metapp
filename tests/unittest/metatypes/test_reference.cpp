#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, int &") {
	int n = 5;
	metapp::Variant v;
	v.set<int &>(n);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
	SECTION("get") {
		REQUIRE(v.canGet<int>());
		REQUIRE(v.get<int>() == 5);
		using namespace metapp;
		REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkReference, tkInt }));
	}
	SECTION("set") {
		REQUIRE(v.canGet<int &>());
		REQUIRE(v.get<int &>() == 5);

		v.get<int &>() = 8;
		REQUIRE(v.get<int &>() == 8);
	}
	SECTION("set &&") {
		REQUIRE(v.canGet<int &&>());
		REQUIRE(v.get<int &&>() == 5);

		v.get<int &>() = 8;
		REQUIRE(v.get<int &&>() == 8);
	}
}

TEST_CASE("metatypes, int &&") {
	int n = 5;
	metapp::Variant v;
	v.set<int &&>((int &&)n);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
	SECTION("get") {
		REQUIRE(v.canGet<int>());
		REQUIRE(v.get<int>() == 5);
		using namespace metapp;
		REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkReference, tkInt }));
	}
	SECTION("set") {
		REQUIRE(v.canGet<int &>());
		REQUIRE(v.get<int &>() == 5);

		v.get<int &>() = 8;
		REQUIRE(v.get<int &>() == 8);
	}
	SECTION("set &&") {
		REQUIRE(v.canGet<int &&>());
		REQUIRE(v.get<int &&>() == 5);

		v.get<int &>() = 8;
		REQUIRE(v.get<int &&>() == 8);
	}
}

TEST_CASE("metatypes, std::reference_wrapper<int>") {
	int n = 5;
	std::reference_wrapper<int> ref = n;
	metapp::Variant v(ref);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
	REQUIRE(v.canGet<int &>());
	REQUIRE(v.get<int &>() == 5);
	REQUIRE(v.canGet<int>());
	REQUIRE(v.get<int>() == 5);
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkReference, tkInt }));
}

