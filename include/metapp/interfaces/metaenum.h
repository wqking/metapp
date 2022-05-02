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

#ifndef METAPP_METAENUM_H_969872685611
#define METAPP_METAENUM_H_969872685611

#include "metapp/variant.h"
#include "metapp/implement/internal/util_i.h"
#include "metapp/metarepo.h"

#include <vector>
#include <map>
#include <algorithm>

namespace metapp {

namespace internal_ {
extern RegisteredItem emptyRegisteredItem;
} // namespace internal_

class MetaEnum
{
public:
	template <typename FT>
	explicit MetaEnum(FT callback)
	{
		callback(*this);
	}

	RegisteredItem & registerValue(const std::string & name, const Variant & value) {
		auto it = nameValueMap.find(name);
		if(it != nameValueMap.end()) {
			return *it->second;
		}
		valueList.emplace_back(name, value);
		RegisteredItem & registeredEnumValue = valueList.back();
		nameValueMap.insert(typename decltype(nameValueMap)::value_type(registeredEnumValue.getName(), &registeredEnumValue));
		return registeredEnumValue;
	}

	const RegisteredItem & getValue(const std::string & name) const {
		auto it = nameValueMap.find(name);
		if(it != nameValueMap.end()) {
			return *it->second;
		}
		return internal_::emptyRegisteredItem;
	}

	std::vector<std::string> getNameList() const {
		std::vector<std::string> nameList;
		std::transform(valueList.begin(), valueList.end(), std::back_inserter(nameList),
			[](const RegisteredItem & item) {
				return item.getName();
			}
		);
		return nameList;
	}

private:
	RegisteredItemList valueList;
	std::map<
		std::reference_wrapper<const std::string>,
		RegisteredItem *,
		std::less<const std::string>
	> nameValueMap;
};

inline const RegisteredItem & enumGetValue(const Variant & var, const std::string & name)
{
	return var.getMetaType()->getMetaEnum()->getValue(name);
}

inline std::vector<std::string> enumGetNameList(const Variant & var)
{
	return var.getMetaType()->getMetaEnum()->getNameList();
}


} // namespace metapp

#endif
