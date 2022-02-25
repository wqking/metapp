#ifndef ARRAY_H_969872685611
#define ARRAY_H_969872685611

#include "metapp/metatype.h"
#include "metapp/metaarray.h"

namespace metapp {

template <typename T, typename U, int length>
struct DeclareMetaTypeArrayBase
	: public DeclarePodMetaType <U>
{
	static constexpr TypeKind typeKind = tkArray;
	static constexpr TypeFlags typeFlags = DeclarePodMetaType<U>::typeFlags
		| (std::is_const<U>::value ? tfConst : 0)
		| (std::is_volatile<U>::value ? tfVolatile : 0)
	;

	static const MetaArray * getMetaArray() {
		static MetaArray metaArray(length);
		return &metaArray;
	}
};

template <typename T>
struct DeclareMetaTypeBase <T[]>
	: public DeclareMetaTypeArrayBase <T[], T *, -1>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <T[N]>
	: public DeclareMetaTypeArrayBase <T[N], T *, N>
{
};

template <typename T>
struct DeclareMetaTypeBase <const T[]>
	: public DeclareMetaTypeArrayBase <const T[], T * const, -1>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <const T[N]>
	: public DeclareMetaTypeArrayBase <const T[N], T * const, N>
{
};

template <typename T>
struct DeclareMetaTypeBase <volatile T[]>
	: public DeclareMetaTypeArrayBase <volatile T[], T * volatile, -1>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <volatile T[N]>
	: public DeclareMetaTypeArrayBase <volatile T[N], T * volatile, N>
{
};

template <typename T>
struct DeclareMetaTypeBase <const volatile T[]>
	: public DeclareMetaTypeArrayBase <const volatile T[], T * const volatile, -1>
{
};

template <typename T, int N>
struct DeclareMetaTypeBase <const volatile T[N]>
	: public DeclareMetaTypeArrayBase <const volatile T[N], T * const volatile, N>
{
};


} // namespace metapp


#endif

