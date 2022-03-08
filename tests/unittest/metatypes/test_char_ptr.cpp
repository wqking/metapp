#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, tkCharPtr, const char *")
{
	REQUIRE(metapp::getMetaType<const char *>()->getTypeKind() == metapp::tkCharPtr);
	const char * s = "hello";
	metapp::Variant v(s);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkCharPtr);

	SECTION("get as char *") {
		REQUIRE(v.get<const char *>() == std::string("hello"));
		REQUIRE(v.get<char *>() == std::string("hello"));
		REQUIRE((const char *)v.get<const volatile char *>() == std::string("hello"));
		REQUIRE(v.get<char []>() == std::string("hello"));
		REQUIRE(v.get<const char []>() == std::string("hello"));
	}

	SECTION("cast to std::string") {
		REQUIRE(v.cast<std::string>().get<const std::string &>() == "hello");
		REQUIRE(v.cast<const std::string &>().get<const std::string &>() == "hello");
		REQUIRE(v.cast<std::string &>().get<const std::string &>() == "hello");
	}
}

TEST_CASE("metatypes, tkCharPtr, char[6]")
{
	REQUIRE(metapp::getMetaType<const char[]>()->getTypeKind() == metapp::tkCharPtr);
	REQUIRE(metapp::getMetaType<const char[6]>()->getTypeKind() == metapp::tkCharPtr);
	
	REQUIRE(metapp::getMetaType<char[]>()->getTypeKind() == metapp::tkCharPtr);
	REQUIRE(metapp::getMetaType<char[6]>()->getTypeKind() == metapp::tkCharPtr);
	char s[6] = "hello";
	metapp::Variant v(metapp::Variant::create<char[6]>(s));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkCharPtr);

	SECTION("get as char *") {
		REQUIRE(v.get<const char *>() == std::string("hello"));
		REQUIRE(v.get<char *>() == std::string("hello"));
		REQUIRE((const char *)v.get<const volatile char *>() == std::string("hello"));
		REQUIRE(v.get<char []>() == std::string("hello"));
		REQUIRE(v.get<char [6]>() == std::string("hello"));
		REQUIRE(v.get<const char []>() == std::string("hello"));
	}

	SECTION("cast to std::string") {
		REQUIRE(v.canCast<std::string>());
		REQUIRE(v.cast<std::string>().get<const std::string &>() == "hello");
	}
}

