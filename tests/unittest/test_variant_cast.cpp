#include "test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <cstring>

struct MyClass
{
	int value;
};

TEST_CASE("Cast object")
{
	MyClass obj{ 38 };
	
	metapp::Variant v(obj);
	REQUIRE(v.canCast<MyClass>());
	REQUIRE(v.canCast<MyClass &>());
	REQUIRE(v.canCast<const MyClass &>());
	REQUIRE(v.canCast<const volatile MyClass &>());
	v.cast<MyClass>().get<MyClass>();
	REQUIRE(v.cast<MyClass>().get<MyClass>().value == 38);
}

TEST_CASE("Cast std::string to char *")
{
	std::string s("abcdef");
	metapp::Variant v(s);
	REQUIRE(v.canCast<const char *>());
	REQUIRE(v.canCast<char *>());
	REQUIRE(strcmp(v.cast<const char *>().get<const char *>(), s.c_str()) == 0);
	REQUIRE(strcmp(v.cast<char *>().get<char *>(), s.c_str()) == 0);
}

TEST_CASE("Cast std::wstring to wchar_t *")
{
	std::wstring ws(L"abcdef");
	metapp::Variant v(ws);
	REQUIRE(! v.canCast<const char *>());
	REQUIRE(v.canCast<const wchar_t *>());
	REQUIRE(v.canCast<wchar_t *>());
	REQUIRE(wcscmp(v.cast<const wchar_t *>().get<const wchar_t *>(), ws.c_str()) == 0);
	REQUIRE(wcscmp(v.cast<wchar_t *>().get<wchar_t *>(), ws.c_str()) == 0);
}

