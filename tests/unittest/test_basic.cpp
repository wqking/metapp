#include "test.h"

#include "varpp/variant.h"

#include <string>
#include <iostream>

struct X
{
	constexpr X() : value() {}
	constexpr X(const int v) : value(v) {}
	constexpr operator int() const {
		return value;
	}
	int value;
};

TEST_CASE("aaa")
{
	constexpr X x = X(2), y = X(1), z = X(2);
	switch(x) {
	case y:
		std::cout << "yyy" << std::endl;
		break;
	case z:
		std::cout << "zzz" << std::endl;
		break;
	}
	{
		varpp::Variant v(true);
		REQUIRE(v.getAs<bool>() == true);
		v.cast<bool>();
		REQUIRE(v.cast<bool>() == true);
		REQUIRE(v.getVarType() == varpp::vtBool);
		REQUIRE(! varpp::isPointer(v));
	}
	{
		varpp::Variant v((char)38);
		REQUIRE(v.getAs<char>() == 38);
		v.cast<bool>();
		REQUIRE(v.cast<bool>() == true);
		REQUIRE(v.getVarType() == varpp::vtChar);
		REQUIRE(! varpp::isPointer(v));
	}
	{
		int n = 5;
		int * p = &n;
		varpp::Variant v(p);
		REQUIRE(v.getAs<int *>() == p);
		REQUIRE(*v.getAs<int *>() == 5);
		REQUIRE(v.getVarType() == varpp::vtInt);
		REQUIRE(varpp::isPointer(v));
	}
	{
		int n = 5;
		int & p = n;
		varpp::Variant v;
		v.set<int &>(p);
		REQUIRE(v.getAs<int &>() == 5);
		REQUIRE(v.getVarType() == varpp::vtInt);
		REQUIRE(varpp::isReference(v));
	}
	{
		varpp::Variant v("abc");
		REQUIRE(v.getAs<const char *>() == std::string("abc"));
		REQUIRE(v.getVarType() == varpp::vtChar);
		REQUIRE(varpp::isPointer(v));
	}
	{
		char s[] = "abc";
		varpp::Variant v;
		v.set<char[]>(s);
		REQUIRE(v.getAs<const char *>() == std::string("abc"));
		REQUIRE(v.getVarType() == varpp::vtChar);
		REQUIRE(varpp::isPointer(v));
	}

	{
		std::string s("abc");
		varpp::Variant v(s);
		REQUIRE(v.getAs<std::string>() == s);
		REQUIRE(v.getVarType() == varpp::vtString);
		REQUIRE(! varpp::isPointer(v));
		REQUIRE(! varpp::isReference(v));
	}

	{
		std::string s("abc");
		varpp::Variant v;
		v.set<const std::string>(s);
		REQUIRE(v.getAs<std::string>() == s);
		REQUIRE(v.getVarType() == varpp::vtString);
		REQUIRE(! varpp::isPointer(v));
		REQUIRE(! varpp::isReference(v));
	}

	{
		std::wstring s(L"abc");
		varpp::Variant v(s);
		REQUIRE(v.getAs<std::wstring>() == s);
		REQUIRE(v.getVarType() == varpp::vtWideString);
		REQUIRE(! varpp::isPointer(v));
		REQUIRE(! varpp::isReference(v));
	}

}
