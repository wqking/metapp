#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, std::set<int>")
{
	using Type = std::set<int>;
	Type container { 3, 2, 5 };
	metapp::Variant v(container);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdSet);
	REQUIRE(*v.get<Type &>().find(3) == 3);
	v.get<Type &>().insert(6);
	REQUIRE(*v.get<Type &>().find(6) == 6);
	REQUIRE(v.getMetaType()->getUpTypeCount() == 1);
	REQUIRE(v.getMetaType()->getUpType()->getTypeKind() == metapp::tkInt);
}

TEST_CASE("metatypes, std::multiset<int>")
{
	using Type = std::multiset<int>;
	Type container { 3, 2, 5 };
	metapp::Variant v(container);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdMultiset);
	REQUIRE(*v.get<Type &>().find(3) == 3);
	v.get<Type &>().insert(6);
	REQUIRE(*v.get<Type &>().find(6) == 6);
	REQUIRE(v.getMetaType()->getUpTypeCount() == 1);
	REQUIRE(v.getMetaType()->getUpType()->getTypeKind() == metapp::tkInt);
}

