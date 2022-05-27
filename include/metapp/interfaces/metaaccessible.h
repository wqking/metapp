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
#include "metapp/utilities/utility.h"

namespace metapp {

class MetaAccessible
{
public:
	MetaAccessible(
		const MetaType * (*getValueType)(const Variant & accessible),
		bool (*isReadOnly)(const Variant & accessible),
		const MetaType * (*getClassType)(const Variant & accessible),
		Variant (*get)(const Variant & accessible, const Variant & instance),
		void (*set)(const Variant & accessible, const Variant & instance, const Variant & value)
	)
		:
			getValueType(getValueType),
			isReadOnly(isReadOnly),
			getClassType(getClassType),
			get(get),
			set(set)
	{}

	const MetaType * (*getValueType)(const Variant & accessible);
	bool (*isReadOnly)(const Variant & accessible);
	const MetaType * (*getClassType)(const Variant & accessible);
	Variant (*get)(const Variant & accessible, const Variant & instance);
	void (*set)(const Variant & accessible, const Variant & instance, const Variant & value);

	bool isStatic(const Variant & accessible) const {
		return getClassType(accessible)->isVoid();
	}
};

const MetaType * getNonReferenceMetaType(const Variant & var);

inline const MetaType * accessibleGetValueType(const Variant & accessible)
{
	return getNonReferenceMetaType(accessible)->getMetaAccessible()->getValueType(accessible);
}

inline const MetaType * accessibleGetClassType(const Variant & accessible)
{
	return getNonReferenceMetaType(accessible)->getMetaAccessible()->getClassType(accessible);
}

inline bool accessibleIsReadOnly(const Variant & accessible)
{
	return getNonReferenceMetaType(accessible)->getMetaAccessible()->isReadOnly(accessible);
}

inline bool accessibleIsStatic(const Variant & accessible)
{
	return getNonReferenceMetaType(accessible)->getMetaAccessible()->isStatic(accessible);
}

inline Variant accessibleGet(const Variant & accessible, const Variant & instance)
{
	return getNonReferenceMetaType(accessible)->getMetaAccessible()->get(accessible, instance);
}

inline void accessibleSet(const Variant & accessible, const Variant & instance, const Variant & value)
{
	getNonReferenceMetaType(accessible)->getMetaAccessible()->set(accessible, instance, value);
}

} // namespace metapp


#endif
