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
class MetaClass;

namespace internal_ {

class UnifiedType;

template <typename T>
const UnifiedType * unifiedTypeGetter();

struct NoneUpType {};

using MetaInterfaceKind = uint32_t;
using MetaInterfaceGetter = const void * (*)();

static constexpr MetaInterfaceKind mikStart = (1 << 8);
static constexpr MetaInterfaceKind mikMetaClass = (mikStart << 0);
static constexpr MetaInterfaceKind mikMetaCallable = (mikStart << 1);
static constexpr MetaInterfaceKind mikMetaAccessible = (mikStart << 2);
static constexpr MetaInterfaceKind mikMetaEnum = (mikStart << 3);
static constexpr MetaInterfaceKind mikMetaIndexable = (mikStart << 4);
static constexpr MetaInterfaceKind mikMetaIterable = (mikStart << 5);
static constexpr MetaInterfaceKind mikMetaStreaming = (mikStart << 6);
static constexpr MetaInterfaceKind mikMetaMap = (mikStart << 7);
static constexpr MetaInterfaceKind mikMetaMember = (mikStart << 8);
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

template <MetaInterfaceKind kind_>
struct MakeMetaInterfaceItem
{
};

template <>
struct MakeMetaInterfaceItem <mikMetaClass>
{
	static constexpr MetaInterfaceKind kind = mikMetaClass;

	template <typename T>
	static constexpr MetaInterfaceItem make() {
		using M = DeclareMetaType<T>;

		return {
			kind,
			(MetaInterfaceGetter)&M::getMetaClass
		};
	}
};

template <>
struct MakeMetaInterfaceItem <mikMetaCallable>
{
	static constexpr MetaInterfaceKind kind = mikMetaCallable;

	template <typename T>
	static constexpr MetaInterfaceItem make() {
		using M = DeclareMetaType<T>;

		return {
			kind,
			(MetaInterfaceGetter)&M::getMetaCallable
		};
	}
};

template <>
struct MakeMetaInterfaceItem <mikMetaAccessible>
{
	static constexpr MetaInterfaceKind kind = mikMetaAccessible;

	template <typename T>
	static constexpr MetaInterfaceItem make() {
		using M = DeclareMetaType<T>;

		return {
			kind,
			(MetaInterfaceGetter)&M::getMetaAccessible
		};
	}
};

template <>
struct MakeMetaInterfaceItem <mikMetaEnum>
{
	static constexpr MetaInterfaceKind kind = mikMetaEnum;

	template <typename T>
	static constexpr MetaInterfaceItem make() {
		using M = DeclareMetaType<T>;

		return {
			kind,
			(MetaInterfaceGetter)&M::getMetaEnum
		};
	}
};

template <>
struct MakeMetaInterfaceItem <mikMetaIndexable>
{
	static constexpr MetaInterfaceKind kind = mikMetaIndexable;

	template <typename T>
	static constexpr MetaInterfaceItem make() {
		using M = DeclareMetaType<T>;

		return {
			kind,
			(MetaInterfaceGetter)&M::getMetaIndexable
		};
	}
};

template <>
struct MakeMetaInterfaceItem <mikMetaIterable>
{
	static constexpr MetaInterfaceKind kind = mikMetaIterable;

	template <typename T>
	static constexpr MetaInterfaceItem make() {
		using M = DeclareMetaType<T>;

		return {
			kind,
			(MetaInterfaceGetter)&M::getMetaIterable
		};
	}
};

template <>
struct MakeMetaInterfaceItem <mikMetaStreaming>
{
	static constexpr MetaInterfaceKind kind = mikMetaStreaming;

	template <typename T>
	static constexpr MetaInterfaceItem make() {
		using M = typename std::conditional<
			HasMember_getMetaStreaming<DeclareMetaType<T> >::value,
			DeclareMetaType<T>,
			CommonDeclareMetaType<T>
		>::type;

		return {
			kind,
			(MetaInterfaceGetter)&M::getMetaStreaming
		};
	}
};

template <>
struct MakeMetaInterfaceItem <mikMetaMap>
{
	static constexpr MetaInterfaceKind kind = mikMetaMap;

	template <typename T>
	static constexpr MetaInterfaceItem make() {
		using M = DeclareMetaType<T>;

		return {
			kind,
			(MetaInterfaceGetter)&M::getMetaMap
		};
	}
};

template <>
struct MakeMetaInterfaceItem <mikMetaMember>
{
	static constexpr MetaInterfaceKind kind = mikMetaMember;

	template <typename T>
	static constexpr MetaInterfaceItem make() {
		using M = DeclareMetaType<T>;

		return {
			kind,
			(MetaInterfaceGetter)&M::getMetaMember
		};
	}
};

template <>
struct MakeMetaInterfaceItem <mikMetaUser>
{
	static constexpr MetaInterfaceKind kind = mikMetaUser;

	template <typename T>
	static constexpr MetaInterfaceItem make() {
		using M = DeclareMetaType<T>;

		return {
			kind,
			(MetaInterfaceGetter)&M::getMetaUser
		};
	}
};

template <typename T>
struct MakeMetaInterfaceData
{
	using M = DeclareMetaType<T>;

	using ItemMakerList = typename FilterTypes<
		TypeList<
		MakeMetaInterfaceItem <mikMetaClass>,
		MakeMetaInterfaceItem <mikMetaCallable>,
		MakeMetaInterfaceItem <mikMetaAccessible>,
		MakeMetaInterfaceItem <mikMetaEnum>,
		MakeMetaInterfaceItem <mikMetaIndexable>,
		MakeMetaInterfaceItem <mikMetaIterable>,
		MakeMetaInterfaceItem <mikMetaStreaming>,
		MakeMetaInterfaceItem <mikMetaMap>,
		MakeMetaInterfaceItem <mikMetaMember>,
		MakeMetaInterfaceItem <mikMetaUser>
		>,
		BoolConstantList<
		HasMember_getMetaClass<M>::value,
		HasMember_getMetaCallable<M>::value,
		HasMember_getMetaAccessible<M>::value,
		HasMember_getMetaEnum<M>::value,
		HasMember_getMetaIndexable<M>::value,
		HasMember_getMetaIterable<M>::value,
		HasMember_getMetaStreaming<M>::value,
		HasMember_getMetaMap<M>::value,
		HasMember_getMetaMember<M>::value,
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
	void * (*constructData)(MetaTypeData * data, const void * copyFrom);

	void (*destroy)(void * instance);

	bool (*cast)(Variant * result, const Variant & value, const MetaType * toMetaType);

	bool (*castFrom)(Variant * result, const Variant & value, const MetaType * fromMetaType);

	MetaInterfaceData metaInterfaceData;
};

struct MetaTable
{
	Variant (*toReference)(const Variant & value);
};

struct UpTypeData
{
	const MetaType ** upTypeList;
};

class UnifiedType
{
private:
	UnifiedType() = delete;
	UnifiedType(const UnifiedType &) = delete;
	UnifiedType(UnifiedType &&) = delete;

	~UnifiedType() = default;

	TypeKind getTypeKind() const noexcept;

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

	void * constructData(MetaTypeData * data, const void * copyFrom) const;

	void destroy(void * instance) const;

	bool cast(Variant * result, const Variant & value, const MetaType * toMetaType) const;

	bool castFrom(Variant * result, const Variant & value, const MetaType * fromMetaType) const;

private:
	constexpr UnifiedType(
		const TypeKind typeKind,
		const internal_::UnifiedMetaTable & metaMethodTable
	) noexcept;

	template <typename T>
	friend const UnifiedType * unifiedTypeGetter();
	friend class metapp::MetaType;

	const void * doGetMetaInterface(const internal_::MetaInterfaceKind kind) const;

private:
	TypeKind typeKind;
	internal_::UnifiedMetaTable metaMethodTable;
};

inline constexpr UnifiedType::UnifiedType(
	const TypeKind typeKind,
	const internal_::UnifiedMetaTable & metaMethodTable
) noexcept
	: typeKind(typeKind), metaMethodTable(metaMethodTable)
{
}

inline TypeKind UnifiedType::getTypeKind() const noexcept
{
	return typeKind;
}

inline const MetaClass * UnifiedType::getMetaClass() const
{
	return static_cast<const MetaClass *>(doGetMetaInterface(internal_::mikMetaClass));
}

inline const MetaCallable * UnifiedType::getMetaCallable() const
{
	return static_cast<const MetaCallable *>(doGetMetaInterface(internal_::mikMetaCallable));
}

inline const MetaAccessible * UnifiedType::getMetaAccessible() const
{
	return static_cast<const MetaAccessible *>(doGetMetaInterface(internal_::mikMetaAccessible));
}

inline const MetaEnum * UnifiedType::getMetaEnum() const
{
	return static_cast<const MetaEnum *>(doGetMetaInterface(internal_::mikMetaEnum));
}

inline const MetaIndexable * UnifiedType::getMetaIndexable() const
{
	return static_cast<const MetaIndexable *>(doGetMetaInterface(internal_::mikMetaIndexable));
}

inline const MetaIterable * UnifiedType::getMetaIterable() const
{
	return static_cast<const MetaIterable *>(doGetMetaInterface(internal_::mikMetaIterable));
}

inline const MetaStreaming * UnifiedType::getMetaStreaming() const
{
	return static_cast<const MetaStreaming *>(doGetMetaInterface(internal_::mikMetaStreaming));
}

inline const MetaMap * UnifiedType::getMetaMap() const
{
	return static_cast<const MetaMap *>(doGetMetaInterface(internal_::mikMetaMap));
}

inline const MetaMember * UnifiedType::getMetaMember() const
{
	return static_cast<const MetaMember *>(doGetMetaInterface(internal_::mikMetaMember));
}

inline const void * UnifiedType::getMetaUser() const
{
	return static_cast<const void *>(doGetMetaInterface(internal_::mikMetaUser));
}

inline const void * UnifiedType::doGetMetaInterface(const internal_::MetaInterfaceKind kind) const
{
	if((kind & metaMethodTable.metaInterfaceData.kinds) != 0) {
		if(metaMethodTable.metaInterfaceData.items[0].kind == kind) {
			return metaMethodTable.metaInterfaceData.items[0].getter();
		}
		const uint32_t count = (metaMethodTable.metaInterfaceData.kinds & metaInterfaceCountMask);
		if(count > 1) {
			for(uint16_t i = 1; i < count; ++i) {
				if(metaMethodTable.metaInterfaceData.items[i].kind == kind) {
					return metaMethodTable.metaInterfaceData.items[i].getter();
				}
			}
		}
	}
	return nullptr;
}

inline void * UnifiedType::constructData(MetaTypeData * data, const void * copyFrom) const
{
	return metaMethodTable.constructData(data, copyFrom);
}

inline void UnifiedType::destroy(void * instance) const
{
	metaMethodTable.destroy(instance);
}

inline bool UnifiedType::cast(Variant * result, const Variant & value, const MetaType * toMetaType) const
{
	return metaMethodTable.cast(result, value, toMetaType);
}

inline bool UnifiedType::castFrom(Variant * result, const Variant & value, const MetaType * fromMetaType) const
{
	if(metaMethodTable.castFrom == nullptr) {
		return false;
	}
	return metaMethodTable.castFrom(result, value, fromMetaType);
}

template <typename P>
inline Variant doPointerToReference(const Variant & value, std::true_type)
{
	return value;
}

template <typename P>
inline Variant doPointerToReference(const Variant & value, std::false_type)
{
	return Variant::create<P &>(**(P **)value.getAddress());
}

template <typename T, typename Enabled = void>
struct ToReferenceBase;

template <typename T>
struct ToReferenceBase <T,
	typename std::enable_if<std::is_reference<T>::value>::type
>
{
	static Variant toReference(const Variant & value)
	{
		return value;
	}
};

template <typename T>
struct ToReferenceBase <T,
	typename std::enable_if<std::is_pointer<T>::value>::type
>
{
	static Variant toReference(const Variant & value)
	{
		using P = typename std::remove_pointer<T>::type;
		return internal_::doPointerToReference<P>(value, std::is_void<P>());
	}
};

template <typename T>
struct ToReferenceBase <T,
	typename std::enable_if<! std::is_reference<T>::value && ! std::is_pointer<T>::value>::type
>
{
	static Variant toReference(const Variant & value)
	{
		return Variant::create<T &>(value.get<T &>());
	}
};


} // namespace internal_


} // namespace metapp

#endif
