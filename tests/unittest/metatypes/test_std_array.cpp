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
}

