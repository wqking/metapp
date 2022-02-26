#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, std::tuple<std::string, int, void *>")
{
	using Type = std::tuple<std::string, int, long *>;
	long n = 5;
	Type tuple("def", 38, &n);
	metapp::Variant v(tuple);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdTuple);
	REQUIRE(std::get<0>(v.get<Type &>()) == "def");
	REQUIRE(std::get<1>(v.get<Type &>()) == 38);
	REQUIRE(std::get<2>(v.get<Type &>()) == &n);
	auto metaType = v.getMetaType();
	REQUIRE(metaType->getUpType(0)->getTypeKind() == metapp::tkStdString);
	REQUIRE(metaType->getUpType(1)->getTypeKind() == metapp::tkInt);
	REQUIRE(metaType->getUpType(2)->getTypeKind() == metapp::tkPointer);
}

