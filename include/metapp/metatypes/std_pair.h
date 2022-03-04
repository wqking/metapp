#ifndef METAPP_STD_PAIR_H_969872685611
#define METAPP_STD_PAIR_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metaindexable.h"

#include <utility>

namespace metapp {

template <typename T1, typename T2>
struct DeclareMetaTypeBase <std::pair<T1, T2> >
	: public DeclareMetaTypeObject <std::pair<T1, T2> >
{
	using UpType = TypeList<T1, T2>;
	static constexpr TypeKind typeKind = tkStdPair;

	static const MetaIndexable * getMetaIndexable() {
		static MetaIndexable metaIndexable(
			&metaIndexableGetSize,
			&metaIndexableGetAt
		);
		return &metaIndexable;
	}

private:
	static size_t metaIndexableGetSize(const Variant & /*value*/)
	{
		return 2;
	}

	static Variant metaIndexableGetAt(const Variant & value, const size_t index)
	{
		using PairType = std::pair<T1, T2>;

		if(index == 0) {
			return Variant::create<T1>(value.get<PairType &>().first);
		}
		return Variant::create<T2>(value.get<PairType &>().second);
	}

};


} // namespace metapp


#endif

