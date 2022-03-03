#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, std::array<int, 5>")
{
	using Type = std::array<int, 5>;
	Type container { 38, 98, 5, 16, 99 };
	metapp::Variant v(container);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdArray);
	REQUIRE(v.get<Type>()[0] == 38);
	REQUIRE(v.get<Type>()[1] == 98);
	REQUIRE(v.get<Type>()[2] == 5);
	REQUIRE(v.get<Type>()[3] == 16);
	REQUIRE(v.get<Type>()[4] == 99);
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkStdArray, tkInt }));

	REQUIRE(v.getMetaType()->getMetaIndexable() != nullptr);
	REQUIRE(v.getMetaType()->getMetaIndexable()->getSize(v) == 5);
	REQUIRE(v.getMetaType()->getMetaIndexable()->getAt(v, 0).get<int>() == 38);
	REQUIRE(v.getMetaType()->getMetaIndexable()->getAt(v, 1).get<int>() == 98);
	REQUIRE(v.getMetaType()->getMetaIndexable()->getAt(v, 2).get<int>() == 5);
	REQUIRE(v.getMetaType()->getMetaIndexable()->getAt(v, 3).get<int>() == 16);
	REQUIRE(v.getMetaType()->getMetaIndexable()->getAt(v, 4).get<int>() == 99);
}

