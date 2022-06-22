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
	REQUIRE(metapp::callableRankInvoke(std::function<void (int)>(), nullptr, 5) == metapp::invokeRankMatch);
	REQUIRE(metapp::callableRankInvoke(std::function<void (int)>(), nullptr, metapp::Variant(5)) == metapp::invokeRankCast);

	REQUIRE(metapp::callableRankInvoke(std::function<void (std::string)>(), nullptr, std::string("abc")) == metapp::invokeRankMatch);
	REQUIRE(metapp::callableRankInvoke(std::function<void (std::string)>(), nullptr, "abc") == metapp::invokeRankCast);

	REQUIRE(metapp::callableRankInvoke(std::function<void (long, std::string)>(), nullptr, 5, "abc") == metapp::invokeRankCast + metapp::invokeRankCast);
	REQUIRE(metapp::callableRankInvoke(std::function<void (long, std::string)>(), nullptr, "def", "abc") == metapp::invokeRankNone);
}


} // namespace
