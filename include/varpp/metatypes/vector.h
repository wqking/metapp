#ifndef VECTOR_H_969872685611
#define VECTOR_H_969872685611

#include "varpp/metatype.h"

#include <vector>

namespace varpp {

template <typename T, typename Alloc>
struct DeclareMetaType <std::vector<T, Alloc> > : public DeclareObjectMetaType<std::vector<T, Alloc> >
{
public:
	using UpType = T;
	static constexpr TypeKind typeKind = tkVector;

};


} // namespace varpp


#endif

