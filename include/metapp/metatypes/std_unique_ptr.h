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

#ifndef METAPP_STD_UNIQUE_PTR_H_969872685611
#define METAPP_STD_UNIQUE_PTR_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metaaccessible.h"

#include <memory>

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <std::unique_ptr<T> >
{
	using UniquePtr = std::unique_ptr<T>;

	using UpType = T;
	static constexpr TypeKind typeKind = tkStdUniquePtr;
	static constexpr TypeFlags typeFlags = tfPointerWrapper;

	static bool cast(Variant * result, const Variant & value, const MetaType * toMetaType) {
		if(toMetaType->isPointer()) {
			if(result != nullptr) {
				*result = value.get<UniquePtr &>().get();
			}
			return true;
		}

		return commonCast(result, value, getMetaType<UniquePtr>(), toMetaType);
	}

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

	static const MetaType * accessibleGetValueType(const Variant & /*accessible*/) {
		return getMetaType<T>();
	}

	static bool accessibleIsReadOnly(const Variant & /*accessible*/) {
		return std::is_const<T>::value;
	}

	static const MetaType * accessibleGetClassType(const Variant & /*accessible*/) {
		return voidMetaType;
	}

	static Variant accessibleGet(const Variant & accessible, const Variant & /*instance*/) {
		return Variant::reference(*accessible.get<UniquePtr &>());
	}

	static void accessibleSet(const Variant & accessible, const Variant & /*instance*/, const Variant & value) {
		internal_::assignValue(*(accessible.get<UniquePtr &>()), value.cast<T>().template get<const T &>());
	}

};


} // namespace metapp


#endif

