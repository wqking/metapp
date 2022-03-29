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

#ifndef METAPP_METATYPE_IMPL_H_969872685611
#define METAPP_METATYPE_IMPL_H_969872685611

#include "metapp/implement/internal/construct_i.h"

#include <atomic>

namespace metapp {

namespace internal_ {

template <typename T>
struct UpTypeGetter;

template <typename Type0, typename ...Types>
struct UpTypeGetter <TypeList<Type0, Types...> >
{
	static const MetaType ** makeUpTypeList()
	{
		static std::array<const MetaType *, 1 + sizeof...(Types) + 1> upTypeList {
			(const MetaType *)(sizeof...(Types) + 1),
			getMetaType<Type0>(),
			getMetaType<Types>()...,
		};
		return upTypeList.data();
	}

	static UpTypeData getUpType() {
		return {
			makeUpTypeList()
		};
	}
};

template <>
struct UpTypeGetter <TypeList<> >
{
	static UpTypeData getUpType() {
		return {
			nullptr
		};
	}
};

template <typename T>
struct UpTypeGetter
{
	static UpTypeData getUpType() {
		return UpTypeGetter<TypeList<T> >::getUpType();
	}
};

template <typename T, bool has>
using SelectDeclareClass = typename std::conditional<
	has,
	DeclareMetaType<T>,
	CommonDeclareMetaType<T>
>::type;

template <typename T>
const MetaType * doGetMetaTypeStorage()
{
	using M = DeclareMetaType<T>;

	static const MetaType metaType(
		&unifiedTypeGetter<typename std::remove_cv<T>::type>,
		MetaTable{
			&SelectDeclareClass<T, HasMember_toReference<M>::value>::toReference,
		},
		UpTypeGetter<
			typename SelectDeclareClass<T, HasMember_UpType<M>::value>::UpType
		>::getUpType(),
		SelectDeclareClass<T, HasMember_typeFlags<M>::value>::typeFlags | CommonDeclareMetaType<T>::typeFlags
	);
	return &metaType;
}

template <typename T>
auto doGetMetaType()
-> typename std::enable_if<std::is_same<T, NoneUpType>::value, const MetaType *>::type
{
	return nullptr;
}

template <typename T>
auto doGetMetaType()
-> typename std::enable_if<
	!std::is_same<T, NoneUpType>::value && !HasMember_setup<DeclareMetaType<T> >::value,
	const MetaType *>::type
{
	return doGetMetaTypeStorage<T>();
}

template <typename T>
auto doGetMetaType()
-> typename std::enable_if<
	!std::is_same<T, NoneUpType>::value && HasMember_setup<DeclareMetaType<T> >::value,
	const MetaType *>::type
{
	static std::atomic_flag hasCalledSetup;
	const MetaType * metaType = doGetMetaTypeStorage<T>();
	if(!hasCalledSetup.test_and_set()) {
		DeclareMetaType<T>::setup();
	}
	return metaType;
}

template <typename T>
const UnifiedType * unifiedTypeGetter()
{
	using M = DeclareMetaType<T>;

	static const UnifiedType unifiedType(
		SelectDeclareClass<T, HasMember_typeKind<M>::value>::typeKind,
		UnifiedMetaTable{
			&SelectDeclareClass<T, HasMember_constructData<M>::value>::constructData,
			&SelectDeclareClass<T, HasMember_destroy<M>::value>::destroy,
			&SelectDeclareClass<T, HasMember_cast<M>::value>::cast,
			&SelectDeclareClass<T, HasMember_castFrom<M>::value>::castFrom,

			MakeMetaInterfaceData<T>::getMetaInterfaceData(),
		}
	);
	return &unifiedType;
}

enum class TristateBool
{
	yes,
	no,
	unknown
};

inline TristateBool doCastObject(
	Variant * result,
	const Variant & value,
	const MetaType * fromMetaType,
	const MetaType * toMetaType
)
{
	const MetaType * fromUpType = fromMetaType;
	const MetaType * toUpType = toMetaType;
	if(toMetaType->isReference()) {
		toUpType = toMetaType->getUpType();
		if(fromMetaType->isReference()) {
			fromUpType = fromMetaType->getUpType();
		}
	}
	else if(toMetaType->isPointer() && fromMetaType->isPointer()) {
		toUpType = toMetaType->getUpType();
		fromUpType = fromMetaType->getUpType();
	}
	else {
		return TristateBool::unknown;
	}
	if(fromUpType->isClass() && toUpType->isClass()) {
		const MetaRepo * inheritanceRepo = getMetaRepo();
		if(inheritanceRepo->isClassInHierarchy(fromUpType) && inheritanceRepo->isClassInHierarchy(toUpType)) {
			if(inheritanceRepo->getRelationship(fromUpType, toUpType) != InheritanceRelationship::none) {
				if(result != nullptr) {
					void * instance = nullptr;
					if(fromMetaType->isPointer()) {
						instance = value.get<void *>();
					}
					else {
						instance = value.getAddress();
					}
					instance = inheritanceRepo->cast(instance, fromUpType, toUpType);
					if(toMetaType->isReference()) {
						Variant temp = Variant::create<int &>(*(int *)instance);
						*result = Variant::retype(toMetaType, temp);
					}
					else {
						Variant temp = Variant::create<void *>(instance);
						*result = Variant::retype(toMetaType, temp);
					}
				}
				return TristateBool::yes;
			}
		}
	}
	return TristateBool::unknown;
}

inline TristateBool doCastPointerReference(
	Variant * result,
	const Variant & value,
	const MetaType * fromMetaType,
	const MetaType * toMetaType
)
{
	if((fromMetaType->isReference() && toMetaType->isReference())
		|| (fromMetaType->isPointer() && toMetaType->isPointer())) {
		const MetaType * fromUpType = fromMetaType->getUpType();
		const MetaType * toUpType = toMetaType->getUpType();
		bool matched = false;
		while(fromUpType != nullptr && toUpType != nullptr) {
			if(fromUpType->getUnifiedType() == toUpType->getUnifiedType()) {
				matched = true;
				break;
			}
			if(fromUpType->isPointer() && toUpType->isPointer()) {
				fromUpType = fromUpType->getUpType();
				toUpType = toUpType->getUpType();
				continue;
			}
			break;
		}
		if(matched) {
			if(result != nullptr) {
				*result = Variant::retype(toMetaType, value);
			}
			return TristateBool::yes;
		}
	}

	// This must be after the previous reference/pointer checking,
	// because retype is prefered to cast when the up type is the same
	if(fromMetaType->isReference() && toMetaType->isReference()) {
		if(fromMetaType->getUpType()->cast(result, value, toMetaType->getUpType())) {
			return TristateBool::yes;
		}
	}

	const TristateBool tristateResult = doCastObject(result, value, fromMetaType, toMetaType);
	if(tristateResult != TristateBool::unknown) {
		return tristateResult;
	}

	if(! fromMetaType->isReference() && toMetaType->isReference()
		) {
		if(fromMetaType->cast(result, value, toMetaType->getUpType())) {
			return TristateBool::yes;
		}
	}
	if(fromMetaType->isReference() && ! toMetaType->isReference()
		) {
		if(fromMetaType->getUpType()->cast(result, value, toMetaType)) {
			return TristateBool::yes;
		}
	}

	if(getNonReferenceMetaType(fromMetaType)->getUnifiedType() == getNonReferenceMetaType(toMetaType)->getUnifiedType()) {
		if(result != nullptr) {
			*result = Variant::retype(toMetaType, value);
		}
		return TristateBool::yes;
	}

	return TristateBool::unknown;
}

} // namespace internal_

inline constexpr MetaType::MetaType(
	const internal_::UnifiedType * (*doGetUnifiedType)(),
	const internal_::MetaTable & metaTable,
	const internal_::UpTypeData & upTypeData,
	const TypeFlags typeFlags
) noexcept :
	doGetUnifiedType(doGetUnifiedType),
#ifdef METAPP_DEBUG_ENABLED
	debugUnifiedType(doGetUnifiedType()),
#endif
	metaTable(metaTable),
	upTypeData(upTypeData),
	typeFlags(typeFlags)
{
}

inline const void * MetaType::getUnifiedType() const noexcept
{
	return doGetUnifiedType();
}

inline const MetaType * MetaType::getUpType() const
{
	return upTypeData.upTypeList[1];
}

inline const MetaType * MetaType::getUpType(const size_t i) const
{
	return upTypeData.upTypeList[i + 1];
}

inline size_t MetaType::getUpTypeCount() const noexcept
{
	if(upTypeData.upTypeList == nullptr) {
		return 0;
	}
	return (size_t)upTypeData.upTypeList[0];
}

inline TypeKind MetaType::getTypeKind() const noexcept
{
	return doGetUnifiedType()->getTypeKind();
}

inline constexpr bool MetaType::isConst() const noexcept
{
	return typeFlags & tfConst;
}

inline constexpr bool MetaType::isVolatile() const noexcept
{
	return typeFlags & tfVolatile;
}

inline constexpr bool MetaType::isPointer() const noexcept
{
	return typeFlags & tfPointer;
}

inline constexpr bool MetaType::isReference() const noexcept
{
	return typeFlags & tfReference;
}

inline constexpr bool MetaType::isClass() const noexcept
{
	return typeFlags & tfClass;
}

inline constexpr bool MetaType::isArray() const noexcept
{
	return typeFlags & tfArray;
}

inline const MetaClass * MetaType::getMetaClass() const
{
	return doGetUnifiedType()->getMetaClass();
}

inline const MetaCallable * MetaType::getMetaCallable() const
{
	return doGetUnifiedType()->getMetaCallable();
}

inline const MetaAccessible * MetaType::getMetaAccessible() const
{
	return doGetUnifiedType()->getMetaAccessible();
}

inline const MetaEnum * MetaType::getMetaEnum() const
{
	return doGetUnifiedType()->getMetaEnum();
}

inline const MetaIndexable * MetaType::getMetaIndexable() const
{
	return doGetUnifiedType()->getMetaIndexable();
}

inline const MetaIterable * MetaType::getMetaIterable() const
{
	return doGetUnifiedType()->getMetaIterable();
}

inline const MetaStreaming * MetaType::getMetaStreaming() const
{
	return doGetUnifiedType()->getMetaStreaming();
}

inline const MetaMap * MetaType::getMetaMap() const
{
	return doGetUnifiedType()->getMetaMap();
}

inline const MetaMember * MetaType::getMetaMember() const
{
	return doGetUnifiedType()->getMetaMember();
}

inline const void * MetaType::getMetaUser() const
{
	return doGetUnifiedType()->getMetaUser();
}

inline void * MetaType::construct() const
{
	return constructData(nullptr, nullptr);
}

inline void * MetaType::copyConstruct(const void * copyFrom) const
{
	return constructData(nullptr, copyFrom);
}

inline void * MetaType::constructData(MetaTypeData * data, const void * copyFrom) const
{
	return doGetUnifiedType()->constructData(data, copyFrom);
}

inline void MetaType::destroy(void * instance) const
{
	doGetUnifiedType()->destroy(instance);
}

inline Variant MetaType::toReference(const Variant & value) const
{
	return metaTable.toReference(value);
}

inline bool MetaType::cast(Variant * result, const Variant & value, const MetaType * toMetaType) const
{
	return doGetUnifiedType()->cast(result, value, toMetaType);
}

inline bool MetaType::castFrom(Variant * result, const Variant & value, const MetaType * fromMetaType) const
{
	return doGetUnifiedType()->castFrom(result, value, fromMetaType);
}

template <typename T>
inline void * CommonDeclareMetaType<T>::constructData(MetaTypeData * data, const void * copyFrom)
{
	if(data != nullptr) {
		data->construct<Underlying>(copyFrom);
		return nullptr;
	}
	else {
		return internal_::constructOnHeap<Underlying>(copyFrom);
	}
}

template <typename T>
inline void CommonDeclareMetaType<T>::destroy(void * instance)
{
#if defined(METAPP_COMPILER_GCC)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"
#endif
#if defined(METAPP_COMPILER_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-abstract-non-virtual-dtor"
#endif
	delete static_cast<Underlying *>(instance);
#if defined(METAPP_COMPILER_CLANG)
#pragma clang diagnostic pop
#endif
#if defined(METAPP_COMPILER_GCC)
#pragma GCC diagnostic pop
#endif
}

template <typename T>
inline bool CommonDeclareMetaType<T>::cast(Variant * result, const Variant & value, const MetaType * toMetaType)
{
	const MetaType * fromMetaType = getMetaType<T>();

	const internal_::TristateBool tristate = internal_::doCastPointerReference(result, value, fromMetaType, toMetaType);
	if(tristate != internal_::TristateBool::unknown) {
		return tristate == internal_::TristateBool::yes;
	}

	if(toMetaType->castFrom(result, value, fromMetaType)) {
		return true;
	}

	return false;
}

template <typename T>
inline bool CommonDeclareMetaType<T>::castFrom(Variant * /*result*/, const Variant & /*value*/, const MetaType * /*fromMetaType*/)
{
	return false;
}


inline void * DeclareMetaTypeVoidBase::constructData(MetaTypeData * /*data*/, const void * /*value*/)
{
	return nullptr;
}

inline void DeclareMetaTypeVoidBase::destroy(void * /*instance*/)
{
}

inline Variant DeclareMetaTypeVoidBase::toReference(const Variant & value)
{
	return value;
}

inline bool DeclareMetaTypeVoidBase::cast(Variant * /*result*/, const Variant & /*value*/, const MetaType * /*toMetaType*/)
{
	return false;
}

inline bool DeclareMetaTypeVoidBase::castFrom(Variant * /*result*/, const Variant & /*value*/, const MetaType * /*fromMetaType*/)
{
	return false;
}

inline const MetaType * getNonReferenceMetaType(const MetaType * metaType)
{
	if(metaType->isReference()) {
		metaType = metaType->getUpType();
	}
	return metaType;
}

template <typename T>
inline const MetaType * getMetaType()
{
	return internal_::doGetMetaType<T>();
}


} // namespace metapp


#endif
