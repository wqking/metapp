#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, Accessor")
{
	int n = 5;
	metapp::Variant v(metapp::createAccessor<int>(&n, &n));
	REQUIRE(v.getMetaType()->getMetaAccessible()->get(v, nullptr).get<int>() == 5);

	v.getMetaType()->getMetaAccessible()->set(v, nullptr, 38);
	REQUIRE(n == 38);
	REQUIRE(v.getMetaType()->getMetaAccessible()->get(v, nullptr).get<int>() == 38);

	v.getMetaType()->getMetaAccessible()->set(v, nullptr, 98.0);
	REQUIRE(n == 98);
	REQUIRE(v.getMetaType()->getMetaAccessible()->get(v, nullptr).get<int>() == 98);
}

namespace {

struct Class1
{
	const void * func(int, const std::vector<int> &) { return nullptr; }
	const std::array<int, 5> * data;
	int value;
};

} // namespace

TEST_CASE("metatypes, Accessor, member data, int Class1::*")
{
	metapp::Variant v(metapp::createAccessor<int>(&Class1::value, &Class1::value));
	Class1 obj;
	obj.value = 5;
	REQUIRE(v.getMetaType()->getMetaAccessible()->get(v, &obj).get<int>() == 5);

	v.getMetaType()->getMetaAccessible()->set(v, &obj, 38);
	REQUIRE(obj.value == 38);
	REQUIRE(v.getMetaType()->getMetaAccessible()->get(v, &obj).get<int>() == 38);

	v.getMetaType()->getMetaAccessible()->set(v, &obj, 98.0);
	REQUIRE(obj.value == 98);
	REQUIRE(v.getMetaType()->getMetaAccessible()->get(v, &obj).get<int>() == 98);
}

