#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"
#include "metapp/utilities//inspectors.h"
#include "metapp/utilities/utility.h"

namespace {

const void * func1(int, const std::vector<int> &) { return nullptr; }

TEST_CASE("metatypes, free function, types")
{
	metapp::Variant v(&func1);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkFunction);
	REQUIRE(v.get<void *>() == (void *)&func1);

	using namespace metapp;
	auto metaType = v.getMetaType();
	metapp::CallableInspector inspector(metaType);
	REQUIRE(inspector.getClassType() == nullptr);
	REQUIRE(inspector.getParamCount() == 2);
	REQUIRE(matchUpTypeKinds(inspector.getReturnType(), { tkPointer, tkVoid }));
	REQUIRE(matchUpTypeKinds(inspector.getParamType(0), { tkInt }));
	REQUIRE(matchUpTypeKinds(inspector.getParamType(1), { tkReference, tkStdVector, tkInt }));
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
		REQUIRE(v.getMetaType()->getMetaCallable()->canInvoke(arguments, 2));
		REQUIRE(! v.getMetaType()->getMetaCallable()->canInvoke(arguments, 1));
		v.getMetaType()->getMetaCallable()->invoke(v, nullptr, arguments, 2);
		REQUIRE(a == 15);
		REQUIRE(b == "world");
	}
	{
		metapp::Variant v(&myFunc2);
		metapp::Variant arguments[] = { 5, "hello" };
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(v, nullptr, arguments, 2).get<std::string>() == "hello5");
		REQUIRE(metapp::invokeCallable(v, nullptr, 6, "world").get<std::string>() == "world6");
	}
	{
		metapp::Variant v(&myFunc3);
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(v, nullptr, nullptr, 0).get<int>() == 38);
		REQUIRE(metapp::invokeCallable(v, nullptr).get<int>() == 38);
	}
}


} // namespace
