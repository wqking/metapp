#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, std::pair<int, std::string>")
{
	using Type = std::pair<int, std::string>;
	Type container { 1, "perfect" };
	metapp::Variant v(container);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdPair);
	REQUIRE(v.get<Type &>().first == 1);
	REQUIRE(v.get<Type &>().second == "perfect");
	v.get<Type &>().first = 2;
	v.get<Type &>().second = "good";
	REQUIRE(v.get<Type &>().first == 2);
	REQUIRE(v.get<Type &>().second == "good");
	REQUIRE(v.getMetaType()->getUpTypeCount() == 2);
	REQUIRE(v.getMetaType()->getUpType(0)->getTypeKind() == metapp::tkInt);
	REQUIRE(v.getMetaType()->getUpType(1)->getTypeKind() == metapp::tkStdString);
}

