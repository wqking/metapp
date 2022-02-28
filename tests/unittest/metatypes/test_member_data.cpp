#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

namespace {

struct Class1
{
	const void * func(int, const std::vector<int> &) { return nullptr; }
	const std::array<int, 5> * data;
	int value;
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

TEST_CASE("metatypes, member data")
{
	metapp::Variant v(&Class1::data);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkMemberPointer);
	
	using namespace metapp;
	auto metaType = v.getMetaType();
	REQUIRE(matchUpTypeKinds(metaType->getUpType(0), { 2000 }));
	REQUIRE(matchUpTypeKinds(metaType->getUpType(1), { tkPointer, tkStdArray, tkInt }));
}



} // namespace