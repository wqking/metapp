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

#ifndef METAPP_UTIL_I_H_969872685611
#define METAPP_UTIL_I_H_969872685611

#include "metapp/utilities/utility.h"

#include <vector>
#include <string>

namespace metapp {

namespace internal_ {

template <typename Map>
auto getMapKeys(const Map & map)
	-> std::vector<typename Map::key_type>
{
	std::vector<typename Map::key_type> result;
	for(auto it = std::begin(map); it != std::end(map); ++it) {
		result.push_back(it->first);
	}
	return result;
}

template <typename Map>
auto getPointerFromMap(const Map & map, const std::string & name)
	-> const typename Map::mapped_type *
{
	auto it = map.find(name);
	if(it == map.end()) {
		return nullptr;
	}
	return &it->second;
}

template <typename Map>
auto getValueFromMap(
		const Map & map,
		const std::string & name,
		const typename Map::mapped_type & defaultValue = typename Map::mapped_type()
	)
	-> typename Map::mapped_type
{
	auto it = map.find(name);
	if(it == map.end()) {
		return defaultValue;
	}
	return it->second;
}

inline void verifyVariantWritable(const Variant & var)
{
	auto metaType = getReferredMetaType(var);
	if(metaType->isConst()) {
		errorUnwritable();
	}
}


} // namespace internal_

} // namespace metapp

#endif
