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

#ifndef METAPP_METATYPE_I_H_969872685611
#define METAPP_METATYPE_I_H_969872685611

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

template <typename T, typename Enabled = void>
struct DeclareMetaType;
template <typename T>
struct CommonDeclareMetaType;

namespace internal_ {

class UnifiedType;

template <typename T>
const UnifiedType * doGetUnifiedType();

using MetaInterfaceKind = uint32_t;
using MetaInterfaceGetter = const void * (*)();

static constexpr MetaInterfaceKind mikStart = (1 << 8);
static constexpr MetaInterfaceKind mikMetaClass = (mikStart << 0);
static constexpr MetaInterfaceKind mikMetaCallable = (mikStart << 1);
static constexpr MetaInterfaceKind mikMetaAccessible = (mikStart << 2);
static constexpr MetaInterfaceKind mikMetaEnum = (mikStart << 3);
static constexpr MetaInterfaceKind mikMetaIndexable = (mikStart << 4);
static constexpr MetaInterfaceKind mikMetaIterable = (mikStart << 5);
static constexpr MetaInterfaceKind mikMetaStreamable = (mikStart << 6);
static constexpr MetaInterfaceKind mikMetaMappable = (mikStart << 7);
static constexpr MetaInterfaceKind mikMetaPointerWrapper = (mikStart << 8);
static constexpr MetaInterfaceKind mikMetaUser = (mikStart << 9);

static constexpr uint32_t metaInterfaceCountMask = 0xff;

struct MetaInterfaceItem
{
	MetaInterfaceKind kind;
	MetaInterfaceGetter getter;
};

struct MetaInterfaceData
{
	MetaInterfaceKind kinds;
	const MetaInterfaceItem * items;
};

struct MakeMetaInterfaceItem_MetaClass
{
	static constexpr MetaInterfaceKind kind = mikMetaClass;

	template <typename T>
	static constexpr MetaInterfaceItem make() {
		using M = DeclareMetaType<T>;

		return {
			kind,
			(MetaInterfaceGetter)M::getMetaClass
		};
	}
};

struct MakeMetaInterfaceItem_MetaCallable
{
	static constexpr MetaInterfaceKind kind = mikMetaCallable;

	template <typename T>
	static constexpr MetaInterfaceItem make() {
		using M = DeclareMetaType<T>;

		return {
			kind,
			(MetaInterfaceGetter)M::getMetaCallable
		};
	}
};

struct MakeMetaInterfaceItem_MetaAccessible
{
	static constexpr MetaInterfaceKind kind = mikMetaAccessible;

	template <typename T>
	static constexpr MetaInterfaceItem make() {
		using M = DeclareMetaType<T>;

		return {
			kind,
			(MetaInterfaceGetter)M::getMetaAccessible
		};
	}
};

struct MakeMetaInterfaceItem_MetaEnum
{
	static constexpr MetaInterfaceKind kind = mikMetaEnum;

	template <typename T>
	static constexpr MetaInterfaceItem make() {
		using M = DeclareMetaType<T>;

		return {
			kind,
			(MetaInterfaceGetter)M::getMetaEnum
		};
	}
};

struct MakeMetaInterfaceItem_MetaIndexable
{
	static constexpr MetaInterfaceKind kind = mikMetaIndexable;

	template <typename T>
	static constexpr MetaInterfaceItem make() {
		using M = DeclareMetaType<T>;

		return {
			kind,
			(MetaInterfaceGetter)M::getMetaIndexable
		};
	}
};

struct MakeMetaInterfaceItem_MetaIterable
{
	static constexpr MetaInterfaceKind kind = mikMetaIterable;

	template <typename T>
	static constexpr MetaInterfaceItem make() {
		using M = DeclareMetaType<T>;

		return {
			kind,
			(MetaInterfaceGetter)M::getMetaIterable
		};
	}
};

struct MakeMetaInterfaceItem_MetaStreamable
{
	static constexpr MetaInterfaceKind kind = mikMetaStreamable;

	template <typename T>
	static constexpr MetaInterfaceItem make() {
		using M = DeclareMetaType<T>;

		return {
			kind,
			(MetaInterfaceGetter)M::getMetaStreamable
		};
	}
};

struct MakeMetaInterfaceItem_MetaMappable
{
	static constexpr MetaInterfaceKind kind = mikMetaMappable;

	template <typename T>
	static constexpr MetaInterfaceItem make() {
		using M = DeclareMetaType<T>;

		return {
			kind,
			(MetaInterfaceGetter)M::getMetaMappable
		};
	}
};

struct MakeMetaInterfaceItem_MetaPointerWrapper
{
	static constexpr MetaInterfaceKind kind = mikMetaPointerWrapper;

	template <typename T>
	static constexpr MetaInterfaceItem make() {
		using M = DeclareMetaType<T>;

		return {
			kind,
			(MetaInterfaceGetter)M::getMetaPointerWrapper
		};
	}
};

struct MakeMetaInterfaceItem_MetaUser
{
	static constexpr MetaInterfaceKind kind = mikMetaUser;

	template <typename T>
	static constexpr MetaInterfaceItem make() {
		using M = DeclareMetaType<T>;

		return {
			kind,
			(MetaInterfaceGetter)M::getMetaUser
		};
	}
};

template <typename T>
struct MakeMetaInterfaceData
{
	using M = DeclareMetaType<T>;

	using ItemMakerList = typename FilterTypes<
		TypeList<
		MakeMetaInterfaceItem_MetaClass,
		MakeMetaInterfaceItem_MetaCallable,
		MakeMetaInterfaceItem_MetaAccessible,
		MakeMetaInterfaceItem_MetaEnum,
		MakeMetaInterfaceItem_MetaIndexable,
		MakeMetaInterfaceItem_MetaIterable,
		MakeMetaInterfaceItem_MetaStreamable,
		MakeMetaInterfaceItem_MetaMappable,
		MakeMetaInterfaceItem_MetaPointerWrapper,
		MakeMetaInterfaceItem_MetaUser
		>,
		BoolConstantList<
		HasMember_getMetaClass<M>::value,
		HasMember_getMetaCallable<M>::value,
		HasMember_getMetaAccessible<M>::value,
		HasMember_getMetaEnum<M>::value,
		HasMember_getMetaIndexable<M>::value,
		HasMember_getMetaIterable<M>::value,
		HasMember_getMetaStreamable<M>::value,
		HasMember_getMetaMappable<M>::value,
		HasMember_getMetaPointerWrapper<M>::value,
		HasMember_getMetaUser<M>::value
		>
	>::Type;

	template <typename TL>
	struct ItemListMaker
	{
		static constexpr MetaInterfaceKind kinds = 0;

		static const MetaInterfaceItem * make() {
			return nullptr;
		}
	};

	template <typename Type0, typename ...Types>
	struct ItemListMaker <TypeList<Type0, Types...> >
	{
		static constexpr MetaInterfaceKind kinds = Type0::kind | ItemListMaker <TypeList<Types...> >::kinds;

		static const MetaInterfaceItem * make() {
			static const std::array<MetaInterfaceItem, sizeof...(Types) + 1> itemList {
				Type0::template make<T>(),
				Types::template make<T>()...
			};
			return itemList.data();
		}
	};

	static MetaInterfaceData getMetaInterfaceData() {
		return {
			ItemListMaker<ItemMakerList>::kinds | TypeListCount<ItemMakerList>::value,
			ItemListMaker<ItemMakerList>::make()
		};
	}

};

struct UnifiedMetaTable
{
	VariantData (*constructVariantData)(const void * copyFrom, const CopyStrategy copyStrategy);
	void * (*constructData)(const void * copyFrom, void * memory, const CopyStrategy copyStrategy);

	void (*destroy)(void * instance, const bool freeMemory);

	bool (*cast)(Variant * result, const Variant * fromVar, const MetaType * toMetaType);

	bool (*castFrom)(Variant * result, const Variant * fromVar, const MetaType * fromMetaType);

	MetaInterfaceData metaInterfaceData;
};

struct UpTypeData
{
	int count;
	const MetaType ** upTypeList;
};

class UnifiedType
{
private:
	UnifiedType() = delete;
	UnifiedType(const UnifiedType &) = delete;
	UnifiedType(UnifiedType &&) = delete;

	~UnifiedType() = default;

	TypeKind getTypeKind() const noexcept {
		return typeKind;
	}

	VariantData constructVariantData(const void * copyFrom, const CopyStrategy copyStrategy) const {
		return metaMethodTable.constructVariantData(copyFrom, copyStrategy);
	}

	void * constructData(const void * copyFrom, void * memory, const CopyStrategy copyStrategy) const {
		return metaMethodTable.constructData(copyFrom, memory, copyStrategy);
	}

	void destroy(void * instance) const {
		metaMethodTable.destroy(instance, true);
	}

	void dtor(void * instance) const {
		metaMethodTable.destroy(instance, false);
	}

	bool cast(Variant * result, const Variant * fromVar, const MetaType * toMetaType) const {
		return metaMethodTable.cast(result, fromVar, toMetaType);
	}

	bool castFrom(Variant * result, const Variant * fromVar, const MetaType * fromMetaType) const {
		if(metaMethodTable.castFrom == nullptr) {
			return false;
		}
		return metaMethodTable.castFrom(result, fromVar, fromMetaType);
	}

	const void * getMetaInterface(const MetaInterfaceKind kind) const;

	bool hasMetaInterface(const MetaInterfaceKind kind) const {
		return (kind & metaMethodTable.metaInterfaceData.kinds) != 0;
	}

private:
	constexpr UnifiedType(
		const TypeKind typeKind,
		const UnifiedMetaTable & metaMethodTable,
		const UpTypeData & upTypeData
	) noexcept
		:
			typeKind(typeKind),
			metaMethodTable(metaMethodTable),
			upTypeData(upTypeData)
	{
	}

	template <typename T>
	friend const UnifiedType * doGetUnifiedType();
	friend class metapp::MetaType;

private:
	TypeKind typeKind;
	UnifiedMetaTable metaMethodTable;
	UpTypeData upTypeData;
};

template <typename P>
inline Variant doPointerToReference(const Variant & value, std::true_type)
{
	return value;
}

template <typename P>
inline Variant doPointerToReference(const Variant & value, std::false_type)
{
	return Variant::reference(**(P **)value.getAddress());
}


} // namespace internal_

} // namespace metapp

#endif
