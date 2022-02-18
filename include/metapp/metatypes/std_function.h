#ifndef STD_FUNCTION_H_969872685611
#define STD_FUNCTION_H_969872685611

#include "metapp/metatype.h"

#include <functional>

namespace metapp {

template <typename T>
struct DeclareMetaType <std::function<T> >
	: public DeclareMetaTypeBase <std::function<T> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkStdFunction;

};


} // namespace metapp


#endif

