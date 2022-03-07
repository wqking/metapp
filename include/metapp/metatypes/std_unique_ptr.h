#ifndef METAPP_STD_UNIQUE_PTR_H_969872685611
#define METAPP_STD_UNIQUE_PTR_H_969872685611

#include "metapp/metatype.h"

#include <memory>

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <std::unique_ptr<T> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkStdUniquePtr;

};


} // namespace metapp


#endif

