#ifndef METAPP_STD_VECTOR_H_969872685611
#define METAPP_STD_VECTOR_H_969872685611

#include "metapp/metatype.h"
#include "metapp/metatypes/utils/indexablebase.h"
#include "metapp/metatypes/utils/iterablebase.h"

#include <vector>

namespace metapp {

template <typename T, typename Allocator>
struct DeclareMetaTypeBase <std::vector<T, Allocator> >
	: public DeclareMetaTypeObject <std::vector<T, Allocator> >,
		public IndexableBase<std::vector<T, Allocator> >,
		public IterableBase<std::vector<T, Allocator> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkStdVector;

	using VectorType = std::vector<T, Allocator>;

};


} // namespace metapp


#endif

