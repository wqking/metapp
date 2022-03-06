#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

namespace {

struct Class1
{
	Class1() : data(), value(), constValue(5) {}

	const void * func(int, const std::vector<int> &) { return nullptr; }
	const std::array<int, 5> * data;
	int value;
	const int constValue;
};

} // namespace

namespace metapp {
template <>
struct DeclareMetaType <Class1> : public DeclareMetaTypeBase<Class1>
{
	static constexpr TypeKind typeKind = 2000;
};
} // namespace metapp

namespace {

TEST_CASE("metatypes, member data, types")
{
	metapp::Variant v(&Class1::data);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkMemberPointer);
	
	using namespace metapp;
	auto metaType = v.getMetaType();
	REQUIRE(matchUpTypeKinds(metaType->getUpType(0), { 2000 }));
	REQUIRE(matchUpTypeKinds(metaType->getUpType(1), { tkPointer, tkStdArray, tkInt }));
}

TEST_CASE("metatypes, member data, MetaAccessible, non-const")
{
	metapp::Variant v(&Class1::value);
	Class1 obj;
	auto metaType = v.getMetaType();
	auto metaAccessible = metaType->getMetaAccessible();
	REQUIRE(metaAccessible != nullptr);
	REQUIRE(metaAccessible->get(v, &obj).get<int>() == 0);
	obj.value = 5;
	REQUIRE(metaAccessible->get(v, &obj).get<int>() == 5);
	metaAccessible->set(v, &obj, 38);
	REQUIRE(obj.value == 38);
	REQUIRE(metaAccessible->get(v, &obj).get<int>() == 38);
}

TEST_CASE("metatypes, member data, MetaAccessible, const")
{
	metapp::Variant v(&Class1::constValue);
	Class1 obj;
	auto metaType = v.getMetaType();
	auto metaAccessible = metaType->getMetaAccessible();
	REQUIRE(metaAccessible != nullptr);
	REQUIRE(metaAccessible->get(v, &obj).get<int>() == 5);
	REQUIRE_THROWS(metaAccessible->set(v, &obj, 38));
	REQUIRE(metaAccessible->get(v, &obj).get<int>() == 5);
}



} // namespace