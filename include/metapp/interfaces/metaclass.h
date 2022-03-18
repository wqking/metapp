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

#ifndef METAPP_METACLASS_H_969872685611
#define METAPP_METACLASS_H_969872685611

#include "metapp/implement/internal/metaclass_i.h"
#include "metapp/implement/internal/metarepobase_i.h"

#include <vector>
#include <memory>

namespace metapp {

class MetaClass : public internal_::MetaRepoBase
{
private:
	using super = internal_::MetaRepoBase;

public:
	template <typename FT>
	explicit MetaClass(FT callback)
		:
			super(),
			constructorList()
	{
		callback(*this);
	}

	void addConstructor(const Variant & constructor) {
		if(constructor.getMetaType()->getMetaCallable() == nullptr) {
			errorWrongMetaType();
			return;
		}
		if(! constructorList) {
			constructorList = std::make_shared<MethodList>();
		}
		constructorList->addMethod(constructor);
	}
	
	const MethodList * getConstructorList() const {
		return constructorList.get();
	}

private:
	std::shared_ptr<MethodList> constructorList;
};


} // namespace metapp

#endif
