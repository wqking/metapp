#ifndef METAPP_STD_TUPLE_H_969872685611
#define METAPP_STD_TUPLE_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metaindexable.h"
#include "metapp/interfaces/metaiterable.h"

#include <tuple>
#include <array>

namespace metapp {

template <typename ...Types>
struct DeclareMetaTypeBase <std::tuple<Types...> >
	: public DeclareMetaTypeObject <std::tuple<Types...> >
{
	using UpType = TypeList<Types...>;
	static constexpr TypeKind typeKind = tkStdTuple;

	static const MetaIndexable * getMetaIndexable() {
		static MetaIndexable metaIndexable(
			&metaIndexableGetSize,
			&metaIndexableGetAt
		);
		return &metaIndexable;
	}

	static const MetaIterable * getMetaIterable() {
		static MetaIterable metaIterable(
			&metaIterableForEach
		);
		return &metaIterable;
	}

private:
	static size_t metaIndexableGetSize(const Variant & /*value*/)
	{
		return sizeof...(Types);
	}

	static Variant metaIndexableGetAt(const Variant & value, const size_t index)
	{
		using IS = typename internal_::MakeIndexSequence<sizeof...(Types)>::Type;
		return doGetAt(value, index, IS());
	}

	template <size_t ...Indexes>
	static Variant doGetAt(const Variant & value, const size_t index, internal_::IndexSequence<Indexes...>)
	{
		using Func = Variant (*)(const Variant & value);

		std::array<Func, sizeof...(Types)> funcList {
			&doGetAtHelper<Indexes>...
		};
		return funcList[index](value);
	}

	template <size_t index>
	static Variant doGetAtHelper(const Variant & value)
	{
		using TupleType = std::tuple<Types...>;

		return std::get<index>(value.get<TupleType &>());
	}

	static void metaIterableForEach(const Variant & value, MetaIterable::Callback callback)
	{
		using IS = typename internal_::MakeIndexSequence<sizeof...(Types)>::Type;
		doForEach(value, callback, IS());
	}

	template <size_t ...Indexes>
	static void doForEach(const Variant & value, MetaIterable::Callback callback, internal_::IndexSequence<Indexes...>)
	{
		using Func = bool (*)(const Variant & value, MetaIterable::Callback callback);

		std::array<Func, sizeof...(Types)> funcList {
			&doForEachHelper<Indexes>...
		};
		for(auto func : funcList) {
			if(! func(value, callback)) {
				break;
			}
		}
	}

	template <size_t index>
	static bool doForEachHelper(const Variant & value, MetaIterable::Callback callback)
	{
		using TupleType = std::tuple<Types...>;

		return callback(Variant::create<
			typename std::tuple_element<index, TupleType>::type &
			>(
				std::get<index>(value.get<TupleType &>())
		));
	}

};


} // namespace metapp


#endif

