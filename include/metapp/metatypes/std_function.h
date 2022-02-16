#ifndef STD_FUNCTION_H_969872685611
#define STD_FUNCTION_H_969872685611

#include "metapp/metatype.h"

#include <functional>

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <std::function<T> >
	: public DeclareMetaType<std::function<T> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkStdFunction;

};


} // namespace metapp


#endif

