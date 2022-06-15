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

#include "test.h"

#include "metapp/variant.h"
#include "metapp/allmetatypes.h"

#include <iostream>

namespace {

std::string myFunc1(const int a, const std::string & b)
{
	return b + std::to_string(a);
}

TEST_CASE("metatypes, DefaultArgsFunction, myFunc1")
{
	auto func = metapp::createDefaultArgsFunction(&myFunc1, { "hello" });
	metapp::Variant v(func);
	REQUIRE(v.getMetaType()->getTypeKind() == metapp::tkDefaultArgsFunction);
	REQUIRE(v.getMetaType()->getUpType(0)->getTypeKind() == metapp::tkStdString);
	REQUIRE(v.getMetaType()->getUpType(1)->getTypeKind() == metapp::tkInt);
	REQUIRE(v.getMetaType()->getUpType(2)->getTypeKind() == metapp::tkReference);

	REQUIRE(metapp::callableGetParameterType(v, 0)->equal(metapp::getMetaType<int>()));
	REQUIRE(metapp::callableGetParameterType(v, 1)->equal(metapp::getMetaType<std::string &>()));

	REQUIRE(metapp::callableGetClassType(v)->isVoid());
	REQUIRE(metapp::callableGetParameterCountInfo(v).getMinParameterCount() == 1);
	REQUIRE(metapp::callableGetParameterCountInfo(v).getMaxParameterCount() == 2);
	REQUIRE(metapp::callableGetReturnType(v)->equal(metapp::getMetaType<std::string>()));

	SECTION("Invoke with default arguments") {
		metapp::Variant arguments[] = { 5 };
		REQUIRE(v.getMetaType()->getMetaCallable()->canInvoke(v, nullptr, arguments));
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(v, nullptr, arguments).get<std::string>() == "hello5");
	}
	SECTION("Invoke without default arguments") {
		metapp::Variant arguments[2] = { 6, "good" };
		REQUIRE(v.getMetaType()->getMetaCallable()->canInvoke(v, nullptr, arguments));
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(v, nullptr, arguments).get<std::string>() == "good6");
	}
}

TEST_CASE("metatypes, DefaultArgsFunction, MyClass::myFunc2")
{
	struct MyClass
	{
		std::vector<std::string> myFunc2(
			const int a,
			const std::string b,
			const long c,
			const std::string d,
			const char e
		)
		{
			return {
				std::to_string(a + value),
				b,
				std::to_string(c),
				d,
				std::to_string(e),
			};
		}

		int value;
	};

	metapp::DefaultArgsFunction<decltype(&MyClass::myFunc2)> func(&MyClass::myFunc2, {
		'a',
		"default",
		38
	});
	metapp::Variant v(func);

	REQUIRE(metapp::callableGetParameterType(v, 0)->equal(metapp::getMetaType<int>()));
	REQUIRE(metapp::callableGetParameterType(v, 1)->equal(metapp::getMetaType<std::string>()));
	REQUIRE(metapp::callableGetParameterType(v, 2)->equal(metapp::getMetaType<long>()));
	REQUIRE(metapp::callableGetParameterType(v, 3)->equal(metapp::getMetaType<std::string>()));
	REQUIRE(metapp::callableGetParameterType(v, 4)->equal(metapp::getMetaType<char>()));

	REQUIRE(metapp::callableGetClassType(v)->equal(metapp::getMetaType<MyClass>()));
	REQUIRE(metapp::callableGetParameterCountInfo(v).getMinParameterCount() == 2);
	REQUIRE(metapp::callableGetParameterCountInfo(v).getMaxParameterCount() == 5);
	REQUIRE(metapp::callableGetReturnType(v)->equal(metapp::getMetaType<std::vector<std::string>>()));

	MyClass obj { 3 };
	
	REQUIRE(! metapp::callableCanInvoke(v, &obj, 5));
	REQUIRE(metapp::callableCanInvoke(v, &obj, 5, "a"));
	REQUIRE(metapp::callableCanInvoke(v, &obj, 5, "a", 5.1));
	REQUIRE(metapp::callableCanInvoke(v, &obj, 5, "a", 5.1, std::string("b")));
	REQUIRE(metapp::callableCanInvoke(v, &obj, 5, "a", 5.1, std::string("b"), 38));
	REQUIRE(! metapp::callableCanInvoke(v, &obj, "5", "a", 5.1, std::string("b"), 38));
	REQUIRE(! metapp::callableCanInvoke(v, &obj, 5, "a", 5.1, std::string("b"), 38, 9));

	REQUIRE(metapp::callableRankInvoke(v, &obj, 5) == metapp::invokeRankNone);
	// metapp::invokeRankMatch * 3 is each of the three default arguments receives metapp::invokeRankMatch.
	REQUIRE(metapp::callableRankInvoke(v, &obj, 5, std::string("abc"))
		== metapp::invokeRankMatch + metapp::invokeRankMatch + metapp::invokeRankMatch * 3);
	REQUIRE(metapp::callableRankInvoke(v, &obj, 5, "abc")
		== metapp::invokeRankMatch + metapp::invokeRankCast + metapp::invokeRankMatch * 3);
	REQUIRE(metapp::callableRankInvoke(v, &obj, 5, "abc", 6)
		== metapp::invokeRankMatch + metapp::invokeRankCast + metapp::invokeRankCast + metapp::invokeRankMatch * 2);
	REQUIRE(metapp::callableRankInvoke(v, &obj, 5, "abc", 6L)
		== metapp::invokeRankMatch + metapp::invokeRankCast + metapp::invokeRankMatch + metapp::invokeRankMatch * 2);

	SECTION("Invoke with 3 default arguments") {
		metapp::Variant arguments[2] = { 5, "hello" };
		REQUIRE(v.getMetaType()->getMetaCallable()->canInvoke(v, nullptr, arguments));
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(v, &obj, arguments).get<const std::vector<std::string> &>()
			== 
			std::vector<std::string> { "8", "hello", "38", "default", "97" }
		);
	}
	SECTION("Invoke with 2 default arguments") {
		metapp::Variant arguments[3] = { 6, "good", 98 };
		REQUIRE(v.getMetaType()->getMetaCallable()->canInvoke(v, nullptr, arguments));
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(v, &obj, arguments).get<const std::vector<std::string> &>()
			== 
			std::vector<std::string> { "9", "good", "98", "default", "97" }
		);
	}
}


} // namespace
