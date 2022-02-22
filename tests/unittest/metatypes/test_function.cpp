#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

namespace {

const void * func1(int, const std::vector<int> &) { return nullptr; }

TEST_CASE("metatypes, free function, types")
{
	metapp::Variant v(&func1);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkFunction);
	REQUIRE(v.get<void *>() == (void *)&func1);

	using namespace metapp;
	auto metaType = v.getMetaType();
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(0), { tkPointer, tkVoid }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(1), { tkInt }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(2), { tkReference, tkVector, tkInt }));
	
	metaType = metapp::getMetaType<const void * (int, const std::vector<int> &)>();
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(0), { tkPointer, tkVoid }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(1), { tkInt }));
	REQUIRE(metapp::matchUpTypeKinds(metaType->getUpType(2), { tkReference, tkVector, tkInt }));
}

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

TEST_CASE("metatypes, free function, invoke")
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
		v.getMetaType()->invoke(nullptr, v, arguments, 2);
		REQUIRE(v.getMetaType()->invoke(nullptr, v, arguments, 2).get<std::string>() == "hello5");
	}
	{
		metapp::Variant v(&myFunc3);
		REQUIRE(v.getMetaType()->invoke(nullptr, v, nullptr, 0).get<int>() == 38);
	}
}


} // namespace
