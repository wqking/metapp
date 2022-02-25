#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, std::vector<int>")
{
	using Type = std::vector<int>;
	Type vec{ 5 };
	metapp::Variant v(vec);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdVector);
	REQUIRE(v.get<Type>()[0] == 5);
	v.get<Type &>().push_back(10);
	REQUIRE(v.get<Type>().back() == 10);
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkStdVector, tkInt }));
}

