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

TEST_CASE("metatypes, tkPointer, const char *")
{
	REQUIRE(metapp::getMetaType<const char *>()->getTypeKind() == metapp::tkPointer);
	const char * s = "hello";
	metapp::Variant v(s);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);

	SECTION("get as char *") {
		REQUIRE(v.get<const char *>() == std::string("hello"));
		REQUIRE(v.get<char *>() == std::string("hello"));
		REQUIRE((const char *)v.get<const volatile char *>() == std::string("hello"));
	}

	SECTION("cast to std::string") {
		REQUIRE(v.cast<std::string>().get<const std::string &>() == "hello");
		REQUIRE(v.cast<const std::string &>().get<const std::string &>() == "hello");
		REQUIRE(v.cast<std::string &>().get<const std::string &>() == "hello");
	}
}

TEST_CASE("metatypes, tkPointer, char[6]")
{
	REQUIRE(metapp::getMetaType<const char[]>()->getTypeKind() == metapp::tkArray);
	REQUIRE(metapp::getMetaType<const char[6]>()->getTypeKind() == metapp::tkArray);
	
	REQUIRE(metapp::getMetaType<char[]>()->getTypeKind() == metapp::tkArray);
	REQUIRE(metapp::getMetaType<char[6]>()->getTypeKind() == metapp::tkArray);
	char s[6] = "hello";
	metapp::Variant v(metapp::Variant::create<char[6]>(s));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkArray);

	SECTION("get as char []") {
		REQUIRE(v.get<char []>() == std::string("hello"));
		REQUIRE(v.get<char [6]>() == std::string("hello"));
		REQUIRE(v.get<const char []>() == std::string("hello"));
	}

	SECTION("cast to std::string") {
		REQUIRE(v.canCast<std::string>());
		REQUIRE(v.cast<std::string>().get<const std::string &>() == "hello");
	}
}

TEST_CASE("metatypes, tkPointer, const wchar_t *")
{
	REQUIRE(metapp::getMetaType<const wchar_t *>()->getTypeKind() == metapp::tkPointer);
	const wchar_t * s = L"hello";
	metapp::Variant v(s);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);

	SECTION("get as wchar_t *") {
		REQUIRE(v.get<const wchar_t *>() == std::wstring(L"hello"));
		REQUIRE(v.get<wchar_t *>() == std::wstring(L"hello"));
		REQUIRE((const wchar_t *)v.get<const volatile wchar_t *>() == std::wstring(L"hello"));
	}

	SECTION("cast to std::wstring") {
		REQUIRE(v.cast<std::wstring>().get<const std::wstring &>() == L"hello");
		REQUIRE(v.cast<const std::wstring &>().get<const std::wstring &>() == L"hello");
		REQUIRE(v.cast<std::wstring &>().get<const std::wstring &>() == L"hello");
	}
}


