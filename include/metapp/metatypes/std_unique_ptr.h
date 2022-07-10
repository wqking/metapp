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
#include "metapp/interfaces/metapointerwrapper.h"
#include "metapp/implement/internal/util_i.h"

#include <memory>

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <std::unique_ptr<T> >
{
	using UniquePtr = std::unique_ptr<T>;

	using UpType = T;
	static constexpr TypeKind typeKind = tkStdUniquePtr;

	static const MetaAccessible * getMetaAccessible() {
		static MetaAccessible metaAccessible(
			&accessibleGetValueType,
			&accessibleIsReadOnly,
			internal_::voidMetaTypeFromVariant,
			&accessibleGet,
			&accessibleSet
		);
		return &metaAccessible;
	}

	static const MetaPointerWrapper * getMetaPointerWrapper() {
		static MetaPointerWrapper metaPointerWrapper(
			&pointerWrapperGetPointer,
			&pointerWrapperSetPointer
		);
		return &metaPointerWrapper;
	}

private:
	static const MetaType * accessibleGetValueType(const Variant & /*accessible*/) {
		return getMetaType<T>();
	}

	static bool accessibleIsReadOnly(const Variant & /*accessible*/) {
		return std::is_const<T>::value;
	}

	static Variant accessibleGet(const Variant & accessible, const Variant & /*instance*/) {
		return Variant::reference(*accessible.get<UniquePtr &>());
	}

	static void accessibleSet(const Variant & accessible, const Variant & /*instance*/, const Variant & value) {
		requireMutable(accessible);

		internal_::assignValue(*(accessible.get<UniquePtr &>()), value.cast<T>().template get<const T &>());
	}

	static Variant pointerWrapperGetPointer(const Variant & pointerWrapper)
	{
		return pointerWrapper.get<UniquePtr &>().get();
	}

	static void pointerWrapperSetPointer(const Variant & pointerWrapper, const Variant & pointer)
	{
		pointerWrapper.get<UniquePtr &>().reset(pointer.get<T *>());
	}

};


} // namespace metapp


#endif

