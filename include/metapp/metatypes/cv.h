#ifndef CV_H_969872685611
#define CV_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <typename T>
struct BaseDeclareMetaType <const T> : public DeclareMetaType<T>
{
	static constexpr TypeFlags typeFlags = tfConst | BaseDeclareMetaType<T>::typeFlags;
};

template <typename T>
struct BaseDeclareMetaType <volatile T> : public DeclareMetaType<T>
{
	static constexpr TypeFlags typeFlags = tfVolatile | BaseDeclareMetaType<T>::typeFlags;
};

template <typename T>
struct BaseDeclareMetaType <const volatile T> : public DeclareMetaType<T>
{
	static constexpr TypeFlags typeFlags = tfConst | tfVolatile | BaseDeclareMetaType<T>::typeFlags;
};


} // namespace metapp


#endif

