#ifndef METAPP_STD_ARRAY_H_969872685611
#define METAPP_STD_ARRAY_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metaindexable.h"
#include "metapp/metatypes/utils/metaindexablebase.h"
#include "metapp/metatypes/utils/metaiterablebase.h"

#include <array>

namespace metapp {

template <typename T, size_t length>
struct DeclareMetaTypeBase <std::array<T, length> >
	: public DeclareMetaTypeObject <std::array<T, length> >,
		public MetaIndexableBase<std::array<T, length> >,
		public MetaIterableBase<std::array<T, length> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkStdArray;

	using ArrayType = std::array<T, length>;

};


} // namespace metapp


#endif

