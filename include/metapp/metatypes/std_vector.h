#ifndef METAPP_STD_VECTOR_H_969872685611
#define METAPP_STD_VECTOR_H_969872685611

#include "metapp/metatype.h"

#include <vector>

namespace metapp {

template <typename T, typename Allocator>
struct DeclareMetaTypeBase <std::vector<T, Allocator> >
	: public DeclareMetaTypeObject <std::vector<T, Allocator> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkStdVector;

};


} // namespace metapp


#endif

