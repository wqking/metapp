#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, void")
{
	REQUIRE(metapp::getTypeKind(metapp::Variant((void *)0)) == metapp::tkPointer);
	REQUIRE(metapp::Variant((void *)0).getMetaType()->getUpType()->getName() == std::string("void"));
}

TEST_CASE("metatypes, nullptr")
{
	REQUIRE(metapp::getTypeKind(metapp::Variant(nullptr)) == metapp::tkPointer);
}

