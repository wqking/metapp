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
#include "metapp/interfaces/metaindexable.h"
#include "metapp/utils/utility.h"

namespace metapp {

template <typename T, size_t length>
struct DeclareMetaTypeArrayBase : CastFromToTypes<T, TypeList<std::string, std::wstring> >
{
	using UpType = typename std::remove_extent<typename std::remove_cv<T>::type>::type;

	static constexpr TypeKind typeKind = tkArray;

	static const MetaIndexable * getMetaIndexable() {
		static MetaIndexable metaIndexable(
			&metaIndexableGetSize,
			&metaIndexableGet,
			&metaIndexableSet
		);
		return &metaIndexable;
	}

	static void * constructData(MetaTypeData * data, const void * copyFrom) {
		return doConstructData<length != MetaIndexable::unknowSize>(data, copyFrom);
	}

private:
	using Common = CommonDeclareMetaType<T &>;
	struct ArrayWrapper
	{
		T data;
	};

	template <bool hasLength>
	static void * doConstructData(MetaTypeData * data, const void * copyFrom, typename std::enable_if<hasLength>::type * = nullptr) {
		if(data != nullptr && copyFrom != nullptr) {
			data->construct<ArrayWrapper>(*(ArrayWrapper **)copyFrom);
		}
		else {
			return Common::constructData(data, copyFrom);
		}
		return nullptr;
	}

	template <bool hasLength>
	static void * doConstructData(MetaTypeData * /*data*/, const void * /*copyFrom*/, typename std::enable_if<! hasLength>::type * = nullptr) {
		errorNotConstructible();
		return nullptr;
	}

	using ElementType = typename std::remove_extent<
			typename std::remove_cv<T>::type
		>::type;

	static size_t metaIndexableGetSize(const Variant & /*var*/)
	{
		return length;
	}

	static Variant metaIndexableGet(const Variant & var, const size_t index)
	{
		return Variant::create<ElementType &>(var.get<T &>()[index]);
	}

	static void metaIndexableSet(const Variant & var, const size_t index, const Variant & value)
	{
		verifyVariantWritable(var);

		if(index >= metaIndexableGetSize(var)) {
			errorInvalidIndex();
		}
		else {
			assignValue(var.get<T &>()[index], value.cast<ElementType>().template get<ElementType &>());
		}
	}

};

template <typename T>
struct DeclareMetaTypeBase <T[]>
	: DeclareMetaTypeArrayBase <T[], MetaIndexable::unknowSize>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <T[N]>
	: DeclareMetaTypeArrayBase <T[N], N>
{
};

template <typename T>
struct DeclareMetaTypeBase <const T[]>
	: DeclareMetaTypeArrayBase <const T[], MetaIndexable::unknowSize>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <const T[N]>
	: DeclareMetaTypeArrayBase <const T[N], N>
{
};

template <typename T>
struct DeclareMetaTypeBase <volatile T[]>
	: DeclareMetaTypeArrayBase <volatile T[], MetaIndexable::unknowSize>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <volatile T[N]>
	: DeclareMetaTypeArrayBase <volatile T[N], N>
{
};

template <typename T>
struct DeclareMetaTypeBase <const volatile T[]>
	: DeclareMetaTypeArrayBase <const volatile T[], MetaIndexable::unknowSize>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <const volatile T[N]>
	: DeclareMetaTypeArrayBase <const volatile T[N], N>
{
};


} // namespace metapp


#endif

