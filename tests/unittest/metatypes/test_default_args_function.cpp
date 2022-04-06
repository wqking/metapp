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

#include <iostream>

namespace {

std::string myFunc1(const int a, const std::string & b)
{
	return b + std::to_string(a);
}

TEST_CASE("metatypes, DefaultArgsFunction, myFunc1")
{
	auto func = metapp::createDefaultArgsFunction<1>(&myFunc1, { "hello" });
	metapp::Variant v(func);
	REQUIRE(v.getMetaType()->getTypeKind() == metapp::tkDefaultArgsFunction);
	REQUIRE(v.getMetaType()->getUpType(0)->getTypeKind() == metapp::tkStdString);
	REQUIRE(v.getMetaType()->getUpType(1)->getTypeKind() == metapp::tkStdString);
	REQUIRE(v.getMetaType()->getUpType(2)->getTypeKind() == metapp::tkStdString);

	SECTION("Invoke with default arguments") {
		metapp::Variant arguments[] = { 5 };
		REQUIRE(v.getMetaType()->getMetaCallable()->canInvoke(arguments, 1));
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(v, nullptr, arguments, 1).get<std::string>() == "hello5");
	}
	SECTION("Invoke without default arguments") {
		metapp::Variant arguments[] = { 6, "good" };
		REQUIRE(v.getMetaType()->getMetaCallable()->canInvoke(arguments, 2));
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(v, nullptr, arguments, 2).get<std::string>() == "good6");
	}
}

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

TEST_CASE("metatypes, DefaultArgsFunction, MyClass::myFunc2")
{
	metapp::DefaultArgsFunction<decltype(&MyClass::myFunc2), 3> func(&MyClass::myFunc2, {
		'a',
		"default",
		38
	});
	metapp::Variant v(func);
	MyClass obj { 3 };
	SECTION("Invoke with 3 default arguments") {
		metapp::Variant arguments[] = { 5, "hello" };
		REQUIRE(v.getMetaType()->getMetaCallable()->canInvoke(arguments, 2));
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(v, &obj, arguments, 2).get<const std::vector<std::string> &>()
			== 
			std::vector<std::string> { "8", "hello", "38", "default", "97" }
		);
	}
	SECTION("Invoke with 2 default arguments") {
		metapp::Variant arguments[] = { 6, "good", 98 };
		REQUIRE(v.getMetaType()->getMetaCallable()->canInvoke(arguments, 3));
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(v, &obj, arguments, 3).get<const std::vector<std::string> &>()
			== 
			std::vector<std::string> { "9", "good", "98", "default", "97" }
		);
	}
}


} // namespace
