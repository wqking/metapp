#ifndef ARRAY_H_969872685611
#define ARRAY_H_969872685611

#include "metapp/metatype.h"

#include <array>

namespace metapp {

template <typename T, size_t Size>
struct DeclareMetaType <std::array<T, Size> >
	: public DeclareMetaTypeBase <std::array<T, Size> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkArray;

};


} // namespace metapp


#endif

