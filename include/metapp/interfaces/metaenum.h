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

#include "metapp/implement/internal/util_i.h"

#include <vector>
#include <map>

namespace metapp {

class MetaEnum
{
public:
	using ValueType = long long;

public:
	template <typename FT>
	MetaEnum(FT callback)
	{
		callback(*this);
	}

	template <typename T>
	void addValue(const std::string & name, const T value) {
		nameValueMap[name] = static_cast<ValueType>(value);
	}

	std::vector<std::string> getNameList() const {
		return internal_::getMapKeys(nameValueMap);
	}

	ValueType getValue(const std::string & name) const {
		return internal_::getValueFromMap(nameValueMap, name);
	}

private:
	std::map<std::string, ValueType> nameValueMap;
};


} // namespace metapp

#endif
