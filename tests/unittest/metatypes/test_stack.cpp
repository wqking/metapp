#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, std::stack<std::string>")
{
	using Type = std::stack<std::string>;
	Type container;
	metapp::Variant v(container);
	v.get<Type &>().push("perfect");
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStack);
	REQUIRE(v.get<Type &>().top() == "perfect");
	v.get<Type &>().push("good");
	REQUIRE(v.get<Type &>().top() == "good");
	v.get<Type &>().pop();
	REQUIRE(v.get<Type &>().top() == "perfect");
	using namespace metapp;
	REQUIRE(metapp::matchUpTypeKinds(v.getMetaType(), { tkStack, tkString }));
}

