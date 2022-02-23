#ifndef ARRAY_H_969872685611
#define ARRAY_H_969872685611

#include "metapp/metatype.h"
#include "metapp/metaarray.h"

namespace metapp {

template <typename T, typename U, int length>
struct DeclareMetaTypeArrayBase
	: public DeclarePodMetaType <U *>
{
	static constexpr TypeKind typeKind = tkArray;

	static const MetaArray * getMetaArray() {
		static MetaArray metaArray(length);
		return &metaArray;
	}
};

template <typename T>
struct DeclareMetaTypeBase <T[]>
	: public DeclareMetaTypeArrayBase <T[], T, -1>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <T[N]>
	: public DeclareMetaTypeArrayBase <T[N], T, N>
{
};


} // namespace metapp


#endif

