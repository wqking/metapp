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
#include "metapp/registration/registeredenumvalue.h"
#include "metapp/implement/internal/util_i.h"

#include <vector>
#include <map>
#include <algorithm>

namespace metapp {

namespace internal_ {
extern RegisteredEnumValue emptyRegisteredEnumValue;
} // namespace internal_

class MetaEnum
{
public:
	using ValueType = RegisteredEnumValue::ValueType;

public:
	template <typename FT>
	explicit MetaEnum(FT callback)
	{
		callback(*this);
	}

	template <typename T>
	RegisteredEnumValue & registerValue(const std::string & name, const T value) {
		auto it = nameValueMap.find(name);
		if(it != nameValueMap.end()) {
			return *it->second;
		}
		valueList.emplace_back(name, static_cast<ValueType>(value));
		RegisteredEnumValue & registeredEnumValue = valueList.back();
		nameValueMap.insert(typename decltype(nameValueMap)::value_type(registeredEnumValue.getName(), &registeredEnumValue));
		return registeredEnumValue;
	}

	const RegisteredEnumValue & getValue(const std::string & name) const {
		auto it = nameValueMap.find(name);
		if(it != nameValueMap.end()) {
			return *it->second;
		}
		return internal_::emptyRegisteredEnumValue;
	}

	std::vector<std::string> getNameList() const {
		std::vector<std::string> nameList;
		std::transform(valueList.begin(), valueList.end(), std::back_inserter(nameList),
			[](const RegisteredEnumValue & item) {
				return item.getName();
			}
		);
		return nameList;
	}

private:
	RegisteredEnumValueList valueList;
	std::map<
		std::reference_wrapper<const std::string>,
		RegisteredEnumValue *,
		std::less<const std::string>
	> nameValueMap;
};

inline const RegisteredEnumValue & enumGetValue(const Variant & var, const std::string & name)
{
	return var.getMetaType()->getMetaEnum()->getValue(name);
}

inline std::vector<std::string> enumGetNameList(const Variant & var)
{
	return var.getMetaType()->getMetaEnum()->getNameList();
}


} // namespace metapp

#endif
