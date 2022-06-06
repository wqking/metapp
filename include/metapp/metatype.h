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

#ifndef METAPP_METATYPE_H_969872685611
#define METAPP_METATYPE_H_969872685611

#include "metapp/compiler.h"
#include "metapp/typekind.h"
#include "metapp/variantdata.h"
#include "metapp/exception.h"

#include "metapp/implement/variant_intf.h"

#include "metapp/implement/internal/metatype_i.h"

#include <type_traits>

namespace metapp {

class MetaType;
class Variant;

class MetaClass;
class MetaCallable;
class MetaAccessible;
class MetaArray;
class MetaEnum;
class MetaIndexable;
class MetaIterable;
class MetaStreamable;
class MetaMappable;
class MetaPointerWrapper;

template <typename T>
constexpr const MetaType * getMetaType();

bool commonCast(
	Variant * result,
	const Variant & value,
	const MetaType * fromMetaType,
	const MetaType * toMetaType
);

namespace internal_ {

template <typename T>
const MetaType * doGetMetaTypeStorage();

class UnifiedType;

struct MetaTable
{
	const void * (*doGetUnifiedData)();
};

} // namespace internal_

class Constness
{
public:
	constexpr bool isConst() const noexcept {
		return flags & tfConst;
	}

	constexpr bool isVolatile() const noexcept {
		return flags & tfVolatile;
	}

	constexpr bool canConvertTo(const Constness & other) const noexcept {
		return (flags & other.flags) == flags;
	}

private:
	explicit constexpr Constness(const TypeFlags flags)
		: flags(flags & (tfConst | tfVolatile)) {
	}

private:
	TypeFlags flags;

	friend class MetaType;
};

class MetaType
{
public:
	MetaType() = delete;
	MetaType(const MetaType &) = delete;
	MetaType(MetaType &&) = delete;

	~MetaType() = default;

	const void * getModule() const noexcept {
		return metaTable.doGetUnifiedData();
	}

	TypeKind getTypeKind() const noexcept {
		return unifiedType->getTypeKind();
	}

	bool isVoid() const noexcept {
		return getTypeKind() == tkVoid;
	}

	constexpr bool isConst() const noexcept {
		return typeFlags & tfConst;
	}

	constexpr bool isVolatile() const noexcept {
		return typeFlags & tfVolatile;
	}

	constexpr bool isPointer() const noexcept {
		return typeFlags & tfPointer;
	}

	constexpr bool isReference() const noexcept {
		return typeFlags & tfReference;
	}

	constexpr bool isClass() const noexcept {
		return typeFlags & tfClass;
	}

	constexpr bool isArray() const noexcept {
		return typeFlags & tfArray;
	}

	constexpr bool isEnum() const noexcept {
		return typeFlags & tfEnum;
	}

	constexpr bool isMemberPointer() const noexcept {
		return typeFlags & tfMemberPointer;
	}

	constexpr bool isIntegral() const noexcept {
		return typeFlags & tfIntegral;
	}

	constexpr bool isFloat() const noexcept {
		return typeFlags & tfFloat;
	}

	constexpr bool isArithmetic() const noexcept {
		return isIntegral() || isFloat();
	}

	constexpr Constness getConstness() const noexcept {
		return Constness(typeFlags);
	}

	const MetaClass * getMetaClass() const {
		return static_cast<const MetaClass *>(unifiedType->getMetaInterface(internal_::mikMetaClass));
	}

	const MetaCallable * getMetaCallable() const {
		return static_cast<const MetaCallable *>(unifiedType->getMetaInterface(internal_::mikMetaCallable));
	}

	const MetaAccessible * getMetaAccessible() const {
		return static_cast<const MetaAccessible *>(unifiedType->getMetaInterface(internal_::mikMetaAccessible));
	}

	const MetaEnum * getMetaEnum() const {
		return static_cast<const MetaEnum *>(unifiedType->getMetaInterface(internal_::mikMetaEnum));
	}

	const MetaIndexable * getMetaIndexable() const {
		return static_cast<const MetaIndexable *>(unifiedType->getMetaInterface(internal_::mikMetaIndexable));
	}

	const MetaIterable * getMetaIterable() const {
		return static_cast<const MetaIterable *>(unifiedType->getMetaInterface(internal_::mikMetaIterable));
	}

	const MetaStreamable * getMetaStreamable() const {
		return static_cast<const MetaStreamable *>(unifiedType->getMetaInterface(internal_::mikMetaStreamable));
	}

	const MetaMappable * getMetaMappable() const {
		return static_cast<const MetaMappable *>(unifiedType->getMetaInterface(internal_::mikMetaMappable));
	}

	const MetaPointerWrapper * getMetaPointerWrapper() const {
		return static_cast<const MetaPointerWrapper *>(unifiedType->getMetaInterface(internal_::mikMetaPointerWrapper));
	}

	const void * getMetaUser() const {
		return static_cast<const void *>(unifiedType->getMetaInterface(internal_::mikMetaUser));
	}

	bool hasMetaClass() const {
		return unifiedType->hasMetaInterface(internal_::mikMetaClass);
	}

	bool hasMetaCallable() const {
		return unifiedType->hasMetaInterface(internal_::mikMetaCallable);
	}

	bool hasMetaAccessible() const {
		return unifiedType->hasMetaInterface(internal_::mikMetaAccessible);
	}

	bool hasMetaEnum() const {
		return unifiedType->hasMetaInterface(internal_::mikMetaEnum);
	}

	bool hasMetaIndexable() const {
		return unifiedType->hasMetaInterface(internal_::mikMetaIndexable);
	}

	bool hasMetaIterable() const {
		return unifiedType->hasMetaInterface(internal_::mikMetaIterable);
	}

	bool hasMetaStreamable() const {
		return unifiedType->hasMetaInterface(internal_::mikMetaStreamable);
	}

	bool hasMetaMappable() const {
		return unifiedType->hasMetaInterface(internal_::mikMetaMappable);
	}

	bool hasMetaPointerWrapper() const {
		return unifiedType->hasMetaInterface(internal_::mikMetaPointerWrapper);
	}

	bool hasMetaUser() const {
		return unifiedType->hasMetaInterface(internal_::mikMetaUser);
	}

	bool equal(const MetaType * other) const;
	int compare(const MetaType * other) const;

	const MetaType * getUpType() const;
	const MetaType * getUpType(const int i) const;
	int getUpTypeCount() const noexcept;

	void * construct() const;
	void * copyConstruct(const void * copyFrom) const;
	void * placementConstruct(void * memory) const;
	void * placementCopyConstruct(void * memory, const void * copyFrom) const;

	void destroy(void * instance) const;
	void dtor(void * instance) const;

	bool cast(Variant * result, const Variant & value, const MetaType * toMetaType) const;

private:
	MetaType(
		const internal_::MetaTable & metaTable,
		const internal_::UnifiedType * unifiedType,
		const TypeFlags typeFlags
	) noexcept;

	void * constructData(VariantData * data, const void * copyFrom, void * memory) const;
	bool castFrom(Variant * result, const Variant & value, const MetaType * fromMetaType) const;

	const void * getRawType() const noexcept {
		return (const void *)metaTable.doGetUnifiedData;
	}

	template <typename T>
	friend const MetaType * internal_::doGetMetaTypeStorage();

	friend bool commonCast(
		Variant * result,
		const Variant & value,
		const MetaType * fromMetaType,
		const MetaType * toMetaType
	);

	// Variant needs to call constructData
	friend class Variant;

private:
	internal_::MetaTable metaTable;
	const internal_::UnifiedType * unifiedType;
	TypeFlags typeFlags;
};

template <typename T>
struct CommonDeclareMetaType
{
private:
	using Underlying = typename std::decay<typename std::remove_reference<T>::type>::type;

public:
	using UpType = TypeList<>;

	static constexpr TypeKind typeKind = tkObject;
	static constexpr TypeFlags typeFlags = 0
		| (std::is_const<T>::value ? tfConst : 0)
		| (std::is_volatile<T>::value ? tfVolatile : 0)
		| ((std::is_pointer<T>::value || std::is_same<std::nullptr_t, T>::value) ? tfPointer : 0)
		| (std::is_reference<T>::value ? tfReference : 0)
		| (std::is_class<T>::value ? tfClass : 0)
		| (std::is_array<T>::value ? tfArray : 0)
		| (std::is_enum<T>::value ? tfEnum : 0)
		| (std::is_member_pointer<T>::value ? tfMemberPointer : 0)
		| (std::is_integral<T>::value ? tfIntegral : 0)
		| (std::is_floating_point<T>::value ? tfFloat : 0)
	;

	static void * constructData(VariantData * data, const void * copyFrom, void * memory);
	static void destroy(void * instance, const bool freeMemory);

	static bool cast(Variant * result, const Variant & value, const MetaType * toMetaType);

	static constexpr bool (*castFrom)(Variant * result, const Variant & value, const MetaType * fromMetaType) = nullptr;
};

template <typename T, typename Enabled = void>
struct DeclareMetaTypeRoot
{
};

template <typename T, typename Enabled = void>
struct DeclareMetaTypeBase : DeclareMetaTypeRoot<T>
{
};

template <typename T, typename Enabled>
struct DeclareMetaType : DeclareMetaTypeBase<T>
{
};

struct DeclareMetaTypeVoidBase
{
	using UpType = TypeList<>;

	static constexpr TypeKind typeKind = tkVoid;
	static constexpr TypeFlags typeFlags = 0;

	static void * constructData(VariantData * data, const void * copyFrom, void * memory);
	static void destroy(void * instance, const bool freeMemory);
	static void dtor(void * instance, const bool freeMemory);

	static bool cast(Variant * result, const Variant & value, const MetaType * toMetaType);

	static bool castFrom(Variant * result, const Variant & value, const MetaType * fromMetaType);

};

template<> struct CommonDeclareMetaType<void> : DeclareMetaTypeVoidBase {};
template<> struct CommonDeclareMetaType<const void> : DeclareMetaTypeVoidBase {};
template<> struct CommonDeclareMetaType<volatile void> : DeclareMetaTypeVoidBase {};
template<> struct CommonDeclareMetaType<const volatile void> : DeclareMetaTypeVoidBase {};

extern const MetaType * voidMetaType;

} // namespace metapp

#include "metapp/implement/metatype_impl.h"

#include "metapp/implement/variant_impl.h"

// Need to include the primary meta types to avoid pointer/reference can't be detected properly
#include "metapp/metatypes/cv.h"
#include "metapp/metatypes/pointer.h"
#include "metapp/metatypes/reference.h"


#endif
