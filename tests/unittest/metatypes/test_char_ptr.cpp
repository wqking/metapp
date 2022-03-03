#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, const char *")
{
	REQUIRE(metapp::getMetaType<const char *>()->getTypeKind() == metapp::tkCharPtr);
	const char * s = "hello";
	metapp::Variant v(s);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkCharPtr);
	REQUIRE(v.cast<std::string>().get<const std::string &>() == s);
	REQUIRE(v.cast<const std::string &>().get<const std::string &>() == s);
	REQUIRE(v.cast<std::string &>().get<const std::string &>() == s);
}

TEST_CASE("metatypes, char[6]")
{
	REQUIRE(metapp::getMetaType<const char[]>()->getTypeKind() == metapp::tkCharPtr);
	REQUIRE(metapp::getMetaType<const char[6]>()->getTypeKind() == metapp::tkCharPtr);
	
	REQUIRE(metapp::getMetaType<char[]>()->getTypeKind() == metapp::tkCharPtr);
	REQUIRE(metapp::getMetaType<char[6]>()->getTypeKind() == metapp::tkCharPtr);
	char s[6] = "hello";
	metapp::Variant v(metapp::Variant::create<char[6]>(s));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkCharPtr);
	REQUIRE(v.cast<std::string>().get<const std::string &>() == s);
}

