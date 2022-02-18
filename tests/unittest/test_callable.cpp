#include "test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <cstring>

namespace {

void myFunc(int & a, std::string & b)
{
	a = 15;
	b = "world";
}

std::string myFunc2(const int a, const std::string & b)
{
	return b + std::to_string(a);
}

int myFunc3()
{
	return 38;
}

TEST_CASE("Callable, free function")
{
	{
		metapp::Variant v(&myFunc);
		int a = 0;
		std::string b;
		metapp::Variant arguments[] = { metapp::Variant().set<int &>(a), metapp::Variant().set<std::string &>(b) };
		v.getMetaType()->invoke(nullptr, v, arguments);
		REQUIRE(a == 15);
		REQUIRE(b == "world");
	}
	{
		metapp::Variant v(&myFunc2);
		metapp::Variant arguments[] = { 5, std::string("hello") };
		REQUIRE(v.getMetaType()->invoke(nullptr, v, arguments).get<std::string>() == "hello5");
	}
	{
		metapp::Variant v(&myFunc3);
		metapp::Variant arguments[] = { 5, std::string("hello") };
		REQUIRE(v.getMetaType()->invoke(nullptr, v, nullptr).get<int>() == 38);
	}
}



} // namespace
