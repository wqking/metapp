#include "test.h"

#include <iostream>
#include "varpp/variant.h"

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
		REQUIRE(v.getVarType() == varpp::vtBool);
	}
	{
		varpp::Variant v((char)38);
		REQUIRE(v.get<char>() == 38);
		v.cast<bool>();
		REQUIRE(v.cast<bool>() == true);
		REQUIRE(v.getVarType() == varpp::vtChar);
	}
	{
		int n = 5;
		int * p = &n;
		varpp::Variant v(p);
		REQUIRE(v.get<int *>() == p);
		REQUIRE(*v.get<int *>() == 5);
		REQUIRE(v.getVarType() == varpp::vtPointer);
		REQUIRE(v.getMetaType()->getUnderlying()->getVarType() == varpp::vtInt);
	}
	{
		int n = 5;
		int & p = n;
		varpp::Variant v;
		v.set<int &>(p);
		REQUIRE(v.get<int &>() == 5);
		REQUIRE(v.getVarType() == varpp::vtReference);
	}
	{
		varpp::Variant v("abc");
		REQUIRE(v.get<const char *>() == std::string("abc"));
		REQUIRE(v.getVarType() == varpp::vtPointer);
	}
	{
		char s[] = "abc";
		varpp::Variant v;
		v.set<char[]>(s);
		REQUIRE(v.get<const char *>() == std::string("abc"));
		REQUIRE(v.getVarType() == varpp::vtPointer);
	}

	{
		std::string s("abc");
		varpp::Variant v(s);
		REQUIRE(v.get<std::string>() == s);
		REQUIRE(v.getVarType() == varpp::vtString);
	}

	{
		std::string s("abc");
		varpp::Variant v;
		v.set<const std::string>(s);
		REQUIRE(v.get<std::string>() == s);
		REQUIRE(v.getVarType() == varpp::vtString);
	}

	{
		std::wstring s(L"abc");
		varpp::Variant v(s);
		REQUIRE(v.get<std::wstring>() == s);
		REQUIRE(v.getVarType() == varpp::vtWideString);
	}

}

} // namespace
