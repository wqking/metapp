#ifndef CV_H_969872685611
#define CV_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <const T> : public DeclareMetaType<T>
{
	static constexpr TypeFlags typeFlags = tfConst | DeclareMetaTypeBase<T>::typeFlags;
};

template <typename T>
struct DeclareMetaTypeBase <volatile T> : public DeclareMetaType<T>
{
	static constexpr TypeFlags typeFlags = tfVolatile | DeclareMetaTypeBase<T>::typeFlags;
};

template <typename T>
struct DeclareMetaTypeBase <const volatile T> : public DeclareMetaType<T>
{
	static constexpr TypeFlags typeFlags = tfConst | tfVolatile | DeclareMetaTypeBase<T>::typeFlags;
};


} // namespace metapp


#endif

