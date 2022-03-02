#ifndef METAPP_METATYPE_I_H_969872685611
#define METAPP_METATYPE_I_H_969872685611

#include <type_traits>

namespace metapp {

class MetaType;
class MetaClass;

namespace internal_ {

struct NoneUpType {};

using MetaInterfaceKind = uint16_t;
using MetaInterfaceGetter = const void * (*)();

static constexpr MetaInterfaceKind mikMetaClass = (1 << 0);
static constexpr MetaInterfaceKind mikMetaCallable = (1 << 1);
static constexpr MetaInterfaceKind mikMetaAccessible = (1 << 2);
static constexpr MetaInterfaceKind mikMetaArray = (1 << 3);
static constexpr MetaInterfaceKind mikMetaEnum = (1 << 4);

struct MetaInterfaceItem
{
	MetaInterfaceKind kind;
	MetaInterfaceGetter getter;
};

struct MetaInterfaceData
{
	uint16_t count;
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

	template <typename M>
	static constexpr MetaInterfaceItem make() {
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

	template <typename M>
	static constexpr MetaInterfaceItem make() {
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

	template <typename M>
	static constexpr MetaInterfaceItem make() {
		return {
			kind,
			(MetaInterfaceGetter)&M::getMetaAccessible
		};
	}
};

template <>
struct MakeMetaInterfaceItem <mikMetaArray>
{
	static constexpr MetaInterfaceKind kind = mikMetaArray;

	template <typename M>
	static constexpr MetaInterfaceItem make() {
		return {
			kind,
			(MetaInterfaceGetter)&M::getMetaArray
		};
	}
};

template <>
struct MakeMetaInterfaceItem <mikMetaEnum>
{
	static constexpr MetaInterfaceKind kind = mikMetaEnum;

	template <typename M>
	static constexpr MetaInterfaceItem make() {
		return {
			kind,
			(MetaInterfaceGetter)&M::getMetaEnum
		};
	}
};

template <typename M>
struct MakeMetaInterfaceData
{
	using ItemMakerList = typename FilterTypes<
		TypeList<
		MakeMetaInterfaceItem <mikMetaClass>,
		MakeMetaInterfaceItem <mikMetaCallable>,
		MakeMetaInterfaceItem <mikMetaAccessible>,
		MakeMetaInterfaceItem <mikMetaArray>,
		MakeMetaInterfaceItem <mikMetaEnum>
		>,
		HasFunctionGetMetaClass<M>::value,
		HasFunctionGetMetaCallable<M>::value,
		HasFunctionGetMetaAccessible<M>::value,
		HasFunctionGetMetaArray<M>::value,
		HasFunctionGetMetaEnum<M>::value
	>::Type;

	template <typename TL>
	struct ItemListMaker
	{
		static constexpr MetaInterfaceKind kinds = 0;

		static const MetaInterfaceItem * make() {
			return nullptr;
		}
	};

	template <typename Arg0, typename ...Args>
	struct ItemListMaker <TypeList<Arg0, Args...> >
	{
		static constexpr MetaInterfaceKind kinds = Arg0::kind | ItemListMaker <TypeList<Args...> >::kinds;

		static const MetaInterfaceItem * make() {
			static const std::array<MetaInterfaceItem, sizeof...(Args) + 1> itemList {
				Arg0::template make<M>(),
				Args::template make<M>()...
			};
			return itemList.data();
		}
	};

	static MetaInterfaceData getMetaInterfaceData() {
		return {
			TypeListCount<ItemMakerList>::value,
			ItemListMaker<ItemMakerList>::kinds,
			ItemListMaker<ItemMakerList>::make()
		};
	}

};

struct MetaMethodTable
{
	void * (*constructData)(MetaTypeData * data, const void * copyFrom);

	void (*destroy)(void * instance);

	void * (*getAddress)(const MetaTypeData & data);

	bool (*canCast)(const Variant & value, const MetaType * toMetaType);
	Variant (*cast)(const Variant & value, const MetaType * toMetaType);

	void (*streamIn)(std::istream & stream, Variant & value);
	void (*streamOut)(std::ostream & stream, const Variant & value);

	MetaInterfaceData metaInterfaceData;
};

struct UpTypeData
{
	const MetaType ** upTypeList;
	uint16_t count;
};

} // namespace internal_


} // namespace metapp

#endif
