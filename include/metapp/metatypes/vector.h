#ifndef VECTOR_H_969872685611
#define VECTOR_H_969872685611

#include "metapp/metatype.h"

#include <vector>

namespace metapp {

template <typename T, typename Allocator>
struct BaseDeclareMetaType <std::vector<T, Allocator> >
	: public DeclareObjectMetaType<std::vector<T, Allocator> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkVector;

};


} // namespace metapp


#endif

