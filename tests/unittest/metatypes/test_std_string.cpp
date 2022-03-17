// metapp library
// 
// Copyright (C) 2022 Wang Qi (wqking)
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

#include <cstring>

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

TEST_CASE("metatypes, std::string, cast to char *")
{
	std::string s("abcdef");
	metapp::Variant v(s);
	REQUIRE(v.canCast<const char *>());
	REQUIRE(v.canCast<char *>());
	REQUIRE(strcmp(v.cast<const char *>().get<const char *>(), s.c_str()) == 0);
	REQUIRE(strcmp(v.cast<char *>().get<char *>(), s.c_str()) == 0);
}

TEST_CASE("metatypes, std::string, cast to char * &")
{
	std::string s("abcdef");
	metapp::Variant v(s);
	REQUIRE(v.canCast<const char * &>());
	REQUIRE(v.canCast<char * &>());
	REQUIRE(strcmp(v.cast<const char * &>().get<const char * &>(), s.c_str()) == 0);
	// intended get char *, not get char * &
	REQUIRE(strcmp(v.cast<char * &>().get<char *>(), s.c_str()) == 0);
}

TEST_CASE("metatypes, std::wstring, cast to wchar_t *")
{
	std::wstring ws(L"abcdef");
	metapp::Variant v(ws);
	REQUIRE(! v.canCast<const char *>());
	REQUIRE(v.canCast<const wchar_t *>());
	REQUIRE(v.canCast<wchar_t *>());
	REQUIRE(wcscmp(v.cast<const wchar_t *>().get<const wchar_t *>(), ws.c_str()) == 0);
	REQUIRE(wcscmp(v.cast<wchar_t *>().get<wchar_t *>(), ws.c_str()) == 0);
}
