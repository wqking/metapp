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
		metapp::Variant arguments[] = { metapp::Variant::create<int &>(a), metapp::Variant::create<std::string &>(b) };
		v.getMetaType()->getMetaCallable()->invoke(v, nullptr, arguments, 2);
		REQUIRE(a == 38);
		REQUIRE(b == "hello");
	}
}

