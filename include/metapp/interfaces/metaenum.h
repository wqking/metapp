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
#include "metapp/metarepo.h"

#include <map>
#include <algorithm>

namespace metapp {

namespace internal_ {
extern MetaItem emptyMetaItem;
} // namespace internal_

class MetaEnum
{
public:
	template <typename FT>
	explicit MetaEnum(FT callback)
	{
		callback(*this);
	}

	MetaItem & registerValue(const std::string & name, const Variant & value) {
		auto it = nameValueMap.find(name);
		if(it != nameValueMap.end()) {
			return *it->second;
		}
		valueList.emplace_back(MetaItem::Type::enumValue, name, value);
		MetaItem & registeredEnumValue = valueList.back();
		nameValueMap.insert(typename decltype(nameValueMap)::value_type(registeredEnumValue.getName(), &registeredEnumValue));
		return registeredEnumValue;
	}

	const MetaItem & getValue(const std::string & name) const {
		auto it = nameValueMap.find(name);
		if(it != nameValueMap.end()) {
			return *it->second;
		}
		return internal_::emptyMetaItem;
	}

	MetaItemView getValueView() const {
		return MetaItemView(&valueList);
	}

private:
	MetaItemList valueList;
	std::map<
		std::reference_wrapper<const std::string>,
		MetaItem *,
		std::less<const std::string>
	> nameValueMap;
};

inline const MetaItem & enumGetValue(const Variant & var, const std::string & name)
{
	return var.getMetaType()->getMetaEnum()->getValue(name);
}

inline MetaItemView enumGetValueView(const Variant & var)
{
	return var.getMetaType()->getMetaEnum()->getValueView();
}


} // namespace metapp

#endif
