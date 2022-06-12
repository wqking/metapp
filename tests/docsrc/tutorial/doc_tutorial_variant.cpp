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

#include "testutil.h"

/*desc
# Tutorial for using Variant

`metapp::Variant` allows to store data of any type and convert between types.  
Variant holds a single value of any type at the same time.
The type can be any C++ type, such as int, class, std::string, std::vector, function pointer, etc, any type.  
desc*/

//desc ## Include headers

//code
//desc Include the header for metapp::Variant
#include "metapp/variant.h"
//desc To use all declared meta types, include this header
#include "metapp/allmetatypes.h"
//code

#include <iostream>
#include <cstring>

//desc ## Use Variant

TEST_CASE("tutorialVariant")
{
	//code
	//desc v contains int.
	metapp::Variant v(5);
	ASSERT(v.get<int>() == 5);
	//desc We can get as reference, to avoid copy the value.
	ASSERT(v.get<int &>() == 5);
	ASSERT(v.get<const int &>() == 5);
	//desc We can even get as fancy const volatile reference.
	ASSERT(v.get<const volatile int &>() == 5);

	//desc Now v contains std::string.
	v = std::string("hello");
	//desc Get as std::string will copy the value, that's inefficient.  
	//desc We should get as reference.
	ASSERT(v.get<std::string>() == "hello");
	//desc Get as reference to avoid copy.
	ASSERT(v.get<const std::string &>() == "hello");
	//desc Whether the reference is const, it doesn't matter.
	ASSERT(v.get<std::string &>() == "hello");
	v.get<std::string &>() = "world";
	ASSERT(v.get<const std::string &>() == "world");

	//desc Now v contains char [].
	v = "great";
	ASSERT(strcmp(v.get<const char []>(), "great") == 0);

	//desc Cast to const char *.
	metapp::Variant casted = v.cast<std::string>();
	ASSERT(casted.get<const std::string &>() == "great");

	//desc Let's see how Variant works with array
	int array[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
	//desc Now v contains reference to int[2][3].
	//desc We can't simply assign array to v because the array type will be lost.
	//desc We need to call Variant::create to retain the array type.
	v = metapp::Variant::reference(array);
	ASSERT(v.get<int (&)[2][3]>()[1][2] == 6);
	//desc Since v is a reference to array, modify array will also modify v
	array[1][2] = 10;
	ASSERT(v.get<int (&)[2][3]>()[1][2] == 10);

	//desc Now we copy array into v.
	int anotherArray[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
	v = anotherArray;
	ASSERT(v.get<int (&)[2][3]>()[1][2] == 6);
	//desc Since v is a copy of anotherArray, modify anotherArray will not affect v.
	anotherArray[1][2] = 10;
	ASSERT(v.get<int (&)[2][3]>()[1][2] == 6);
	//code
}

