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

/*desc
# Tutorial for using MetaRepo and runtime meta data

`MetaRepo` is used to register and retrieve meta types at running time.
The registered meta types can be any C++ types, no matter if they are declared with DeclareMetaType or not.   
desc*/

//desc ## Include headers

//code
//desc Header for MetaRepo
#include "metapp/metarepo.h"

//desc To use the built-in meta types, we must include "metapp/allmetatypes.h"
#include "metapp/allmetatypes.h"
//code

#include "metapp/interfaces/metaclass.h"
#include "metapp/interfaces/metaenum.h"

// The anonymous namespace is to avid duplicated symbols with other source file.
// It's not required by metapp, and you don't need it in your projects.
namespace {

/*desc
## Define some C++ variables and types to reflect for
desc*/

//code
//desc Global variable
std::vector<std::string> textList { "hello", "world" };

//desc Global getter/setter
int value_ = 0;
void setValue(const int newValue)
{
	value_ = newValue;
}

int getValue()
{
	return value_;
}

//desc Global function
std::string concat(const int a, const bool b, const std::string & c)
{
	return std::to_string(a)
		+ (b ? "true" : "false")
		+ c
	;
}

//desc Global class
class MyClass
{
};
//code

} // namespace

void tutorialMetaRepo_registerMetaData()
{
	//code
	/*desc
	## Register meta data

	To get meta data from MetaRepo, we must register meta data first.  
	Usually the global meta data should be registered to the global MetaRepo getting from `metapp::getMetaRepo()`.
	desc*/
	metapp::MetaRepo * metaRepo = metapp::getMetaRepo();

	/*desc
	Register an accessible. An accessible is any meta type that implements meta interface `MetaAccessible`.  
	An accessible can be a pointer to data, pointer to member data, or Accessor.  
	The second argument in `registerAccessible` is a `metapp::Variant`. Any data can be casted to `metapp::Variant`
	implicitly, so we don't need to specify `metapp::Variant` explicitly.
	desc*/
	metaRepo->registerAccessible("textList", &textList);
	metaRepo->registerAccessible("value", metapp::createAccessor<int>(&getValue, &setValue));

	/*desc
	Register an callable. A callable is any meta type that implements meta interface `MetaCallable`.  
	A callable can be a pointer to function, pointer to member function, or std::function, and so on.  
	desc*/
	metaRepo->registerCallable("concat", &concat);

	/*desc
	Register a C++ type, here is class MyClass.  
	This is only to demonstrate how to use registered meta type. There is a separate completed tutorial for how to use MetaClass.  
	desc*/
	metaRepo->registerType("MyClass", metapp::getMetaType<MyClass>());
	//code
}

void tutorialMetaRepo_accessible()
{
	//code
	/*desc
	## Use field meta data

	Get the `MetaRepo` to which we registered the meta data
	desc*/
	metapp::MetaRepo * metaRepo = metapp::getMetaRepo();

	//desc Get the meta data of field "value"
	metapp::RegisteredAccessible fieldValue = metaRepo->getAccessible("value");

	//desc Call metapp::accessibleGet to get the value of the field. The first parameter is the Variant.  
	//desc Call getTarget() to get the underlying Variant.
	ASSERT(metapp::accessibleGet(fieldValue.getTarget(), nullptr).get<int>() == 0);
	//desc getTarget() can also be omitted, the RegisteredAccessible can convert to Variant automatically
	ASSERT(metapp::accessibleGet(fieldValue, nullptr).get<int>() == 0);

	//desc Now let's set a new value
	metapp::accessibleSet(fieldValue, nullptr, 5);
	ASSERT(getValue() == 5);
	ASSERT(metapp::accessibleGet(fieldValue, nullptr).get<int>() == 5);

	//desc Now append some new texts to textList
	metapp::RegisteredAccessible fieldtextList = metaRepo->getAccessible("textList");
	metapp::accessibleGet(fieldtextList, nullptr).get<std::vector<std::string> &>().push_back("good");
	ASSERT(metapp::accessibleGet(fieldtextList, nullptr).get<const std::vector<std::string> &>()[0] == "hello");
	ASSERT(metapp::accessibleGet(fieldtextList, nullptr).get<const std::vector<std::string> &>()[1] == "world");
	ASSERT(metapp::accessibleGet(fieldtextList, nullptr).get<const std::vector<std::string> &>()[2] == "good");
	//code
}

void tutorialMetaRepo_method()
{
	//code
	/*desc
	## Use function meta data
	desc*/
	
	metapp::MetaRepo * metaRepo = metapp::getMetaRepo();

	//desc Get the meta data of method "concat".
	metapp::RegisteredCallable methodConcat = metaRepo->getCallable("concat");
	
	//desc Call metapp::callableInvoke to invoke the method, and pass the arguments.  
	//desc The return value is a metapp::Variant.
	metapp::Variant result = metapp::callableInvoke(methodConcat, nullptr, 38, true, "Great");
	ASSERT(result.get<const std::string &>() == "38trueGreat");
	//code
}

void tutorialMetaRepo_type()
{
	//code
	/*desc
	## Use registered types
	desc*/

	metapp::MetaRepo * metaRepo = metapp::getMetaRepo();

	metapp::RegisteredType myClassType = metaRepo->getType("MyClass");
	ASSERT(myClassType.getTarget() == metapp::getMetaType<MyClass>());
	//code
}

TEST_CASE("tutorialMetaRepo_main")
{
	tutorialMetaRepo_registerMetaData();

	tutorialMetaRepo_accessible();
	tutorialMetaRepo_method();
	tutorialMetaRepo_type();
}

