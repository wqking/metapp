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
	REQUIRE(callable->getMetaType()->getMetaCallable()->invoke(*callable, nullptr, arguments, 1).get<int>() == 10);

	arguments[0] = 5L;
	REQUIRE(methodList.findCallable(arguments, 1) == &methodList.get(1));
	callable = methodList.findCallable(arguments, 1);
	REQUIRE(callable->getMetaType()->getMetaCallable()->invoke(*callable, nullptr, arguments, 1).get<int>() == 15);
}

