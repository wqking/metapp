#ifndef METAPP_STD_LIST_H_969872685611
#define METAPP_STD_LIST_H_969872685611

#include "metapp/metatype.h"
#include "metapp/metatypes/utils/iterablebase.h"

#include <list>

namespace metapp {

template <typename T, typename Allocator>
struct DeclareMetaTypeBase <std::list<T, Allocator> >
	: public DeclareMetaTypeObject <std::list<T, Allocator> >,
		public IterableBase<std::list<T, Allocator> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkStdList;

};


} // namespace metapp


#endif

