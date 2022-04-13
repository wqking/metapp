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
#include "metapp/metatypedata.h"
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
class MetaStreaming;
class MetaMap;
class MetaMember;

template <typename T>
constexpr const MetaType * getMetaType();

namespace internal_ {

template <typename T>
const MetaType * doGetMetaTypeStorage();

} // namespace internal_

class MetaType
{
public:
	MetaType() = delete;
	MetaType(const MetaType &) = delete;
	MetaType(MetaType &&) = delete;

	~MetaType() = default;

	const void * getUnifiedType() const noexcept;

	const MetaType * getUpType() const;
	const MetaType * getUpType(const size_t i) const;
	size_t getUpTypeCount() const noexcept;

	TypeKind getTypeKind() const noexcept;

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

	const MetaClass * getMetaClass() const;
	const MetaCallable * getMetaCallable() const;
	const MetaAccessible * getMetaAccessible() const;
	const MetaEnum * getMetaEnum() const;
	const MetaIndexable * getMetaIndexable() const;
	const MetaIterable * getMetaIterable() const;
	const MetaStreaming * getMetaStreaming() const;
	const MetaMap * getMetaMap() const;
	const MetaMember * getMetaMember() const;
	const void * getMetaUser() const;

	void * construct() const;
	void * copyConstruct(const void * copyFrom) const;

	// re-implementable meta methods, begin
	void * constructData(MetaTypeData * data, const void * copyFrom) const;

	void destroy(void * instance) const;

	const MetaType * addReference() const;
	
	bool cast(Variant * result, const Variant & value, const MetaType * toMetaType) const;

	bool castFrom(Variant * result, const Variant & value, const MetaType * fromMetaType) const;
	// re-implementable meta methods, end

private:
	MetaType(
		const internal_::UnifiedType * (*doGetUnifiedType)(),
		const internal_::MetaTable & metaTable,
		const internal_::UpTypeData & upTypeData,
		const TypeFlags typeFlags
	) noexcept;

	template <typename T>
	friend const MetaType * internal_::doGetMetaTypeStorage();

private:
	const internal_::UnifiedType * (*doGetUnifiedType)();

#ifdef METAPP_DEBUG_ENABLED
	// need this in debug window to ease debugging
	const internal_::UnifiedType * debugUnifiedType;
#endif

	internal_::MetaTable metaTable;
	internal_::UpTypeData upTypeData;
	TypeFlags typeFlags;
};

template <typename T>
struct CommonDeclareMetaType
{
private:
	using Underlying = typename std::decay<typename std::remove_reference<T>::type>::type;

public:
	using UpType = internal_::NoneUpType;

	static constexpr TypeKind typeKind = tkObject;
	static constexpr TypeFlags typeFlags = 0
		| ((
			(std::is_pointer<T>::value || std::is_same<std::nullptr_t, typename std::remove_cv<T>::type>::value)
			&& ! std::is_reference<T>::value)
			? tfPointer : 0
			)
		| (std::is_reference<T>::value ? tfReference : 0)
		| (std::is_const<T>::value ? tfConst : 0)
		| (std::is_volatile<T>::value ? tfVolatile : 0)
		| (std::is_class<T>::value ? tfClass : 0)
		| (std::is_array<T>::value ? tfArray : 0)
	;

	static void * constructData(MetaTypeData * data, const void * copyFrom);
	static void destroy(void * instance);

	static const MetaType * addReference();

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
	using UpType = internal_::NoneUpType;

	static constexpr TypeKind typeKind = tkVoid;
	static constexpr TypeFlags typeFlags = 0;

	static void * constructData(MetaTypeData * data, const void * copyFrom);
	static void destroy(void * instance);

	static const MetaType * addReference();

	static bool cast(Variant * result, const Variant & value, const MetaType * toMetaType);

	static bool castFrom(Variant * result, const Variant & value, const MetaType * fromMetaType);

};

template<> struct CommonDeclareMetaType<void> : DeclareMetaTypeVoidBase {};
template<> struct CommonDeclareMetaType<const void> : DeclareMetaTypeVoidBase {};
template<> struct CommonDeclareMetaType<volatile void> : DeclareMetaTypeVoidBase {};
template<> struct CommonDeclareMetaType<const volatile void> : DeclareMetaTypeVoidBase {};

} // namespace metapp

#include "metapp/implement/metatype_impl.h"

#include "metapp/implement/variant_impl.h"

#endif
