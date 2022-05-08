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
# Tutorial for using MetaType

`metapp::MetaType` represents the meta information of arbitrary C++ type.  
We can obtain almost all information on any C++ type through `metapp::MetaType`,
such as, but not limited to, pointer, reference, const-volatile qualifier, array, function parameters, etc.  
desc*/

//desc ## Include headers

//code
//desc To use all declared meta types, include this header
#include "metapp/allmetatypes.h"
//code

#include <iostream>

/*desc
## Identify and compare MetaType
desc*/
TEST_CASE("tutorialMetaType_identify_MetaType")
{
	//code
	//desc We can use metapp::getMetaType<T>() to get the MetaType for T at compile time.  
	//desc For the same type with same constness, metapp::getMetaType is always the same.
	ASSERT(metapp::getMetaType<int>() == metapp::getMetaType<int>());
	ASSERT(metapp::getMetaType<const std::string>() == metapp::getMetaType<const std::string>());
	ASSERT(metapp::getMetaType<const volatile double>() == metapp::getMetaType<const volatile double>());

	//desc Different constness give different MetaType.
	ASSERT(metapp::getMetaType<int>() != metapp::getMetaType<const int>());
	ASSERT(metapp::getMetaType<const std::string>() != metapp::getMetaType<volatile std::string>());
	ASSERT(metapp::getMetaType<const volatile double>() != metapp::getMetaType<volatile double>());

	//desc Different constness give same UnifiedType.
	//TBF
	//ASSERT(metapp::getMetaType<int>()->getUnifiedType() == metapp::getMetaType<const int>()->getUnifiedType());
	//ASSERT(metapp::getMetaType<const std::string>()->getUnifiedType() == metapp::getMetaType<volatile std::string>()->getUnifiedType());
	//ASSERT(metapp::getMetaType<const volatile double>()->getUnifiedType() == metapp::getMetaType<volatile double>()->getUnifiedType());
	//code
}

/*desc
## Inspect MetaType attributes
desc*/
TEST_CASE("tutorialMetaType_attributes")
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

	{
		//code
		//desc pointer
		auto metaType = metapp::getMetaType<const int *>();
		ASSERT(metaType->getTypeKind() == metapp::tkPointer);
		ASSERT(metaType->getUpType()->getTypeKind() == metapp::tkInt);
		//desc "const int *" is not a const type.
		ASSERT(! metaType->isConst());
		//desc The UpType "const int" is const.
		ASSERT(metaType->getUpType()->isConst());
		//code
	}

	{
		//code
		//desc std::map
		auto metaType = metapp::getMetaType<std::map<long, std::string> >();
		ASSERT(metaType->getTypeKind() == metapp::tkStdMap);
		//desc it has two UpType, the first is long, the second is std::string.
		ASSERT(metaType->getUpTypeCount() == 2);
		ASSERT(metaType->getUpType(0)->getTypeKind() == metapp::tkLong);
		ASSERT(metaType->getUpType(1)->getTypeKind() == metapp::tkStdString);
		//code
	}
}

/*desc
## Working with up type
desc*/
TEST_CASE("tutorialMetaType_upType")
{
	{
		//code
		//desc MetaType `int` doesn't have any UpType.  
		const metapp::MetaType * metaType = metapp::getMetaType<int>();
		ASSERT(metaType->getTypeKind() == metapp::tkInt);
		ASSERT(metaType->getUpTypeCount() == 0);
		ASSERT(metaType->getUpType() == nullptr);
		//code
	}

	{
		//code
		//desc MetaType `int *` has one UpType.  
		const metapp::MetaType * metaType = metapp::getMetaType<int *>();
		ASSERT(metaType->getTypeKind() == metapp::tkPointer);
		ASSERT(metaType->getUpTypeCount() == 1);
		ASSERT(metaType->getUpType() == metapp::getMetaType<int>());
		//code
	}
}

