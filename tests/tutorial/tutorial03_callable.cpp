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
#include "metapp/interfaces/metacallable.h"
// for invokeCallable
#include "metapp/utils/utility.h"

#include <iostream>
#include <cstring>

namespace {

inline std::string func1(const int n)
{
	return std::to_string(n);
}

void tutorialCallable_freeFunction()
{
	// v is pointer to func1
	metapp::Variant v(&func1);
	// Prepare the arguments array
	metapp::Variant arguments[] { 5 };
	// Invoke the callable, the nullptr is the object instance, for free function, it's nullptr
	metapp::Variant result = v.getMetaType()->getMetaCallable()->invoke(v, nullptr, arguments, 1);
	ASSERT(result.get<std::string>() == "5");

	// Or we can use metapp::invokeCallable to pass the arguments directly
	result = metapp::invokeCallable(v, nullptr, 38);
	ASSERT(result.get<std::string>() == "38");
}

void tutorialCallable_memberFunction()
{
	struct MyClass {
		int value;

		int add(const int delta1, const int delta2) const {
			return value + delta1 + delta2;
		}
	};
	metapp::Variant v(&MyClass::add);
	MyClass obj { 5 };
	metapp::Variant result = metapp::invokeCallable(v, &obj, 3, 9);
	ASSERT(result.get<int>() == 17);
}

void tutorialCallable_stdFunction()
{
	metapp::Variant v(std::function<std::string (const std::string &, const std::string &)>(
		[](const std::string & a, const std::string & b) -> std::string {
			return a + b;
		}
	));
	metapp::Variant result = metapp::invokeCallable(v, nullptr, "Hello ", "world");
	ASSERT(result.get<const std::string &>() == "Hello world");
}

RUN_TUTORIAL(tutorialCallable_freeFunction)
RUN_TUTORIAL(tutorialCallable_memberFunction)
RUN_TUTORIAL(tutorialCallable_stdFunction)

} // namespace
