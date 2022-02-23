#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, int[]")
{
	int array[] = { 3, 8, 9 };
	metapp::Variant v;
	v.set<int[]>(array);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkArray);
	REQUIRE(v.getMetaType()->hasMetaArray());
	REQUIRE(v.getMetaType()->getMetaArray()->getLength() == -1);
	REQUIRE(v.get<int[]>()[0] == 3);
	REQUIRE(v.get<int[]>()[1] == 8);
	REQUIRE(v.get<int[3]>()[2] == 9);
}

TEST_CASE("metatypes, int[3]")
{
	int array[3] = { 3, 8, 9 };
	metapp::Variant v;
	v.set<int[3]>(array);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkArray);
	REQUIRE(v.getMetaType()->hasMetaArray());
	REQUIRE(v.getMetaType()->getMetaArray()->getLength() == 3);
	REQUIRE(v.get<int[]>()[0] == 3);
	REQUIRE(v.get<int[]>()[1] == 8);
	REQUIRE(v.get<int[3]>()[2] == 9);
}

