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
		REQUIRE(v.getMetaType()->canInvoke(arguments, 2));
		REQUIRE(! v.getMetaType()->canInvoke(arguments, 1));
		v.getMetaType()->invoke(nullptr, v, arguments, 2);
		REQUIRE(a == 15);
		REQUIRE(b == "world");
	}
	{
		metapp::Variant v(&myFunc2);
		metapp::Variant arguments[] = { 5, std::string("hello") };
		REQUIRE(v.getMetaType()->invoke(nullptr, v, arguments, 2).get<std::string>() == "hello5");
	}
	{
		metapp::Variant v(&myFunc3);
		REQUIRE(v.getMetaType()->invoke(nullptr, v, nullptr, 0).get<int>() == 38);
	}
}


TEST_CASE("Callable, std::function")
{
	{
		std::function<void (int &, std::string &)> func([](int & a, std::string & b) {
			a = 38;
			b = "hello";
		});
		metapp::Variant v(func);
		int a = 0;
		std::string b;
		metapp::Variant arguments[] = { metapp::Variant().set<int &>(a), metapp::Variant().set<std::string &>(b) };
		v.getMetaType()->invoke(nullptr, v, arguments, 2);
		REQUIRE(a == 38);
		REQUIRE(b == "hello");
	}
}

struct Base
{
	Base(const int n = 0) : myValue(n) {
		//printf("===%d\n", n);
	}

	int myValue;

	void func1(int & a, std::string & b)
	{
		a = 3 + myValue;
		b = "Good";
	}

	virtual int add(const int ) const {
		return 0;
	}

};

struct Derived : Base
{
	using Base::Base;

	virtual int add(const int a) const override {
		return a + myValue;
	}
};

TEST_CASE("Callable, member function")
{
	SECTION("Non-virtual") {
		metapp::Variant v(&Base::func1);
		int a = 0;
		std::string b;
		metapp::Variant arguments[] = { metapp::Variant().set<int &>(a), metapp::Variant().set<std::string &>(b) };
		Base obj { 5 };
		v.getMetaType()->invoke(&obj, v, arguments, 2);
		REQUIRE(a == 8);
		REQUIRE(b == "Good");
	}

	SECTION("virtual") {
		metapp::Variant v(&Base::add);
		metapp::Variant arguments[] = { 7 };
		Derived obj { 5 };
		metapp::Variant result = v.getMetaType()->invoke(&obj, v, arguments, 1);
		REQUIRE(result.get<int>() == 12);
	}

}

TEST_CASE("Callable, constructor")
{
	auto metaType = metapp::getMetaType<metapp::Constructor<Base (int)> >();
	metapp::Variant arguments[] = { 7 };
	metapp::Variant obj = metaType->invoke(nullptr, metapp::Variant(), arguments, 1);
	Base & base = obj.get<Base &>();
	REQUIRE(obj.getMetaType() == metapp::getMetaType<Base>());
	REQUIRE(base.myValue == 7);
	REQUIRE(obj.getAddress<Base>()->myValue == 7);
}


} // namespace
