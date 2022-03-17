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

#ifndef METAPP_POINTER_H_969872685611
#define METAPP_POINTER_H_969872685611

#include "metapp/metatype.h"
#include "metapp/metatypes/utils/declareutil.h"
#include "metapp/utils/utility.h"

namespace metapp {

template <typename T>
struct DeclareMetaTypePointerBase
{
	using Common = CommonDeclareMetaType<T>;
	using UpType = typename std::remove_pointer<T>::type;

	static constexpr TypeKind typeKind = tkPointer;

};

template <typename T>
struct DeclareMetaTypeBase <T *> : public DeclareMetaTypePointerBase<T *>
{
private:
	static_assert(! std::is_void<T>::value, "T should not be void in T * specialization.");

public:
	static const MetaAccessible * getMetaAccessible() {
		static MetaAccessible metaAccessible(
			&accessibleGet,
			&accessibleSet
		);
		return &metaAccessible;
	}

	static Variant accessibleGet(const Variant & accessible, const void * /*instance*/) {
		return *accessible.get<T *>();
	}

	static void accessibleSet(const Variant & accessible, void * /*instance*/, const Variant & value) {
		assignValue(*(accessible.get<T *>()), value.cast<T>().template get<const T &>());
	}

};

template <typename T>
struct DeclareMetaTypeVoidPtrBase : public DeclareMetaTypePointerBase<T>
{
public:
};

template <>
struct DeclareMetaTypeBase <void *> : public DeclareMetaTypeVoidPtrBase<void *>
{
};

template <>
struct DeclareMetaTypeBase <const void *> : public DeclareMetaTypeVoidPtrBase<const void *>
{
};

template <>
struct DeclareMetaTypeBase <volatile void *> : public DeclareMetaTypeVoidPtrBase<volatile void *>
{
};

template <>
struct DeclareMetaTypeBase <const volatile void *> : public DeclareMetaTypeVoidPtrBase<const volatile void *>
{
};

template <>
struct DeclareMetaTypeBase <std::nullptr_t> : public DeclareMetaTypeBase<void *>
{
};


} // namespace metapp


#endif

