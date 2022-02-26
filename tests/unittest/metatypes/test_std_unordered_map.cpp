#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, std::unordered_map<int, std::string>")
{
	using Type = std::unordered_map<int, std::string>;
	Type container { { 1, "perfect" } };
	metapp::Variant v(container);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdUnorderedMap);
	REQUIRE(v.get<Type &>()[1] == "perfect");
	v.get<Type &>().insert(std::make_pair(2, "good"));
	REQUIRE(v.get<Type &>()[2] == "good");
	REQUIRE(v.getMetaType()->getUpTypeCount() == 2);
	REQUIRE(v.getMetaType()->getUpType(0)->getTypeKind() == metapp::tkInt);
	REQUIRE(v.getMetaType()->getUpType(1)->getTypeKind() == metapp::tkStdString);
}

TEST_CASE("metatypes, std::unordered_multimap<int, std::string>")
{
	using Type = std::unordered_multimap<int, std::string>;
	Type container { { 1, "perfect" } };
	metapp::Variant v(container);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdUnorderedMultimap);
	REQUIRE(v.get<Type &>().find(1)->second == "perfect");
	v.get<Type &>().insert(std::make_pair(2, "good"));
	REQUIRE(v.get<Type &>().find(2)->second == "good");
	REQUIRE(v.getMetaType()->getUpTypeCount() == 2);
	REQUIRE(v.getMetaType()->getUpType(0)->getTypeKind() == metapp::tkInt);
	REQUIRE(v.getMetaType()->getUpType(1)->getTypeKind() == metapp::tkStdString);
}

