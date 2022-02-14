#include "test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <sstream>

namespace {

TEST_CASE("Input stream")
{
}

TEST_CASE("Output stream")
{
	std::stringstream ss;
	SECTION("bool") {
		metapp::Variant v(false);
		ss << std::boolalpha;
		ss << v;
		REQUIRE(ss.str() == "false");
	}
}


} // namespace
