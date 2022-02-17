#include "test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

std::string myFunc(const int a, const std::string & b)
{
	std::cout << a << " " << b << std::endl;
	return b + std::to_string(a);
}

TEST_CASE("play camp")
{
#if 0
	using namespace metapp;
	Variant v(&myFunc);
	Variant arguments[] = { 5, "hello"};
	using MT = DeclareMetaType<decltype(&myFunc)>;
	MT::invoke(v, nullptr, nullptr, arguments);
#endif
}

TEST_CASE("aaa")
{
	{
		metapp::Variant v(true);
		REQUIRE(v.get<bool>() == true);
		v.cast<bool>();
		REQUIRE(v.cast<bool>().get<bool>() == true);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkBool);
	}
	{
		metapp::Variant v((char)38);
		REQUIRE(v.get<char>() == 38);
		v.cast<bool>();
		REQUIRE(v.cast<bool>().get<bool>() == true);
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

} // namespace
