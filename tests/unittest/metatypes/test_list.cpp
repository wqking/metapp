#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, std::list<std::string>")
{
	using Type = std::list<std::string>;
	Type list{ "hello", "world" };
	metapp::Variant v(list);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkList);
	REQUIRE(v.get<const Type &>().front() == "hello");
	REQUIRE(v.get<const Type &>().back() == "world");
	v.get<Type &>().push_back("good");
	REQUIRE(v.get<const Type &>().back() == "good");
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkList, tkString }));
}

