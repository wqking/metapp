#ifndef METAPP_CV_H_969872685611
#define METAPP_CV_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <typename T>
struct DeclareMetaTypeRoot <const T> : public DeclareMetaType<T>
{
	static constexpr TypeFlags typeFlags = tfConst;
};

template <typename T>
struct DeclareMetaTypeRoot <volatile T> : public DeclareMetaType<T>
{
	static constexpr TypeFlags typeFlags = tfVolatile;
};

template <typename T>
struct DeclareMetaTypeRoot <const volatile T> : public DeclareMetaType<T>
{
	static constexpr TypeFlags typeFlags = tfConst | tfVolatile;
};


} // namespace metapp


#endif

