#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, std::function<int (int)>")
{
	std::function<int (int)> f([](const int n) { return n * 2; });
	metapp::Variant v(f);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdFunction);
	metapp::Variant arguments[] = { 5 };
	REQUIRE(v.getMetaType()->invoke(nullptr, v, arguments, 1).get<int>() == 10);

	using namespace metapp;
	auto metaType = v.getMetaType();
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(0), { tkInt }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(1), { tkInt }));
}

