#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, std::deque<std::string>")
{
	using Type = std::deque<std::string>;
	Type vec{ "perfect" };
	metapp::Variant v(vec);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkDeque);
	REQUIRE(v.get<Type>()[0] == "perfect");
	v.get<Type &>().push_back("good");
	REQUIRE(v.get<Type &>().back() == "good");
	using namespace metapp;
	REQUIRE(metapp::matchUpTypeKinds(v.getMetaType(), { tkDeque, tkString }));
}

