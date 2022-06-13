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

#include "benchmark.h"
#include "metapp/variant.h"
#include "metapp/allmetatypes.h"
#include "metapp/interfaces/metacallable.h"

namespace {

struct TestClass
{
	void nothing()
	{
	}

	int add(const int a, const int b)
	{
		return a + b;
	}
};

BenchmarkFunc
{
	constexpr int iterations = generalIterations;
	const auto t = measureElapsedTime([iterations]() {
		metapp::Variant v = &TestClass::nothing;
		TestClass obj;
		metapp::Variant instance = &obj;
		const metapp::MetaCallable * metaCallable = v.getMetaType()->getMetaCallable();
		for(int i = 0; i < iterations; ++i) {
			metaCallable->invoke(v, instance, {});
		}
	});
	printResult(t, iterations, "Callable, invoke `void TestClass::nothing()`");
}

BenchmarkFunc
{
	constexpr int iterations = generalIterations;
	const auto t = measureElapsedTime([iterations]() {
		metapp::Variant v = &TestClass::add;
		TestClass obj;
		metapp::Variant instance = &obj;
		const metapp::MetaCallable * metaCallable = v.getMetaType()->getMetaCallable();
		for(int i = 0; i < iterations; ++i) {
			metapp::Variant arguments[] { i, i + 1 };
			metaCallable->invoke(v, instance, arguments);
		}
	});
	printResult(t, iterations, "Callable, invoke `int TestClass::add(const int a, const int b)` with `int, int`");
}

BenchmarkFunc
{
	constexpr int iterations = generalIterations;
	const auto t = measureElapsedTime([iterations]() {
		metapp::Variant v = &TestClass::add;
		TestClass obj;
		metapp::Variant instance = &obj;
		const metapp::MetaCallable * metaCallable = v.getMetaType()->getMetaCallable();
		for(int i = 0; i < iterations; ++i) {
			metapp::Variant arguments[] { (double)i, (double)i + 1.0 };
			metaCallable->invoke(v, instance, arguments);
		}
	});
	printResult(t, iterations, "Callable, invoke `int TestClass::add(const int a, const int b)` with `double, double`");
}

} //namespace
