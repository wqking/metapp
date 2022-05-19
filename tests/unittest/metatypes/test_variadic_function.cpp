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

int sum(const metapp::Variant * arguments, const size_t argumentCount)
{
	int total = 0;
	for(size_t i = 0; i < argumentCount; ++i) {
		total += arguments[i].cast<int>().get<int>();
	}
	return total;
}

TEST_CASE("metatypes, VariadicFunction, sum")
{
	metapp::Variant v = (metapp::VariadicFunction<decltype(&sum)>(&sum));
	REQUIRE(v.getMetaType()->getTypeKind() == metapp::tkVariadicFunction);
	REQUIRE(v.getMetaType()->getUpType() == nullptr);

	SECTION("0 arguments") {
		REQUIRE(v.getMetaType()->getMetaCallable()->canInvoke(v, {}));
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(v, nullptr, {}).get<int>() == 0);
	}
	SECTION("2 arguments") {
		metapp::Variant arguments[] = { 6, 3 };
		REQUIRE(v.getMetaType()->getMetaCallable()->canInvoke(v, { arguments, 2 }));
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(v, nullptr, { arguments, 2 }).get<int>() == 9);
	}
	SECTION("5 arguments") {
		metapp::Variant arguments[] = { 6, 3, 1, 5, 8 };
		REQUIRE(v.getMetaType()->getMetaCallable()->canInvoke(v, { arguments, 5 }));
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(v, nullptr, { arguments, 5 }).get<int>() == 23);
	}
}

struct ClassForVariadicFunc
{
	std::string join(const metapp::Variant * arguments, const size_t argumentCount)
	{
		std::string result(value);
		for(size_t i = 0; i < argumentCount; ++i) {
			result += arguments[i].cast<std::string>().get<std::string>();
		}
		return result;
	}

	std::string value;
};

TEST_CASE("metatypes, VariadicFunction, ClassForVariadicFunc::join")
{
	metapp::Variant v(metapp::createVariadicFunction(&ClassForVariadicFunc::join));
	SECTION("0 arguments") {
		ClassForVariadicFunc obj { "first" };
		REQUIRE(v.getMetaType()->getMetaCallable()->canInvoke(v, {}));
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(v, &obj, {}).get<const std::string&>()
			== 
			"first"
		);
	}
	SECTION("2 arguments") {
		ClassForVariadicFunc obj { "first" };
		metapp::Variant arguments[] = { "abc", "hello" };
		REQUIRE(v.getMetaType()->getMetaCallable()->canInvoke(v, { arguments, 2 }));
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(v, &obj, { arguments, 2 }).get<const std::string&>()
			== 
			"firstabchello"
		);
	}
}


} // namespace
