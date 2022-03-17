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

#ifndef METAPP_METHODLIST_H_969872685611
#define METAPP_METHODLIST_H_969872685611

#include "metapp/variant.h"
#include "metapp/interfaces/metacallable.h"

#include <vector>

namespace metapp {

class MethodList
{
public:
	void addMethod(const Variant & method) {
		methodList.push_back(method);
	}

	size_t getCount() const {
		return methodList.size();
	}

	const Variant & get(const size_t index) const {
		return methodList[index];
	}

	const Variant * findCallable(const Variant * arguments, const size_t argumentCount) const {
		int maxRank = 0;
		const Variant * callable = nullptr;
		for(const Variant & method : methodList) {
			const int rank = method.getMetaType()->getMetaCallable()->rankInvoke(arguments, argumentCount);
			if(rank > maxRank) {
				maxRank = rank;
				callable = &method;
			}
		}
		return callable;
	}

private:
	std::vector<Variant> methodList;
};


} // namespace metapp

#endif
