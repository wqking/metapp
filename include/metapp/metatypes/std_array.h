#ifndef METAPP_STD_ARRAY_H_969872685611
#define METAPP_STD_ARRAY_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metaarray.h"

#include <array>

namespace metapp {

template <typename T, size_t length>
struct DeclareMetaTypeBase <std::array<T, length> >
	: public DeclareMetaTypeObject <std::array<T, length> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkStdArray;

	static const MetaArray * getMetaArray() {
		static MetaArray metaArray((int)length);
		return &metaArray;
	}

};


} // namespace metapp


#endif

