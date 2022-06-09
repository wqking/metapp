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
#include "metapp/interfaces/metaaccessible.h"

namespace {

struct TestClass
{
	int value;
};

BenchmarkFunc
{
	constexpr int iterations = 1000 * 1000;
	const auto t = measureElapsedTime([iterations]() {
		metapp::Variant v = &TestClass::value;
		TestClass obj;
		metapp::Variant instance = &obj;
		const metapp::MetaAccessible * metaAccessible = v.getMetaType()->getMetaAccessible();
		for(int i = 0; i < iterations; ++i) {
			metaAccessible->get(v, instance);
		}
	});
	printResult(t, iterations, "Accessible, get `TestClass::int`");
}

BenchmarkFunc
{
	constexpr int iterations = 1000 * 1000;
	const auto t = measureElapsedTime([iterations]() {
		metapp::Variant v = &TestClass::value;
		TestClass obj;
		metapp::Variant instance = &obj;
		const metapp::MetaAccessible * metaAccessible = v.getMetaType()->getMetaAccessible();
		for(int i = 0; i < iterations; ++i) {
			metaAccessible->set(v, instance, i);
		}
	});
	printResult(t, iterations, "Accessible, set `TestClass::int`");
}


} //namespace
