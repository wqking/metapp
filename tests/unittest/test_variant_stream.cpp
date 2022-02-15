#include "test.h"

#include "metapp/variant.h"
#include "metapp/metatyperepo.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <sstream>

namespace {

TEST_CASE("Input stream")
{
	std::stringstream ss;
	SECTION("bool") {
		metapp::Variant v(metapp::getMetaTypeRepo()->getMetaType(metapp::tkBool));
		REQUIRE(v.get<bool>() == false);
		ss >> std::boolalpha;
		ss.str("true");
		ss >> v;
		REQUIRE(v.get<bool>() == true);
	}

	SECTION("int") {
		metapp::Variant v(metapp::getMetaType<int>());
		REQUIRE(v.get<int>() == 0);
		ss.str("138");
		ss >> v;
		REQUIRE(v.get<int>() == 138);
	}
	
	SECTION("std::string") {
		metapp::Variant v(metapp::getMetaType<std::string>());
		REQUIRE(v.get<std::string>() == "");
		ss.str("abcxyz");
		ss >> v;
		REQUIRE(v.get<std::string>() == "abcxyz");
	}
}

TEST_CASE("Output stream")
{
	std::stringstream ss;
	SECTION("bool") {
		metapp::Variant v(false);
		ss << std::boolalpha;
		ss << v;
		REQUIRE(ss.str() == "false");

		v = true;
		ss << std::boolalpha;
		ss << v;
		REQUIRE(ss.str() == "falsetrue");
	}

	SECTION("char") {
		metapp::Variant v((char)(int)'w');
		ss << v;
		REQUIRE(ss.str() == "w");
	}

	SECTION("float") {
		metapp::Variant v(3.1415f);
		ss << v;
		REQUIRE(ss.str() == "3.1415");
	}

	SECTION("double") {
		metapp::Variant v(3.1415);
		ss << v;
		REQUIRE(ss.str() == "3.1415");
	}

	SECTION("std::string") {
		std::string s("abcdef");
		metapp::Variant v(s);
		ss << v;
		REQUIRE(ss.str() == s);
	}

}


} // namespace