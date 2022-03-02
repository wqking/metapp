#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, Variant")
{
	auto metaType = metapp::getMetaType<metapp::Variant>();
	REQUIRE(metaType->getTypeKind() == metapp::tkVariant);
	metapp::Variant copyFrom("abc");
	metapp::Variant v(metaType, &copyFrom);
	REQUIRE(! v.canCast<int>());
	REQUIRE(v.canCast<std::string>());
	REQUIRE(v.cast<std::string>().get<std::string>() == "abc");
}

