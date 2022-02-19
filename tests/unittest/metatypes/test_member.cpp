#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

struct Class1
{
	const void * func(int, const std::vector<int> &) { return nullptr; }
	const std::array<int, 5> * data;
};

namespace metapp {
template <>
struct DeclareMetaType <Class1> : public DeclareMetaTypeBase<Class1>
{
	static constexpr TypeKind typeKind = 2000;
};
} // namespace metapp

TEST_CASE("metatypes, member data")
{
	metapp::Variant v(&Class1::data);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkMemberPointer);
	
	using namespace metapp;
	auto metaType = v.getMetaType();
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(0), { 2000 }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(1), { tkPointer, tkStdArray, tkInt }));
}

TEST_CASE("metatypes, member function")
{
	metapp::Variant v(&Class1::func);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkMemberFunction);
	
	using namespace metapp;
	auto metaType = v.getMetaType();
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(0), { 2000 }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(1), { tkPointer, tkVoid }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(2), { tkInt }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(3), { tkReference, tkVector, tkInt }));
}

