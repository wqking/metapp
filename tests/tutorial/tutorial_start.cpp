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

#include <iostream>
#include <cstring>

//desc ### Use Variant

//code
//desc Header for Variant
#include "metapp/variant.h"
//desc To use all declared meta types, include this header
#include "metapp/allmetatypes.h"
//code

void tutorialStart_Variant()
{
	{
		//code
		//desc v contains int.
		metapp::Variant v(5);
		//desc Get the value
		ASSERT(v.get<int>() == 5);
		//code
	}

	{
		//code
		//desc Now v contains std::string.
		metapp::Variant v = std::string("hello");
		//desc Get as reference to avoid copy.
		ASSERT(v.get<const std::string &>() == "hello");
		//desc Cast to const char *.
		metapp::Variant casted = v.cast<const char *>();
		const char * s = casted.get<const char *>();
		ASSERT(strcmp(s, "hello") == 0);
		//code
	}
}

//desc ### Use MetaType
void tutorialStart_MetaType()
{
	{
		//code
		//desc int
		const metapp::MetaType * metaType = metapp::getMetaType<int>();
		ASSERT(metaType->getTypeKind() == metapp::tkInt);
		//code
	}

	{
		//code
		//desc constness
		auto metaType = metapp::getMetaType<const int>();
		ASSERT(metaType->getTypeKind() == metapp::tkInt);
		ASSERT(metaType->isConst());
		//code
	}
}

//desc ### Call function
void tutorialStart_Callable()
{
	//code
	struct MyClass {
		int value;

		int add(const int delta1, const int delta2) const {
			return value + delta1 + delta2;
		}
	};

	metapp::Variant v(&MyClass::add);
	MyClass obj { 5 };
	// The second argument is the pointer to obj, it's required when invoking member function
	metapp::Variant result = metapp::callableInvoke(v, &obj, 3, 9);
	ASSERT(result.get<int>() == 17);
	//code
}

RUN_TUTORIAL(tutorialStart_Variant)
RUN_TUTORIAL(tutorialStart_MetaType)
RUN_TUTORIAL(tutorialStart_Callable)
