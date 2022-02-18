#ifndef CV_H_969872685611
#define CV_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <const T> : public DeclareMetaType<T>
{
	static constexpr TypeFlags typeFlags = tfConst | DeclareMetaType<T>::typeFlags;
};

template <typename T>
struct DeclareMetaTypeBase <volatile T> : public DeclareMetaType<T>
{
	static constexpr TypeFlags typeFlags = tfVolatile | DeclareMetaType<T>::typeFlags;
};

template <typename T>
struct DeclareMetaTypeBase <const volatile T> : public DeclareMetaType<T>
{
	static constexpr TypeFlags typeFlags = tfConst | tfVolatile | DeclareMetaType<T>::typeFlags;
};


} // namespace metapp


#endif

