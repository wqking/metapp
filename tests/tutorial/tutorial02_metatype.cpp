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

// To use all declared meta types, include this header
#include "metapp/allmetatypes.h"

#include <iostream>

void tutorialMetaType_identify_MetaType()
{
	// For the same type with same constness, metapp::getMetaType is always the same
	ASSERT(metapp::getMetaType<int>() == metapp::getMetaType<int>());
	ASSERT(metapp::getMetaType<const std::string>() == metapp::getMetaType<const std::string>());
	ASSERT(metapp::getMetaType<const volatile double>() == metapp::getMetaType<const volatile double>());

	// Different constness give different MetaType
	ASSERT(metapp::getMetaType<int>() != metapp::getMetaType<const int>());
	ASSERT(metapp::getMetaType<const std::string>() != metapp::getMetaType<volatile std::string>());
	ASSERT(metapp::getMetaType<const volatile double>() != metapp::getMetaType<volatile double>());

	// Different constness give same UnifiedType
	ASSERT(metapp::getMetaType<int>()->getUnifiedType() == metapp::getMetaType<const int>()->getUnifiedType());
	ASSERT(metapp::getMetaType<const std::string>()->getUnifiedType() == metapp::getMetaType<volatile std::string>()->getUnifiedType());
	ASSERT(metapp::getMetaType<const volatile double>()->getUnifiedType() == metapp::getMetaType<volatile double>()->getUnifiedType());
}

void tutorialMetaType_attributes()
{
	{
		// int
		const metapp::MetaType * metaType = metapp::getMetaType<int>();
		ASSERT(metaType->getTypeKind() == metapp::tkInt);
	}

	{
		// constness
		auto metaType = metapp::getMetaType<const int>();
		ASSERT(metaType->getTypeKind() == metapp::tkInt);
		ASSERT(metaType->isConst());
	}

	{
		// pointer
		auto metaType = metapp::getMetaType<const int *>();
		ASSERT(metaType->getTypeKind() == metapp::tkPointer);
		ASSERT(metaType->getUpType()->getTypeKind() == metapp::tkInt);
		// "const int *" is not a const type
		ASSERT(! metaType->isConst());
		// The UpType "const int" is const
		ASSERT(metaType->getUpType()->isConst());
	}

	{
		// std::map
		auto metaType = metapp::getMetaType<std::map<long, std::string> >();
		ASSERT(metaType->getTypeKind() == metapp::tkStdMap);
		// it has two UpType, the first is long, the second is std::string
		ASSERT(metaType->getUpTypeCount() == 2);
		ASSERT(metaType->getUpType(0)->getTypeKind() == metapp::tkLong);
		ASSERT(metaType->getUpType(1)->getTypeKind() == metapp::tkStdString);
	}
}

void tutorialMetaType_upType()
{
	{
		// MetaType `int` doesn't have any UpType.  
		const metapp::MetaType * metaType = metapp::getMetaType<int>();
		ASSERT(metaType->getTypeKind() == metapp::tkInt);
		ASSERT(metaType->getUpTypeCount() == 0);
		ASSERT(metaType->getUpType() == nullptr);
	}

	{
		// MetaType `int *` has one UpType.  
		const metapp::MetaType * metaType = metapp::getMetaType<int *>();
		ASSERT(metaType->getTypeKind() == metapp::tkInt);
		ASSERT(metaType->getUpTypeCount() == 1);
		ASSERT(metaType->getUpType() == metapp::getMetaType<int>());
	}
}

RUN_TUTORIAL(tutorialMetaType_identify_MetaType)
RUN_TUTORIAL(tutorialMetaType_attributes)
RUN_TUTORIAL(tutorialMetaType_upType)
