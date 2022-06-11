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

#ifndef METAPP_ARRAY_H_969872685611
#define METAPP_ARRAY_H_969872685611

#include "metapp/metatype.h"
#include "metapp/cast.h"
#include "metapp/interfaces/metaindexable.h"
#include "metapp/interfaces/bases/metastreamablebase.h"
#include "metapp/implement/internal/util_i.h"
#include "metapp/metatypes/std_string.h"

namespace metapp {

namespace internal_ {

constexpr std::size_t unknownSize = std::size_t(-1);

} // namespace internal_

template <typename T, std::size_t length>
struct DeclareMetaTypeArrayBase : MetaStreamableBase<T>
{
	using UpType = typename std::remove_extent<typename std::remove_cv<T>::type>::type;

	static constexpr TypeKind typeKind = tkArray;

	static const MetaIndexable * getMetaIndexable() {
		static MetaIndexable metaIndexable(
			&metaIndexableGetSizeInfo,
			&metaIndexableGetValueType,
			nullptr,
			&metaIndexableGet,
			&metaIndexableSet
		);
		return &metaIndexable;
	}

	static void * constructData(VariantData * data, const void * copyFrom, void * memory) {
		return doConstructData<length != internal_::unknownSize>(data, copyFrom, memory);
	}

	static bool cast(Variant * result, const Variant & value, const MetaType * toMetaType) {
		const MetaType * upType = getMetaType<UpType>();
		const MetaType * nonRef = getNonReferenceMetaType(toMetaType);
		if(nonRef->equal(getMetaType<std::string>()) && upType->equal(getMetaType<char>())) {
			if(result != nullptr) {
				*result = std::string((const char *)(value.getAddress()));
			}
			return true;
		}
		if(nonRef->equal(getMetaType<std::wstring>()) && upType->equal(getMetaType<wchar_t>())) {
			if(result != nullptr) {
				*result = std::wstring((const wchar_t *)(value.getAddress()));
			}
			return true;
		}
		if(nonRef->isPointer() && nonRef->getUpType()->equal(upType)) {
			if(result != nullptr) {
				*result = (UpType *)(value.getAddress());
			}
			return true;
		}

		return commonCast(result, value, getMetaType<T>(), toMetaType);
	}

private:
	struct ArrayWrapper
	{
		T data;
	};

	template <bool hasLength>
	static void * doConstructData(VariantData * data, const void * copyFrom, void * memory,
		typename std::enable_if<hasLength>::type * = nullptr) {
		if(data != nullptr) {
			if(copyFrom != nullptr) {
				data->construct<ArrayWrapper>((ArrayWrapper *)copyFrom);
			}
			else {
				data->construct<ArrayWrapper>(nullptr);
			}
		}
		else {
			if(copyFrom != nullptr) {
				return internal_::constructOnHeap<ArrayWrapper>((ArrayWrapper *)copyFrom, memory);
			}
			else {
				return internal_::constructOnHeap<ArrayWrapper>(nullptr, memory);
			}
		}
		return nullptr;
	}

	template <bool hasLength>
	static void * doConstructData(VariantData * /*data*/, const void * /*copyFrom*/, void * /*memory*/,
		typename std::enable_if<! hasLength>::type * = nullptr) {
		raiseException<NotConstructibleException>();
		return nullptr;
	}

	using ElementType = typename std::remove_extent<
			typename std::remove_cv<T>::type
		>::type;

	static MetaIndexable::SizeInfo metaIndexableGetSizeInfo(const Variant & /*var*/)
	{
		MetaIndexable::SizeInfo sizeInfo { length };
		sizeInfo.setResizable(false);
		sizeInfo.setUnknowSize(length == internal_::unknownSize);
		return sizeInfo;
	}

	static const MetaType * metaIndexableGetValueType(const Variant & /*var*/, const std::size_t /*index*/)
	{
		return getMetaType<T>();
	}

	static Variant metaIndexableGet(const Variant & var, const std::size_t index)
	{
		return Variant::create<ElementType &>(var.get<T &>()[index]);
	}

	static void metaIndexableSet(const Variant & var, const std::size_t index, const Variant & value)
	{
		internal_::verifyVariantWritable(var);

		if(index >= metaIndexableGetSizeInfo(var).getSize()) {
			raiseException<OutOfRangeException>();
		}
		else {
			internal_::assignValue(var.get<T &>()[index], value.cast<ElementType>().template get<ElementType &>());
		}
	}

};

template <typename T>
struct DeclareMetaTypeBase <T[]>
	: DeclareMetaTypeArrayBase <T[], internal_::unknownSize>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <T[N]>
	: DeclareMetaTypeArrayBase <T[N], N>
{
};

template <typename T>
struct DeclareMetaTypeBase <const T[]>
	: DeclareMetaTypeArrayBase <const T[], internal_::unknownSize>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <const T[N]>
	: DeclareMetaTypeArrayBase <const T[N], N>
{
};

template <typename T>
struct DeclareMetaTypeBase <volatile T[]>
	: DeclareMetaTypeArrayBase <volatile T[], internal_::unknownSize>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <volatile T[N]>
	: DeclareMetaTypeArrayBase <volatile T[N], N>
{
};

template <typename T>
struct DeclareMetaTypeBase <const volatile T[]>
	: DeclareMetaTypeArrayBase <const volatile T[], internal_::unknownSize>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <const volatile T[N]>
	: DeclareMetaTypeArrayBase <const volatile T[N], N>
{
};


} // namespace metapp


#endif

