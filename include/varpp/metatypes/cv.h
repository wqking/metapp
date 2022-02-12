#ifndef CV_H_969872685611
#define CV_H_969872685611

#include "varpp/metatype.h"

namespace varpp {

template <typename T>
struct DeclareMetaType <const T> : public DeclareMetaType<T>
{
	static constexpr QualifierKind qualifiers = qkConst;
};

template <typename T>
struct DeclareMetaType <volatile T> : public DeclareMetaType<T>
{
	static constexpr QualifierKind qualifiers = qkVolatile;
};

template <typename T>
struct DeclareMetaType <const volatile T> : public DeclareMetaType<T>
{
	static constexpr QualifierKind qualifiers = qkConst | qkVolatile;
};


} // namespace varpp


#endif

