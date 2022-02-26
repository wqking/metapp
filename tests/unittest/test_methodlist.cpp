#include "test.h"

#include "metapp/variant.h"
#include "metapp/methodlist.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>


TEST_CASE("MethodList")
{
	metapp::MethodList methodList;
	methodList.addMethod(std::function<int (int)>([](const int n) {
		return n * 2;
	}));
	methodList.addMethod(std::function<int (long)>([](const long n) {
		return (int)n * 3;
	}));
	const metapp::Variant * callable;
	metapp::Variant arguments[] = { 5 };
	REQUIRE(methodList.findCallable(arguments, 1) == &methodList.get(0));
	callable = methodList.findCallable(arguments, 1);
	REQUIRE(callable->getMetaType()->getMetaCallable()->invoke(nullptr, *callable, arguments, 1).get<int>() == 10);

	arguments[0] = 5L;
	REQUIRE(methodList.findCallable(arguments, 1) == &methodList.get(1));
	callable = methodList.findCallable(arguments, 1);
	REQUIRE(callable->getMetaType()->getMetaCallable()->invoke(nullptr, *callable, arguments, 1).get<int>() == 15);
}

