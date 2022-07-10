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

#ifndef METAPP_STD_SHARED_PTR_H_969872685611
#define METAPP_STD_SHARED_PTR_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metaaccessible.h"
#include "metapp/interfaces/metapointerwrapper.h"
#include "metapp/implement/internal/util_i.h"

#include <memory>

namespace metapp {

namespace internal_ {

template <typename T>
std::shared_ptr<void> castSharedPtrToVoid(const std::shared_ptr<T> & sp)
{
	return std::static_pointer_cast<void>(sp);
}

template <typename T>
std::shared_ptr<void> castSharedPtrToVoid(const std::shared_ptr<const T> & sp)
{
	return std::static_pointer_cast<void>(
		std::const_pointer_cast<T>(sp)
	);
}

template <typename T>
std::shared_ptr<void> castSharedPtrToVoid(const std::shared_ptr<volatile T> & sp)
{
	return std::static_pointer_cast<void>(
		std::const_pointer_cast<T>(sp)
	);
}

template <typename T>
std::shared_ptr<void> castSharedPtrToVoid(const std::shared_ptr<const volatile T> & sp)
{
	return std::static_pointer_cast<void>(
		std::const_pointer_cast<T>(sp)
	);
}

} // namespace internal_

template <typename T>
struct DeclareMetaTypeBase <std::shared_ptr<T> >
{
	using SharedPtr = std::shared_ptr<T>;

	using UpType = T;
	static constexpr TypeKind typeKind = tkStdSharedPtr;

	static VariantData constructVariantData(const void * copyFrom, const CopyStrategy /*copyStrategy*/) {
		if(copyFrom == nullptr) {
			return VariantData(internal_::castSharedPtrToVoid(SharedPtr()), VariantData::StorageTagSharedPtr());
		}
		else {
			return VariantData(internal_::castSharedPtrToVoid(*(SharedPtr *)copyFrom), VariantData::StorageTagSharedPtr());
		}
	}

	static void * constructData(const void * copyFrom, void * memory, const CopyStrategy copyStrategy) {
		return internal_::constructOnHeap<SharedPtr>(copyFrom, memory, copyStrategy);
	}

	static bool cast(Variant * result, const Variant * fromVar, const MetaType * toMetaType) {
		if(getNonReferenceMetaType(toMetaType)->equal(getMetaType<std::weak_ptr<T> >())) {
			if(result != nullptr) {
				*result = std::weak_ptr<T>(fromVar->get<SharedPtr &>());
			}
			return true;
		}

		return commonCast(result, fromVar, getMetaType<SharedPtr>(), toMetaType);
	}

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
		return Variant::reference(*accessible.get<SharedPtr &>());
	}

	static void accessibleSet(const Variant & accessible, const Variant & /*instance*/, const Variant & value) {
		requireMutable(accessible);

		internal_::assignValue(*(accessible.get<SharedPtr &>()), value.cast<T>().template get<const T &>());
	}

	static Variant pointerWrapperGetPointer(const Variant & pointerWrapper)
	{
		return pointerWrapper.get<SharedPtr &>().get();
	}

	static void pointerWrapperSetPointer(const Variant & pointerWrapper, const Variant & pointer)
	{
		pointerWrapper.get<SharedPtr &>().reset(pointer.get<T *>());
	}

};


} // namespace metapp


#endif

