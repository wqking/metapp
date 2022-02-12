#include "test.h"

#include "varpp/variant.h"
#include "varpp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

TEST_CASE("cast, fundamental")
{
	SECTION("bool") {
	}
}

TEST_CASE("aaa")
{
	{
		varpp::Variant v(true);
		REQUIRE(v.get<bool>() == true);
		v.cast<bool>();
		REQUIRE(v.cast<bool>() == true);
		REQUIRE(v.getTypeKind() == varpp::tkBool);
	}
	{
		varpp::Variant v((char)38);
		REQUIRE(v.get<char>() == 38);
		v.cast<bool>();
		REQUIRE(v.cast<bool>() == true);
		REQUIRE(v.getTypeKind() == varpp::tkChar);
	}
	{
		int n = 5;
		int * p = &n;
		varpp::Variant v(p);
		REQUIRE(v.get<int *>() == p);
		REQUIRE(*v.get<int *>() == 5);
		REQUIRE(v.getTypeKind() == varpp::tkPointer);
		REQUIRE(v.getMetaType()->getUpType()->getTypeKind() == varpp::tkInt);
	}
	{
		int n = 5;
		int & p = n;
		varpp::Variant v;
		v.set<int &>(p);
		REQUIRE(v.get<int &>() == 5);
		REQUIRE(v.getTypeKind() == varpp::tkReference);
	}
	{
		varpp::Variant v("abc");
		REQUIRE(v.get<const char *>() == std::string("abc"));
		REQUIRE(v.getTypeKind() == varpp::tkPointer);
	}
	{
		char s[] = "abc";
		varpp::Variant v;
		v.set<char[]>(s);
		REQUIRE(v.get<const char *>() == std::string("abc"));
		REQUIRE(v.getTypeKind() == varpp::tkPointer);
	}

	{
		std::string s("abc");
		varpp::Variant v(s);
		REQUIRE(v.get<std::string>() == s);
		REQUIRE(v.getTypeKind() == varpp::tkString);
	}

	{
		std::string s("abc");
		varpp::Variant v;
		v.set<const std::string>(s);
		REQUIRE(v.get<std::string>() == s);
		REQUIRE(v.getTypeKind() == varpp::tkString);
	}

	{
		std::wstring s(L"abc");
		varpp::Variant v(s);
		REQUIRE(v.get<std::wstring>() == s);
		REQUIRE(v.getTypeKind() == varpp::tkWideString);
	}

}

} // namespace
