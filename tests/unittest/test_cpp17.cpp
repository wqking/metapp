#include "test.h"

#include "metapp/compiler.h"

#if METAPP_SUPPORT_STANDARD_17

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>

TEST_CASE("Any")
{
	{
		metapp::Variant v(true);
		REQUIRE(v.get<bool>() == true);
		v.cast<bool>();
		REQUIRE(v.cast<bool>() == true);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkBool);
	}
	{
		metapp::Variant v((char)38);
		REQUIRE(v.get<char>() == 38);
		v.cast<bool>();
		REQUIRE(v.cast<bool>() == true);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkChar);
	}
	{
		int n = 5;
		int * p = &n;
		metapp::Variant v(p);
		REQUIRE(v.get<int *>() == p);
		REQUIRE(*v.get<int *>() == 5);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
		REQUIRE(v.getMetaType()->getUpType()->getTypeKind() == metapp::tkInt);
	}
	{
		int n = 5;
		int & p = n;
		metapp::Variant v;
		v.set<int &>(p);
		REQUIRE(v.get<int &>() == 5);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
	}
	{
		metapp::Variant v("abc");
		REQUIRE(v.get<const char *>() == std::string("abc"));
		REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	}
	{
		char s[] = "abc";
		metapp::Variant v;
		v.set<char[]>(s);
		REQUIRE(v.get<const char *>() == std::string("abc"));
		REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	}

	{
		std::string s("abc");
		metapp::Variant v(s);
		REQUIRE(v.get<std::string>() == s);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkString);
	}

	{
		std::string s("abc");
		metapp::Variant v;
		v.set<const std::string>(s);
		REQUIRE(v.get<std::string>() == s);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkString);
	}

	{
		std::wstring s(L"abc");
		metapp::Variant v(s);
		REQUIRE(v.get<std::wstring>() == s);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkWideString);
	}

}

#endif
