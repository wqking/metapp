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
#include "metapp/interfaces/metacallable.h"
#include "metapp/allmetatypes.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

TEST_CASE("MetaCallable, rankInvoke")
{
	REQUIRE(metapp::callableRankInvoke(std::function<void (int)>(), 5) == metapp::invokeRankMatch);
	REQUIRE(metapp::callableRankInvoke(std::function<void (int)>(), metapp::Variant(5)) == metapp::invokeRankMatch);

	REQUIRE(metapp::callableRankInvoke(std::function<void (std::string)>(), std::string("abc")) == metapp::invokeRankMatch);
	REQUIRE(metapp::callableRankInvoke(std::function<void (std::string)>(), "abc") == metapp::invokeRankCast);

	REQUIRE(metapp::callableRankInvoke(std::function<void (long, std::string)>(), 5, "abc") == metapp::invokeRankCast + metapp::invokeRankCast);
	REQUIRE(metapp::callableRankInvoke(std::function<void (long, std::string)>(), "def", "abc") == metapp::invokeRankNone);
}

TEST_CASE("MetaCallable, overloaded, succeed")
{
	std::vector<metapp::Variant> callableList;
	callableList.push_back(std::function<int (int)>([](const int n) {
		return n + 1;
	}));
	callableList.push_back(std::function<std::string (const std::string &)>([](const std::string & s) {
		return s + "good";
	}));
	//REQUIRE(metapp::r)
}



} // namespace
