#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, std::unordered_set<int>")
{
	using Type = std::unordered_set<int>;
	Type container { 3, 2, 5 };
	metapp::Variant v(container);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdUnorderedSet);
	REQUIRE(*v.get<Type &>().find(3) == 3);
	v.get<Type &>().insert(6);
	REQUIRE(*v.get<Type &>().find(6) == 6);
	REQUIRE(v.getMetaType()->getUpTypeCount() == 1);
	REQUIRE(v.getMetaType()->getUpType()->getTypeKind() == metapp::tkInt);
}

TEST_CASE("metatypes, std::unordered_multiset<int>")
{
	using Type = std::unordered_multiset<int>;
	Type container { 3, 2, 5 };
	metapp::Variant v(container);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdUnorderedMultiset);
	REQUIRE(*v.get<Type &>().find(3) == 3);
	v.get<Type &>().insert(6);
	REQUIRE(*v.get<Type &>().find(6) == 6);
	REQUIRE(v.getMetaType()->getUpTypeCount() == 1);
	REQUIRE(v.getMetaType()->getUpType()->getTypeKind() == metapp::tkInt);
}

