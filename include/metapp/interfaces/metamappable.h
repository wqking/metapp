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

#ifndef METAPP_METAMAPPABLE_H_969872685611
#define METAPP_METAMAPPABLE_H_969872685611

#include "metapp/variant.h"
#include "metapp/utilities/utility.h"

#include <functional>

namespace metapp {

class MetaMappable
{
public:
	using Callback = std::function<bool (const Variant &, const Variant &)>;

	MetaMappable(
		const MetaType * (*getValueType)(const Variant & mappable),
		Variant (*get)(const Variant & mappable, const Variant & key),
		void (*set)(const Variant & mappable, const Variant & key, const Variant & value),
		void (*forEach)(const Variant & mappable, const Callback & callback)
	)
		:
			getValueType(getValueType),
			get(get),
			set(set),
			forEach(forEach)
	{
	}

	const MetaType * (*getValueType)(const Variant & mappable);
	Variant (*get)(const Variant & mappable, const Variant & key);
	void (*set)(const Variant & mappable, const Variant & key, const Variant & value);
	void (*forEach)(const Variant & mappable, const Callback & callback);
};

inline const MetaType * mappableGetValueType(const Variant & mappable)
{
	return getNonReferenceMetaType(mappable)->getMetaMappable()->getValueType(mappable);
}

inline Variant mappableGet(const Variant & mappable, const Variant & key)
{
	return getNonReferenceMetaType(mappable)->getMetaMappable()->get(mappable, key);
}

inline void mappableSet(const Variant & mappable, const Variant & key, const Variant & value)
{
	getNonReferenceMetaType(mappable)->getMetaMappable()->set(mappable, key, value);
}

inline void mappableForEach(const Variant & mappable, const MetaMappable::Callback & callback)
{
	getNonReferenceMetaType(mappable)->getMetaMappable()->forEach(mappable, callback);
}


} // namespace metapp

#endif
