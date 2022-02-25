#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, std::string")
{
	REQUIRE(metapp::getTypeKind(metapp::Variant(std::string("abc"))) == metapp::tkStdString);
	REQUIRE(metapp::Variant(std::string("abc")).get<std::string>() == "abc");
	std::string s("def");
	REQUIRE(metapp::getTypeKind(metapp::Variant(s)) == metapp::tkStdString);
	REQUIRE(metapp::Variant(s).get<std::string>() == s);
}

TEST_CASE("metatypes, std::wstring")
{
	REQUIRE(metapp::getTypeKind(metapp::Variant(std::wstring(L"abc"))) == metapp::tkStdWideString);
	REQUIRE(metapp::Variant(std::wstring(L"abc")).get<std::wstring>() == L"abc");
	std::wstring ws(L"def");
	REQUIRE(metapp::getTypeKind(metapp::Variant(ws)) == metapp::tkStdWideString);
	REQUIRE(metapp::Variant(ws).get<std::wstring>() == ws);
}

