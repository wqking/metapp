#include "../test.h"

#include "metapp/compiler.h"

#if METAPP_SUPPORT_STANDARD_17

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, std::any")
{
	std::any var;
	var = std::string("perfect");
	metapp::Variant v(var);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdAny);
	REQUIRE(std::any_cast<std::string &>(v.get<std::any &>()) == "perfect");

	var = 5;
	v = var;
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdAny);
	REQUIRE(std::any_cast<int>(v.get<std::any &>()) == 5);
}

#endif
