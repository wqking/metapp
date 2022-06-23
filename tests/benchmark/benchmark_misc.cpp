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
			metapp::getMetaType<int>();
		}
	});
	printResult(t, iterations, "Misc, getMetaType<int>`");
}

/*
BenchmarkFunc
{
	constexpr int iterations = generalIterations;
	const auto t = measureElapsedTime([iterations]() {
		int n = 5;
		for(int i = 0; i < iterations; ++i) {
			metapp::VariantData data;
			data.construct<int>(&n, metapp::CopyStrategy::copy);
			// below line is to ensure the loop is not optimized out
			if(data.getAddress() == nullptr) break;
		}
	});
	printResult(t, iterations, "Misc, metapp::VariantData construct<int>`");
}
*/

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
		}
	});
	printResult(t, iterations, "Misc, Variant construct with int");
}

BenchmarkFunc
{
	constexpr int iterations = generalIterations;
	const auto t = measureElapsedTime([iterations]() {
		metapp::Variant v;
		for(int i = 0; i < iterations; ++i) {
			v = 38.0;
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


} //namespace
