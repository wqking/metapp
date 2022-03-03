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

TEST_CASE("metatypes, std::vector<std::string>, MetaIndexable")
{
	metapp::Variant v(std::vector<std::string> {
		"good", "great", "perfect"
	});
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdVector);
	REQUIRE(v.getMetaType()->getMetaIndexable() != nullptr);
	REQUIRE(v.getMetaType()->getMetaIndexable()->getSize(v) == 3);
	REQUIRE(v.getMetaType()->getMetaIndexable()->getAt(v, 0).get<const std::string &>() == "good");
	REQUIRE(v.getMetaType()->getMetaIndexable()->getAt(v, 1).get<const std::string &>() == "great");
	REQUIRE(v.getMetaType()->getMetaIndexable()->getAt(v, 2).get<const std::string &>() == "perfect");
}

