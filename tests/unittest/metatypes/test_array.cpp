#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, int[]")
{
	int array[] = { 3, 8, 9 };
	metapp::Variant v(metapp::Variant::create<int[]>(array));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkArray);
	REQUIRE(v.get<int[]>()[0] == 3);
	REQUIRE(v.get<int[]>()[1] == 8);
	REQUIRE(v.get<int[3]>()[2] == 9);

	REQUIRE(v.getMetaType()->getMetaIndexable() != nullptr);
	REQUIRE(v.getMetaType()->getMetaIndexable()->getSize(v) == metapp::MetaIndexable::unknowSize);
	REQUIRE(v.getMetaType()->getMetaIndexable()->get(v, 0).get<int>() == 3);
	REQUIRE(v.getMetaType()->getMetaIndexable()->get(v, 1).get<int>() == 8);
	REQUIRE(v.getMetaType()->getMetaIndexable()->get(v, 2).get<int>() == 9);
}

TEST_CASE("metatypes, std::string[3]")
{
	std::string array[3] = { "good", "great", "perfect" };
	metapp::Variant v(metapp::Variant::create<std::string[3]>(array));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkArray);
	REQUIRE(v.get<std::string[]>()[0] == "good");
	REQUIRE(v.get<std::string[]>()[1] == "great");
	REQUIRE(v.get<std::string[3]>()[2] == "perfect");

	REQUIRE(v.getMetaType()->getMetaIndexable() != nullptr);
	REQUIRE(v.getMetaType()->getMetaIndexable()->getSize(v) == 3);
	REQUIRE(v.getMetaType()->getMetaIndexable()->get(v, 0).get<const std::string &>() == "good");
	REQUIRE(v.getMetaType()->getMetaIndexable()->get(v, 1).get<const std::string &>() == "great");
	REQUIRE(v.getMetaType()->getMetaIndexable()->get(v, 2).get<const std::string &>() == "perfect");
}

TEST_CASE("metatypes, int[], constness")
{
	REQUIRE(metapp::getMetaType<const int[]>()->getTypeKind() == metapp::tkArray);
	REQUIRE(metapp::getMetaType<const int[]>()->isConst());
	
	REQUIRE(metapp::getMetaType<volatile int[]>()->getTypeKind() == metapp::tkArray);
	REQUIRE(metapp::getMetaType<volatile int[]>()->isVolatile());
	
	REQUIRE(metapp::getMetaType<const volatile int[]>()->getTypeKind() == metapp::tkArray);
	REQUIRE(metapp::getMetaType<const volatile int[]>()->isConst());
	REQUIRE(metapp::getMetaType<const volatile int[]>()->isVolatile());
}

