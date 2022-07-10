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

#ifndef METAPP_MEMBER_DATA_H_969872685611
#define METAPP_MEMBER_DATA_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metaaccessible.h"
#include "metapp/utilities/utility.h"

namespace metapp {

template <typename Class, typename T>
struct DeclareMetaTypeBase <T Class::*, typename std::enable_if<! std::is_function<T>::value>::type>
{
	using UpType = TypeList <Class, T>;
	static constexpr TypeKind typeKind = tkMemberPointer;

	static const MetaAccessible * getMetaAccessible() {
		static MetaAccessible metaAccessible(
			&accessibleGetValueType,
			&accessibleIsReadOnly,
			&accessibleGetClassType,
			&accessibleGet,
			&accessibleSet
		);
		return &metaAccessible;
	}

private:
	using FullType = T Class::*;

	static const MetaType * accessibleGetValueType(const Variant & /*accessible*/) {
		return getMetaType<T>();
	}

	static bool accessibleIsReadOnly(const Variant & /*accessible*/) {
		return std::is_const<T>::value;
	}

	static const MetaType * accessibleGetClassType(const Variant & /*accessible*/) {
		return getMetaType<Class>();
	}

	static Variant accessibleGet(const Variant & accessible, const Variant & instance) {
		const auto pointerAndType = getPointerAndType(instance);
		if(pointerAndType.second->isConst()) {
			if(pointerAndType.second->isVolatile()) {
				return Variant::reference(((const volatile Class *)pointerAndType.first)->*(accessible.get<FullType>()));
			}
			else {
				return Variant::reference(((const Class *)pointerAndType.first)->*(accessible.get<FullType>()));
			}
		}
		else if(pointerAndType.second->isVolatile()) {
			return Variant::reference(((volatile Class *)pointerAndType.first)->*(accessible.get<FullType>()));
		}
		else {
			return Variant::reference(((Class *)pointerAndType.first)->*(accessible.get<FullType>()));
		}
	}

	static void accessibleSet(const Variant & accessible, const Variant & instance, const Variant & value) {
		requireMutable(accessible);
		requireMutable(instance);

		internal_::assignValue(
			((Class *)getPointer(instance))->*(accessible.get<FullType>()),
			value.cast<T>().template get<const T &>()
		);
	}

};


} // namespace metapp


#endif

