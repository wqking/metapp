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
#include "metapp/metatypes/metatypes.h"
#include "metapp/utils/metatypedumper.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

TEST_CASE("play camp")
{
	//metapp::MetaTypeDumper().dump(std::cout, metapp::getMetaType<std::vector<const char * volatile * (*)(int)> >());
}

// This test case is used for writting the sample code in the documentation
TEST_CASE("For writting document")
{
	// v contains int
	metapp::Variant v(5);
	assert(v.get<int>() == 5);

	// now v contains std::string
	v = std::string("hello");
	assert(v.get<std::string>() == "hello");
	// get as reference to avoid copy
	assert(v.get<const std::string &>() == "hello");
	v.get<std::string &>() = "world";
	assert(v.get<const std::string &>() == "world");

	// cast to const char *
	const char * s = v.cast<const char *>().get<const char *>();
	assert(strcmp(s, "world") == 0);

	// now v contains char *
	v = "great";
	assert(strcmp(v.get<const char *>(), "great") == 0);

	int array[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
	// now v contains reference to int[2][3]
	v = metapp::Variant::create<int (&)[2][3]>(array);
	assert(v.get<int (&)[2][3]>()[1][2] == 6);
	array[1][2] = 10;
	// since v is a reference to array, modify array will also modify v
	assert(v.get<int (&)[2][3]>()[1][2] == 10);

	int anotherArray[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
	// Now we copy array into v
	v = metapp::Variant::create<int [2][3]>(anotherArray);
	assert(v.get<int (&)[2][3]>()[1][2] == 6);
	anotherArray[1][2] = 10;
	// since v is a copy of anotherArray, modify anotherArray will not affect v
	assert(v.get<int (&)[2][3]>()[1][2] == 6);
}

} // namespace
