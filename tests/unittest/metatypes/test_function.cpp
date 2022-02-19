#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

namespace {

const void * func1(int, const std::vector<int> &) { return nullptr; }

TEST_CASE("metatypes, function pointer")
{
	metapp::Variant v(&func1);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkFunction);
	REQUIRE(v.get<void *>() == (void *)&func1);

	using namespace metapp;
	auto metaType = v.getMetaType();
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(0), { tkPointer, tkVoid }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(1), { tkInt }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(2), { tkReference, tkVector, tkInt }));
	
	metaType = metapp::getMetaType<const void * (int, const std::vector<int> &)>();
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(0), { tkPointer, tkVoid }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(1), { tkInt }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(2), { tkReference, tkVector, tkInt }));
}


} // namespace
