#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, std::deque<std::string>")
{
	using Type = std::deque<std::string>;
	Type container { "perfect" };
	metapp::Variant v(container);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdDeque);
	REQUIRE(v.get<Type>()[0] == "perfect");
	v.get<Type &>().push_back("good");
	REQUIRE(v.get<Type &>().back() == "good");
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkStdDeque, tkStdString }));
}

TEST_CASE("metatypes, std::deque<std::string>, MetaIndexable")
{
	metapp::Variant v(std::deque<std::string> {
		"good", "great", "perfect"
	});
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdDeque);
	REQUIRE(v.getMetaType()->getMetaIndexable() != nullptr);
	REQUIRE(v.getMetaType()->getMetaIndexable()->getSize(v) == 3);
	REQUIRE(v.getMetaType()->getMetaIndexable()->getAt(v, 0).get<const std::string &>() == "good");
	REQUIRE(v.getMetaType()->getMetaIndexable()->getAt(v, 1).get<const std::string &>() == "great");
	REQUIRE(v.getMetaType()->getMetaIndexable()->getAt(v, 2).get<const std::string &>() == "perfect");
}

