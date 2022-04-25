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

#include "metapp/metarepo.h"

// To use the built-in meta types, we must include "metapp/allmetatypes.h"
#include "metapp/allmetatypes.h"

#include "metapp/interfaces/metaclass.h"
#include "metapp/interfaces/metaenum.h"

// The anonymous namespace is to avid duplicated symbols with other source file.
// It's not required by metapp, and you don't need it in your projects.
namespace {

// global variable
std::vector<std::string> textList { "hello", "world" };

// global getter/setter
int value_ = 0;
void setValue(const int newValue)
{
	value_ = newValue;
}

int getValue()
{
	return value_;
}

// global function
std::string concat(const int a, const bool b, const std::string & c)
{
	return std::to_string(a)
		+ (b ? "true" : "false")
		+ c
	;
}

// globa class
class MyClass
{
};

} // namespace

void tutorialMetaRepo_registerMetaData()
{
	metapp::MetaRepo * metaRepo = metapp::getMetaRepo();
	metaRepo->registerAccessible("textList", &textList);
	metaRepo->registerAccessible("value", metapp::createAccessor<int>(&getValue, &setValue));
	metaRepo->registerCallable("concat", &concat);
	metaRepo->registerType("MyClass", metapp::getMetaType<MyClass>());
}

// Now let's see how to use field meta data
void tutorialMetaRepo_accessible()
{
	metapp::MetaRepo * metaRepo = metapp::getMetaRepo();

	// Get the meta data of field "value"
	metapp::RegisteredAccessible fieldValue = metaRepo->getAccessible("value");

	// Call metapp::accessibleGet to get the value of the field. The first parameter is the Variant.
	// Call getTarget() to get the underlying Variant.
	ASSERT(metapp::accessibleGet(fieldValue.getTarget(), nullptr).get<int>() == 0);
	// getTarget() can also be omitted, the RegisteredAccessible can convert to Variant automatically
	ASSERT(metapp::accessibleGet(fieldValue, nullptr).get<int>() == 0);

	// Now let's set a new value
	metapp::accessibleSet(fieldValue, nullptr, 5);
	ASSERT(getValue() == 5);
	ASSERT(metapp::accessibleGet(fieldValue, nullptr).get<int>() == 5);

	// Now append some new texts to textList
	metapp::RegisteredAccessible fieldtextList = metaRepo->getAccessible("textList");
	metapp::accessibleGet(fieldtextList, nullptr).get<std::vector<std::string> &>().push_back("good");
	ASSERT(metapp::accessibleGet(fieldtextList, nullptr).get<const std::vector<std::string> &>()[0] == "hello");
	ASSERT(metapp::accessibleGet(fieldtextList, nullptr).get<const std::vector<std::string> &>()[1] == "world");
	ASSERT(metapp::accessibleGet(fieldtextList, nullptr).get<const std::vector<std::string> &>()[2] == "good");
}

// Now let's call the function
void tutorialMetaRepo_method()
{
	metapp::MetaRepo * metaRepo = metapp::getMetaRepo();

	// Get the meta data of method "concat".
	metapp::RegisteredCallable methodConcat = metaRepo->getCallable("concat");
	
	// Call metapp::callableInvoke to invoke the method, and pass the arguments.
	// The return value is a metapp::Variant.
	metapp::Variant result = metapp::callableInvoke(methodConcat, nullptr, 38, true, "Great");
	ASSERT(result.get<const std::string &>() == "38trueGreat");
}

void tutorialMetaRepo_type()
{
	metapp::MetaRepo * metaRepo = metapp::getMetaRepo();

	metapp::RegisteredType myClassType = metaRepo->getType("MyClass");
	ASSERT(myClassType.getTarget() == metapp::getMetaType<MyClass>());
}

void tutorialMetaRepo_main()
{
	tutorialMetaRepo_registerMetaData();

	tutorialMetaRepo_accessible();
	tutorialMetaRepo_method();
	tutorialMetaRepo_type();
}

RUN_TUTORIAL(tutorialMetaRepo_main)
