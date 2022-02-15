#include "test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

using namespace metapp;
template <typename Arg0, typename ...Args>
const MetaType ** getXxx()
{
	static std::array<const MetaType *, sizeof...(Args) + 1> xxx {
		getMetaType<Arg0>(),
		getMetaType<Args>()...,
	};
	return xxx.data();
}

TEST_CASE("play camp")
{
	const MetaType * p = getMetaType<const int *>();
	REQUIRE(p->getUpType()->isConst());
	auto xxx = getXxx<char, int>();
	REQUIRE(xxx[0] == getMetaType<char>());
	REQUIRE(xxx[1] == getMetaType<int>());
}

TEST_CASE("aaa")
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

} // namespace
