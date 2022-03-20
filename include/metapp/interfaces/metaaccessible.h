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

#ifndef METAPP_METAACCESSIBLE_H_969872685611
#define METAPP_METAACCESSIBLE_H_969872685611

#include "metapp/variant.h"

namespace metapp {

class MetaAccessible
{
public:
	MetaAccessible(
		const MetaType * (*getValueType)(),
		Variant (*get)(const Variant & accessible, const void * instance),
		void (*set)(const Variant & accessible, void * instance, const Variant & value)
	)
		:
			getValueType(getValueType),
			get(get),
			set(set)
	{}

	const MetaType * (*getValueType)();
	Variant (*get)(const Variant & accessible, const void * instance);
	void (*set)(const Variant & accessible, void * instance, const Variant & value);
};


} // namespace metapp


#endif
