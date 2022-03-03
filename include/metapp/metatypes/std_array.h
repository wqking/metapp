#ifndef METAPP_STD_ARRAY_H_969872685611
#define METAPP_STD_ARRAY_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metaindexable.h"

#include <array>

namespace metapp {

template <typename T, size_t length>
struct DeclareMetaTypeBase <std::array<T, length> >
	: public DeclareMetaTypeObject <std::array<T, length> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkStdArray;

	using ArrayType = std::array<T, length>;

	static const MetaIndexable * getMetaIndexable() {
		static MetaIndexable metaIndexable(
			&metaIndexableGetSize,
			&metaIndexableGetAt
		);
		return &metaIndexable;
	}

	static size_t metaIndexableGetSize(const Variant & value)
	{
		return value.get<ArrayType &>().size();
	}

	static Variant metaIndexableGetAt(const Variant & value, const size_t index)
	{
		return Variant::create<T &>(value.get<ArrayType &>()[index]);
	}

};


} // namespace metapp


#endif

