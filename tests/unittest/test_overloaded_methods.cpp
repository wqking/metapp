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
#include "metapp/metatypes/metatypes.h"
#include "metapp/utilities/utility.h"

#include <string>


TEST_CASE("Overloaded methods, (), (int), (long)")
{
	std::vector<metapp::Variant> methodList;
	methodList.push_back(std::function<int ()>([]() {
		return 1;
	}));
	methodList.push_back(std::function<int (int)>([](const int n) {
		return n * 2;
	}));
	methodList.push_back(std::function<int (long)>([](const long n) {
		return (int)n * 3;
	}));

	auto it = metapp::findCallable(methodList.begin(), methodList.end(), nullptr, 0);
	REQUIRE(it == methodList.begin());
	REQUIRE(it->getMetaType()->getMetaCallable()->invoke(*it, nullptr, nullptr, 0).get<int>() == 1);

	metapp::Variant arguments[] = { 5 };
	it = metapp::findCallable(methodList.begin(), methodList.end(), arguments, 1);
	REQUIRE(it == methodList.begin() + 1);
	REQUIRE(it->getMetaType()->getMetaCallable()->invoke(*it, nullptr, arguments, 1).get<int>() == 10);

	arguments[0] = 5L;
	it = metapp::findCallable(methodList.begin(), methodList.end(), arguments, 1);
	REQUIRE(it == methodList.begin() + 2);
	REQUIRE(it->getMetaType()->getMetaCallable()->invoke(*it, nullptr, arguments, 1).get<int>() == 15);

	arguments[0] = "abc";
	it = metapp::findCallable(methodList.begin(), methodList.end(), arguments, 1);
	REQUIRE(it == methodList.end());
}

TEST_CASE("Overloaded methods, (const char *), (std::string)")
{
	std::vector<metapp::Variant> methodList;
	methodList.push_back(std::function<std::string (const char *)>([](const char * s) {
		return std::string("char*") + s;
	}));
	methodList.push_back(std::function<std::string (const std::string &)>([](const std::string & s) {
		return "string" + s;
	}));
	metapp::Variant arguments[] = { "abc" };
	auto it = metapp::findCallable(methodList.begin(), methodList.end(), arguments, 1);
	REQUIRE(it == methodList.begin());
	REQUIRE(it->getMetaType()->getMetaCallable()->invoke(*it, nullptr, arguments, 1).get<const std::string &>() == "char*abc");

	arguments[0] = std::string("def");
	it = metapp::findCallable(methodList.begin(), methodList.end(), arguments, 1);
	REQUIRE(it == methodList.begin() + 1);
	REQUIRE(it->getMetaType()->getMetaCallable()->invoke(*it, nullptr, arguments, 1).get<const std::string &>() == "stringdef");

	arguments[0] = 5;
	it = metapp::findCallable(methodList.begin(), methodList.end(), arguments, 1);
	REQUIRE(it == methodList.end());
}

