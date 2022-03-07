#ifndef METAPP_STD_DEQUE_H_969872685611
#define METAPP_STD_DEQUE_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/bases/metaindexablebase.h"
#include "metapp/interfaces/bases/metaiterablebase.h"

#include <deque>

namespace metapp {

template <typename T, typename Allocator>
struct DeclareMetaTypeBase <std::deque<T, Allocator> >
	: public MetaIndexableBase<std::deque<T, Allocator> >,
		public MetaIterableBase<std::deque<T, Allocator> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkStdDeque;

};


} // namespace metapp


#endif

