#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, std::unique_ptr<int>")
{
	using PTR = std::unique_ptr<int>;
	PTR up = PTR(new int{38});
	metapp::Variant v(metapp::Variant::create<PTR>(std::move(up)));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdUniquePtr);
	REQUIRE(v.canGet<PTR &>());
	REQUIRE(*(v.get<PTR &>()) == 38);
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkStdUniquePtr, tkInt }));
}

