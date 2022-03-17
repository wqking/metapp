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

#ifndef TEST_H
#define TEST_H

#include "metapp/metatype.h"
#include "metapp/typekind.h"
#include "metapp/utils/typelist.h"

#include "../catch.hpp"

#include <vector>
#include <unordered_set>
#include <tuple>
#include <initializer_list>

const metapp::MetaType * getUpTypeAt(const metapp::MetaType * metaType, size_t index);
std::vector<metapp::TypeKind> getUpTypeTypeKinds(const metapp::MetaType * metaType);

template <typename Iterator>
inline bool matchUpTypeKinds(const metapp::MetaType * metaType, Iterator begin, Iterator end)
{
	while(begin != end) {
		if(metaType == nullptr) {
			return false;
		}
		if(metaType->getTypeKind() != *begin) {
			return false;
		}
		metaType = metaType->getUpType();
		++begin;
	}
	return true;
}

template <typename T>
inline bool matchUpTypeKinds(const metapp::MetaType * metaType, const std::initializer_list<T> & typeKindList)
{
	return matchUpTypeKinds(metaType, std::begin(typeKindList), std::end(typeKindList));
}

template <typename T, typename U>
inline bool matchUpTypeKinds(const metapp::MetaType * metaType, const U & typeKindList)
{
	return matchUpTypeKinds(metaType, std::begin(typeKindList), std::end(typeKindList));
}


#endif
