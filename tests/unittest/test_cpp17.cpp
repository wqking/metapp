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
		REQUIRE(v.getTypeKind() == metapp::tkBool);
	}
	{
		metapp::Variant v((char)38);
		REQUIRE(v.get<char>() == 38);
		v.cast<bool>();
		REQUIRE(v.cast<bool>() == true);
		REQUIRE(v.getTypeKind() == metapp::tkChar);
	}
	{
		int n = 5;
		int * p = &n;
		metapp::Variant v(p);
		REQUIRE(v.get<int *>() == p);
		REQUIRE(*v.get<int *>() == 5);
		REQUIRE(v.getTypeKind() == metapp::tkPointer);
		REQUIRE(v.getMetaType()->getUpType()->getTypeKind() == metapp::tkInt);
	}
	{
		int n = 5;
		int & p = n;
		metapp::Variant v;
		v.set<int &>(p);
		REQUIRE(v.get<int &>() == 5);
		REQUIRE(v.getTypeKind() == metapp::tkReference);
	}
	{
		metapp::Variant v("abc");
		REQUIRE(v.get<const char *>() == std::string("abc"));
		REQUIRE(v.getTypeKind() == metapp::tkPointer);
	}
	{
		char s[] = "abc";
		metapp::Variant v;
		v.set<char[]>(s);
		REQUIRE(v.get<const char *>() == std::string("abc"));
		REQUIRE(v.getTypeKind() == metapp::tkPointer);
	}

	{
		std::string s("abc");
		metapp::Variant v(s);
		REQUIRE(v.get<std::string>() == s);
		REQUIRE(v.getTypeKind() == metapp::tkString);
	}

	{
		std::string s("abc");
		metapp::Variant v;
		v.set<const std::string>(s);
		REQUIRE(v.get<std::string>() == s);
		REQUIRE(v.getTypeKind() == metapp::tkString);
	}

	{
		std::wstring s(L"abc");
		metapp::Variant v(s);
		REQUIRE(v.get<std::wstring>() == s);
		REQUIRE(v.getTypeKind() == metapp::tkWideString);
	}

}

#endif
