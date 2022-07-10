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

BenchmarkFunc
{
	constexpr int iterations = generalIterations;
	const auto t = measureElapsedTime([iterations]() {
		for(int i = 0; i < iterations; ++i) {
			dontOptimizeAway(metapp::getMetaType<int>());
		}
	});
	printResult(t, iterations, "Misc, getMetaType<int>`");
}

BenchmarkFunc
{
	constexpr int iterations = generalIterations;
	const auto t = measureElapsedTime([iterations]() {
		for(int i = 0; i < iterations; ++i) {
			metapp::Variant v;
			dontOptimizeAway(v);
		}
	});
	printResult(t, iterations, "Misc, Variant construct default");
}

BenchmarkFunc
{
	constexpr int iterations = generalIterations;
	const auto t = measureElapsedTime([iterations]() {
		for(int i = 0; i < iterations; ++i) {
			metapp::Variant v(5);
			dontOptimizeAway(v);
		}
	});
	printResult(t, iterations, "Misc, Variant construct with int");
}

BenchmarkFunc
{
	constexpr int iterations = generalIterations;
	const auto t = measureElapsedTime([iterations]() {
		metapp::Variant v(5);
		for(int i = 0; i < iterations; ++i) {
			dontOptimizeAway(v.getAddress());
		}
	});
	printResult(t, iterations, "Misc, Variant getAddress");
}

BenchmarkFunc
{
	constexpr int iterations = generalIterations;
	const auto t = measureElapsedTime([iterations]() {
		metapp::Variant v;
		for(int i = 0; i < iterations; ++i) {
			v = 38.0;
			dontOptimizeAway(v);
		}
	});
	printResult(t, iterations, "Misc, Variant assignment with double");
}

BenchmarkFunc
{
	constexpr int iterations = generalIterations;
	const auto t = measureElapsedTime([iterations]() {
		for(int i = 0; i < iterations; ++i) {
			metapp::Variant v(i);
			v = v.cast<double>();
		}
	});
	printResult(t, iterations, "Misc, Variant cast int to double");
}

BenchmarkFunc
{
	constexpr int iterations = generalIterations;
	const auto t = measureElapsedTime([iterations]() {
		for(int i = 0; i < iterations; ++i) {
			metapp::Variant v(metapp::Variant::reference(i));
			v = v.cast<double>();
		}
	});
	printResult(t, iterations, "Misc, Variant cast int & to double");
}

BenchmarkFunc
{
	constexpr int iterations = generalIterations;
	const auto t = measureElapsedTime([iterations]() {
		for(int i = 0; i < iterations; ++i) {
			metapp::Variant v(i);
			v = v.cast<double &>();
		}
	});
	printResult(t, iterations, "Misc, Variant cast int to double &");
}

BenchmarkFunc
{
	constexpr int iterations = generalIterations;

	const auto t1 = measureElapsedTime([iterations]() {
		metapp::Variant v = std::vector<metapp::Variant>();
		std::vector<metapp::Variant> & dataList = v.get<std::vector<metapp::Variant> &>();
		dataList.resize(1000);
		for(int i = 0; i < iterations; ++i) {
			dataList[i % 1000] = i;
		}
		dontOptimizeAway(v);
	});
	printResult(t1, iterations, "Misc, Fill std::vector<metapp::Variant>, directly");

	const auto t2 = measureElapsedTime([iterations]() {
		metapp::Variant v = std::vector<metapp::Variant>();
		const metapp::MetaIndexable * metaIndexable = v.getMetaType()->getMetaIndexable();
		metaIndexable->resize(v, 1000);
		for(int i = 0; i < iterations; ++i) {
			metaIndexable->set(v, i % 1000, i);
		}
		dontOptimizeAway(v);
	});
	printResult(t2, iterations, "Misc, Fill std::vector<metapp::Variant>, MetaIndexable");
}


} //namespace
