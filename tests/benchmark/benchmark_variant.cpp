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

namespace {

BenchmarkFunc
{
	constexpr int iterations = generalIterations;
	const auto t = measureElapsedTime([iterations]() {
		for(int i = 0; i < iterations; ++i) {
			metapp::Variant v = 5;
			v = 38.0;
			v = (long long)38;
			v = (unsigned short)9;
			v = true;
			v = 1.5f;
			dontOptimizeAway(v);
		}
	});
	printResult(t, iterations, "Variant construct and assignment, with fundamental");
}

BenchmarkFunc
{
	constexpr int iterations = generalIterations;
	const auto t = measureElapsedTime([iterations]() {
		for(int i = 0; i < iterations; ++i) {
			metapp::Variant v = 5;
			v = 38.0;
			v = "abc";
			v = std::string("def");
		}
	});
	printResult(t, iterations, "Variant construct and assignment, with string");
}

BenchmarkFunc
{
	constexpr int iterations = generalIterations;
	const auto t = measureElapsedTime([iterations]() {
		metapp::Variant v = 5;
		for(int i = 0; i < iterations; ++i) {
			dontOptimizeAway(v.get<int>());
		}
	});
	printResult(t, iterations, "Variant get int");
}

BenchmarkFunc
{
	constexpr int iterations = generalIterations;
	const auto t = measureElapsedTime([iterations]() {
		metapp::Variant v = 5;
		for(int i = 0; i < iterations; ++i) {
			v.cast<double>();
			v.cast<long long>();
			v.cast<int>();
		}
	});
	printResult(t, iterations, "Variant cast");
}

struct HeavyCopy
{
	HeavyCopy() : value(0) {}

	HeavyCopy(const HeavyCopy & other) : value(other.value) {
		doHeavyWork();
	}

	HeavyCopy & operator = (const HeavyCopy & other) {
		value = other.value;
		doHeavyWork();
		return *this;
	}

	HeavyCopy(HeavyCopy && other) noexcept : value(other.value) {
	}

	HeavyCopy & operator = (HeavyCopy && other) noexcept {
		value = other.value;
		return *this;
	}

	void doHeavyWork() {
		std::vector<int> dataList(128, 6);
		for(const int item : dataList) {
			value += item;
		}
	}
	int value;
};

BenchmarkFunc
{
	constexpr int iterations = generalIterations;
	const auto t = measureElapsedTime([iterations]() {
		for(int i = 0; i < iterations; ++i) {
			metapp::Variant v = HeavyCopy();
			v = 5;
			v = HeavyCopy();
		}
	});
	printResult(t, iterations, "Variant from heavy copy object");
}


} //namespace
