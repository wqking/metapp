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
#include "metapp/metatypes/metatypes.h"

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
	// int
	const metapp::MetaType * metaType1 = metapp::getMetaType<int>();
	ASSERT(metaType1->getTypeKind() == metapp::tkInt);

	// constness
	auto metaType2 = metapp::getMetaType<const int>();
	ASSERT(metaType2->getTypeKind() == metapp::tkInt);
	ASSERT(metaType2->isConst());

	// pointer
	auto metaType3 = metapp::getMetaType<const int *>();
	ASSERT(metaType3->getTypeKind() == metapp::tkPointer);
	ASSERT(metaType3->getUpType()->getTypeKind() == metapp::tkInt);
	// "const int *" is not a const type
	ASSERT(! metaType3->isConst());
	// The UpType "const int" is const
	ASSERT(metaType3->getUpType()->isConst());

	// std::map
	auto metaType4 = metapp::getMetaType<std::map<long, std::string> >();
	ASSERT(metaType4->getTypeKind() == metapp::tkStdMap);
	// it has two UpType, the first is long, the second is std::string
	ASSERT(metaType4->getUpTypeCount() == 2);
	ASSERT(metaType4->getUpType(0)->getTypeKind() == metapp::tkLong);
	ASSERT(metaType4->getUpType(1)->getTypeKind() == metapp::tkStdString);
}

RUN_TUTORIAL(tutorialMetaType_identify_MetaType)
RUN_TUTORIAL(tutorialMetaType_attributes)
