#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, std::shared_ptr<int>")
{
	std::shared_ptr<int> sp = std::make_shared<int>(38);
	metapp::Variant v(sp);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkSharedPtr);
	REQUIRE(v.canGet<std::shared_ptr<int> >());
	REQUIRE(*(v.get<std::shared_ptr<int> >()) == 38);
	using namespace metapp;
	REQUIRE(metapp::matchUpTypeKinds(v.getMetaType(), { tkSharedPtr, tkInt }));
}

