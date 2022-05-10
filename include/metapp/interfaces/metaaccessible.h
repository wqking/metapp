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
		const MetaType * (*getValueType)(const Variant & var),
		const MetaType * (*getClassType)(const Variant & var),
		bool (*isReadOnly)(const Variant & var),
		Variant (*get)(const Variant & var, const void * instance),
		void (*set)(const Variant & var, void * instance, const Variant & value)
	)
		:
			getValueType(getValueType),
			getClassType(getClassType),
			isReadOnly(isReadOnly),
			get(get),
			set(set)
	{}

	const MetaType * (*getValueType)(const Variant & var);
	const MetaType * (*getClassType)(const Variant & var);
	bool (*isReadOnly)(const Variant & var);
	Variant (*get)(const Variant & var, const void * instance);
	void (*set)(const Variant & var, void * instance, const Variant & value);

	bool isStatic(const Variant & var) const {
		return getClassType(var)->isVoid();
	}
};

inline const MetaType * accessibleGetValueType(const Variant & var)
{
	return var.getMetaType()->getMetaAccessible()->getValueType(var);
}

inline const MetaType * accessibleGetClassType(const Variant & var)
{
	return var.getMetaType()->getMetaAccessible()->getClassType(var);
}

inline bool accessibleIsReadOnly(const Variant & var)
{
	return var.getMetaType()->getMetaAccessible()->isReadOnly(var);
}

inline bool accessibleIsStatic(const Variant & var)
{
	return var.getMetaType()->getMetaAccessible()->isStatic(var);
}

inline Variant accessibleGet(const Variant & var, const void * instance)
{
	return var.getMetaType()->getMetaAccessible()->get(var, instance);
}

inline void accessibleSet(const Variant & var, void * instance, const Variant & value)
{
	var.getMetaType()->getMetaAccessible()->set(var, instance, value);
}

} // namespace metapp


#endif
