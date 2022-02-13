#ifndef VECTOR_H_969872685611
#define VECTOR_H_969872685611

#include "metapp/metatype.h"

#include <vector>

namespace metapp {

template <typename T, typename Alloc>
struct BaseDeclareMetaType <std::vector<T, Alloc> > : public DeclareObjectMetaType<std::vector<T, Alloc> >
{
public:
	using UpType = T;
	static constexpr TypeKind typeKind = tkVector;

};


} // namespace metapp


#endif

