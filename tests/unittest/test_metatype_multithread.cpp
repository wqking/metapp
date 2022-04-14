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

#include <string>
#include <iostream>
#include <thread>

namespace {

struct SetupClass {};

volatile size_t setupCalledCount = 0;

} // namespace

template <>
struct metapp::DeclareMetaType <SetupClass> : metapp::DeclareMetaTypeBase <SetupClass>
{
	static void setup() {
		++setupCalledCount;
	}
};


TEST_CASE("MetaType, multithread, DeclareMetaType::setup should be called only once")
{
	std::vector<std::thread> threadList;
	for(int i = 0; i < 1024; ++i) {
		threadList.emplace_back([]() {
			for(int k = 0; k < 1024; ++k) {
				metapp::getMetaType<SetupClass>();
			}
		});
	}
	for(auto & thread : threadList) {
		thread.join();
	}
	REQUIRE(setupCalledCount == 1);
}

