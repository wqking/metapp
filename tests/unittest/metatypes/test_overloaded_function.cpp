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

TEST_CASE("Overloaded function, (), (int), (long)")
{
	metapp::Variant callable = metapp::OverloadedFunction();
	metapp::OverloadedFunction & overloadedFunction = callable.get<metapp::OverloadedFunction &>();
	overloadedFunction.addCallable(std::function<int ()>([]() {
		return 1;
		}));
	overloadedFunction.addCallable(std::function<int (int)>([](const int n) {
		return n * 2;
		}));
	overloadedFunction.addCallable(std::function<int (long)>([](const long n) {
		return (int)n * 3;
		}));

	REQUIRE(metapp::callableInvoke(callable, nullptr).get<int>() == 1);

	REQUIRE(metapp::callableInvoke(callable, nullptr, 5).get<int>() == 10);

	REQUIRE(metapp::callableInvoke(callable, nullptr, 5L).get<int>() == 15);

	REQUIRE_THROWS(metapp::callableInvoke(callable, nullptr, "abc"));
}

TEST_CASE("Overloaded function, (const char *), (std::string)")
{
	metapp::Variant callable = metapp::OverloadedFunction();
	metapp::OverloadedFunction & overloadedFunction = callable.get<metapp::OverloadedFunction &>();
	overloadedFunction.addCallable(std::function<std::string (const char *)>([](const char * s) {
		return std::string("char*") + s;
		}));
	overloadedFunction.addCallable(std::function<std::string (const std::string &)>([](const std::string & s) {
		return "string" + s;
		}));

	REQUIRE(metapp::callableInvoke(callable, nullptr, "abc").get<const std::string &>() == "char*abc");

	REQUIRE(metapp::callableInvoke(callable, nullptr, std::string("def")).get<const std::string &>() == "stringdef");

	REQUIRE_THROWS(metapp::callableInvoke(callable, nullptr, 5));
}

