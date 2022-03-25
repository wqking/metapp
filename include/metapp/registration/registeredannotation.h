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

#ifndef METAPP_REGISTEREDANNOTATION_H_969872685611
#define METAPP_REGISTEREDANNOTATION_H_969872685611

#include "metapp/variant.h"

#include <map>
#include <string>

namespace metapp {

class RegisteredAnnotation
{
public:
	RegisteredAnnotation()
		: nameValueMap()
	{
	}

	void addAnnotation(const std::string & name, const Variant & value) {
		nameValueMap.insert(std::make_pair(name, value));
	}

	const Variant & getAnnotation(const std::string & name) const {
		auto it = nameValueMap.find(name);
		if(it != nameValueMap.end()) {
			return it->second;
		}
		return getEmptyVariant();
	}

	const std::map<std::string, Variant> & getAllAnnotations() const {
		return nameValueMap;
	}

private:
	std::map<std::string, Variant> nameValueMap;
};


} // namespace metapp


#endif
