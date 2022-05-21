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

#include <memory>

namespace metapp {

namespace internal_ {

template <typename T>
std::shared_ptr<void> castSharedPtrToVoid(const std::shared_ptr<T> & sp)
{
	return std::static_pointer_cast<void>(sp);
};

template <typename T>
std::shared_ptr<void> castSharedPtrToVoid(const std::shared_ptr<const T> & sp)
{
	return std::static_pointer_cast<void>(
		std::const_pointer_cast<T>(sp)
	);
};

template <typename T>
std::shared_ptr<void> castSharedPtrToVoid(const std::shared_ptr<volatile T> & sp)
{
	return std::static_pointer_cast<void>(
		std::const_pointer_cast<T>(sp)
	);
};

template <typename T>
std::shared_ptr<void> castSharedPtrToVoid(const std::shared_ptr<const volatile T> & sp)
{
	return std::static_pointer_cast<void>(
		std::const_pointer_cast<T>(sp)
	);
};

} // namespace internal_

template <typename T>
struct DeclareMetaTypeBase <std::shared_ptr<T> >
{
	using SharedPtr = std::shared_ptr<T>;

	using UpType = T;
	static constexpr TypeKind typeKind = tkStdSharedPtr;
	static constexpr TypeFlags typeFlags = tfPointerWrapper;

	static void * constructData(VariantData * data, const void * copyFrom) {
		if(data != nullptr) {
			if(copyFrom == nullptr) {
				data->constructSharedPtr(internal_::castSharedPtrToVoid(SharedPtr()));
			}
			else {
				data->constructSharedPtr(internal_::castSharedPtrToVoid(*(SharedPtr *)copyFrom));
			}
			return nullptr;
		}
		else {
			if(copyFrom == nullptr) {
				return new SharedPtr();
			}
			else {
				return new SharedPtr(*(SharedPtr *)copyFrom);
			}
		}
	}

	static bool cast(Variant * result, const Variant & value, const MetaType * toMetaType) {
		if(toMetaType->isPointer()) {
			if(result != nullptr) {
				*result = value.get<SharedPtr &>().get();
			}
			return true;
		}
		if(toMetaType->getTypeKind() == tkStdWeakPtr && getMetaType<SharedPtr>()->getUpType()->equal(toMetaType->getUpType())) {
			if(result != nullptr) {
				*result = std::weak_ptr<T>(value.get<SharedPtr &>());
			}
			return true;
		}

		return commonCast(result, value, getMetaType<SharedPtr>(), toMetaType);
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

	static Variant accessibleGet(const Variant & accessible, const void * /*instance*/) {
		return Variant::reference(*accessible.get<SharedPtr &>());
	}

	static void accessibleSet(const Variant & accessible, void * /*instance*/, const Variant & value) {
		internal_::assignValue(*(accessible.get<SharedPtr &>()), value.cast<T>().template get<const T &>());
	}

};


} // namespace metapp


#endif

