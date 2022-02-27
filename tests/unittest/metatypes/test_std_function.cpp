#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, std::function<int (int)>, types")
{
	std::function<int (int)> f([](const int n) { return n * 2; });
	metapp::Variant v(f);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdFunction);
	metapp::Variant arguments[] = { 5 };
	REQUIRE(v.getMetaType()->getMetaCallable()->invoke(v, nullptr, arguments, 1).get<int>() == 10);

	using namespace metapp;
	auto metaType = v.getMetaType();
	REQUIRE(matchUpTypeKinds(metaType->getUpType(0), { tkInt }));
	REQUIRE(matchUpTypeKinds(metaType->getUpType(1), { tkInt }));
}

TEST_CASE("metatypes, std::function<void (int &, std::string &)>, invoke")
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
		v.getMetaType()->getMetaCallable()->invoke(v, nullptr, arguments, 2);
		REQUIRE(a == 38);
		REQUIRE(b == "hello");
	}
}

