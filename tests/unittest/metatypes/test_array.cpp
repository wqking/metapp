#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, tkArray, int[3]")
{
	int array[] = { 3, 8, 9 };
	metapp::Variant v(metapp::Variant::create<int[3]>(array));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkArray);
	REQUIRE(v.canGet<int[]>());
	REQUIRE(v.canGet<int[3]>());
	REQUIRE(v.get<int [3]>()[2] == 9);
	REQUIRE(v.get<int []>()[0] == 3);
	REQUIRE(v.get<int(&)[]>()[1] == 8);
	REQUIRE(v.get<int(&)[3]>()[2] == 9);

	auto metaIndexable = v.getMetaType()->getMetaIndexable();
	REQUIRE(metaIndexable != nullptr);
	REQUIRE(metaIndexable->getSize(v) == 3);
	REQUIRE(metaIndexable->get(v, 0).get<int>() == 3);
	REQUIRE(metaIndexable->get(v, 1).get<int>() == 8);
	REQUIRE(metaIndexable->get(v, 2).get<int>() == 9);

	REQUIRE_THROWS(metaIndexable->set(v, 1, "abc"));
	metaIndexable->set(v, 0, 5);
	metaIndexable->set(v, 1, 6);
	metaIndexable->set(v, 2, 7);
	REQUIRE(array[0] == 3);
	REQUIRE(array[1] == 8);
	REQUIRE(array[2] == 9);
}

TEST_CASE("metatypes, tkArray, int (&)[3]")
{
	int array[] = { 3, 8, 9 };
	metapp::Variant v(metapp::Variant::create<int (&)[3]>(array));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
	REQUIRE(v.getMetaType()->getUpType()->getTypeKind() == metapp::tkArray);
	REQUIRE(v.canGet<int[]>());
	REQUIRE(v.canGet<int[3]>());
	REQUIRE(v.get<int [3]>()[2] == 9);
	REQUIRE(v.get<int []>()[0] == 3);
	REQUIRE(v.get<int(&)[]>()[1] == 8);
	REQUIRE(v.get<int(&)[3]>()[2] == 9);

	auto metaIndexable = v.getMetaType()->getUpType()->getMetaIndexable();
	REQUIRE(metaIndexable != nullptr);
	REQUIRE(metaIndexable->getSize(v) == 3);
	REQUIRE(metaIndexable->get(v, 0).get<int>() == 3);
	REQUIRE(metaIndexable->get(v, 1).get<int>() == 8);
	REQUIRE(metaIndexable->get(v, 2).get<int>() == 9);

	REQUIRE_THROWS(metaIndexable->set(v, 1, "abc"));
	metaIndexable->set(v, 0, 5);
	metaIndexable->set(v, 1, 6);
	metaIndexable->set(v, 2, 7);
	REQUIRE(array[0] == 5);
	REQUIRE(array[1] == 6);
	REQUIRE(array[2] == 7);
}

TEST_CASE("metatypes, tkArray, std::string[3]")
{
	std::string array[3] = { "good", "great", "perfect" };
	metapp::Variant v(metapp::Variant::create<std::string[3]>(array));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkArray);
	REQUIRE(v.get<std::string[]>()[0] == "good");
	REQUIRE(v.get<std::string[]>()[1] == "great");
	REQUIRE(v.get<std::string[3]>()[2] == "perfect");

	auto metaIndexable = v.getMetaType()->getMetaIndexable();
	REQUIRE(metaIndexable != nullptr);
	REQUIRE(metaIndexable->getSize(v) == 3);
	REQUIRE(metaIndexable->get(v, 0).get<const std::string &>() == "good");
	REQUIRE(metaIndexable->get(v, 1).get<const std::string &>() == "great");
	REQUIRE(metaIndexable->get(v, 2).get<const std::string &>() == "perfect");

	REQUIRE_THROWS(metaIndexable->set(v, 1, 5));
	REQUIRE_THROWS(metaIndexable->set(v, 18, "abc"));
	metaIndexable->set(v, 0, "def");
	REQUIRE(metaIndexable->get(v, 0).get<const std::string &>() == "def");
	REQUIRE(array[0] == "good");
}

TEST_CASE("metatypes, tkArray, int[], constness")
{
	REQUIRE(metapp::getMetaType<const int[]>()->getTypeKind() == metapp::tkArray);
	REQUIRE(metapp::getMetaType<const int[]>()->isConst());
	
	REQUIRE(metapp::getMetaType<volatile int[]>()->getTypeKind() == metapp::tkArray);
	REQUIRE(metapp::getMetaType<volatile int[]>()->isVolatile());
	
	REQUIRE(metapp::getMetaType<const volatile int[]>()->getTypeKind() == metapp::tkArray);
	REQUIRE(metapp::getMetaType<const volatile int[]>()->isConst());
	REQUIRE(metapp::getMetaType<const volatile int[]>()->isVolatile());
}

TEST_CASE("metatypes, tkArray, const std::string[3], can't set to const array")
{
	const std::string array[3] = { "good", "great", "perfect" };
	metapp::Variant v(metapp::Variant::create<const std::string[3]>(array));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkArray);
	REQUIRE(v.getMetaType()->isConst());

	REQUIRE(v.getMetaType()->getMetaIndexable() != nullptr);

	REQUIRE_THROWS_AS(v.getMetaType()->getMetaIndexable()->set(v, 0, "abc"), metapp::UnwritableException);
}

TEST_CASE("metatypes, tkArray, int[2][3]")
{
	int array[2][3] = { { 3, 8, 9 }, { 5, 12, 21 } };
	metapp::Variant v(metapp::Variant::create<int[2][3]>(array));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkArray);
	REQUIRE(v.get<int[][3]>()[0][0] == 3);
	REQUIRE(v.get<int[][3]>()[0][1] == 8);
	REQUIRE(v.get<int[][3]>()[0][2] == 9);
	REQUIRE(v.get<int[][3]>()[1][0] == 5);
	REQUIRE(v.get<int[][3]>()[1][1] == 12);
	REQUIRE(v.get<int[][3]>()[1][2] == 21);

	auto metaIndexable = v.getMetaType()->getMetaIndexable();
	auto upMetaIndexable = v.getMetaType()->getUpType()->getMetaIndexable();
	REQUIRE(metaIndexable != nullptr);
	REQUIRE(upMetaIndexable != nullptr);
	REQUIRE(metaIndexable->getSize(v) == 2);
	REQUIRE(upMetaIndexable->getSize(v) == 3);
	metapp::Variant v1 = metaIndexable->get(v, 1);
	REQUIRE(metapp::getTypeKind(v1) == metapp::tkReference);
	auto v1UpMetaIndexable = v.getMetaType()->getUpType()->getMetaIndexable();
	REQUIRE(v1UpMetaIndexable->get(v1, 0).get<int>() == 5);
	REQUIRE(v1UpMetaIndexable->get(v1, 1).get<int>() == 12);
	REQUIRE(v1UpMetaIndexable->get(v1, 2).get<int>() == 21);

	REQUIRE_THROWS(v1UpMetaIndexable->set(v, 1, "abc"));
	v1UpMetaIndexable->set(v1, 0, 6);
	v1UpMetaIndexable->set(v1, 1, 7);
	v1UpMetaIndexable->set(v1, 2, 8);
	REQUIRE(v1UpMetaIndexable->get(v1, 0).get<int>() == 6);
	REQUIRE(v1UpMetaIndexable->get(v1, 1).get<int>() == 7);
	REQUIRE(v1UpMetaIndexable->get(v1, 2).get<int>() == 8);
}
