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

TEST_CASE("metatypes, std::tuple, MetaIndexable")
{
	metapp::Variant v(std::make_tuple<std::string, int, long>("hello", 5, 38));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdTuple);
	auto metaType = v.getMetaType();
	auto metaIndexable = metaType->getMetaIndexable();
	REQUIRE(metaIndexable != nullptr);
	REQUIRE(metaIndexable->getSize(v) == 3);
	REQUIRE(metaIndexable->get(v, 0).get<const std::string &>() == "hello");
	REQUIRE(metaIndexable->get(v, 1).get<int>() == 5);
	REQUIRE(metaIndexable->get(v, 2).get<long>() == 38);
}

TEST_CASE("metatypes, std::tuple, MetaIterable")
{
	auto original = std::make_tuple<std::string, int, long>("hello", 5, 38);
	metapp::Variant v(original);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdTuple);
	REQUIRE(v.getMetaType()->getMetaIterable() != nullptr);

	std::vector<metapp::Variant> newVector;
	v.getMetaType()->getMetaIterable()->forEach(v, [&newVector](const metapp::Variant & value) {
		REQUIRE(metapp::getTypeKind(value) == metapp::tkReference);
		newVector.push_back(value);
		return true;
	});
	REQUIRE(std::get<0>(original) == newVector[0].get<const std::string &>());
	REQUIRE(std::get<1>(original) == newVector[1].get<int>());
	REQUIRE(std::get<2>(original) == newVector[2].get<long>());
}
