#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, std::forward_list<std::string>")
{
	using Type = std::forward_list<std::string>;
	Type container { "perfect" };
	metapp::Variant v(container);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdForwardList);
	REQUIRE(v.get<Type>().front() == "perfect");
	v.get<Type &>().push_front("good");
	REQUIRE(v.get<Type &>().front() == "good");
	using namespace metapp;
	REQUIRE(matchUpTypeKinds(v.getMetaType(), { tkStdForwardList, tkStdString }));
}

