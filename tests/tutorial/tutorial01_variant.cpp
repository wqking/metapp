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

#include "tutorial.h"

// Include the header for metapp::Variant
#include "metapp/variant.h"
// To use all declared meta types, include this header
#include "metapp/metatypes/metatypes.h"

#include <iostream>
#include <cstring>

void tutorialVariant()
{
	// v contains int
	metapp::Variant v(5);
	ASSERT(v.get<int>() == 5);
	// can get as reference, to avoid copy the value
	ASSERT(v.get<int &>() == 5);
	ASSERT(v.get<const int &>() == 5);
	// can even get as fancy const volatile reference
	ASSERT(v.get<const volatile int &>() == 5);

	// now v contains std::string
	v = std::string("hello");
	ASSERT(v.get<std::string>() == "hello");
	// get as reference to avoid copy
	ASSERT(v.get<const std::string &>() == "hello");
	v.get<std::string &>() = "world";
	ASSERT(v.get<const std::string &>() == "world");

	// cast to const char *
	const char * s = v.cast<const char *>().get<const char *>();
	ASSERT(strcmp(s, "world") == 0);

	// now v contains char *
	v = "great";
	ASSERT(strcmp(v.get<const char *>(), "great") == 0);

	int array[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
	// now v contains reference to int[2][3]
	v = metapp::Variant::create<int (&)[2][3]>(array);
	ASSERT(v.get<int (&)[2][3]>()[1][2] == 6);
	array[1][2] = 10;
	// since v is a reference to array, modify array will also modify v
	ASSERT(v.get<int (&)[2][3]>()[1][2] == 10);

	int anotherArray[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
	// Now we copy array into v
	v = metapp::Variant::create<int [2][3]>(anotherArray);
	ASSERT(v.get<int (&)[2][3]>()[1][2] == 6);
	anotherArray[1][2] = 10;
	// since v is a copy of anotherArray, modify anotherArray will not affect v
	ASSERT(v.get<int (&)[2][3]>()[1][2] == 6);
}

RUN_TUTORIAL(tutorialVariant)
