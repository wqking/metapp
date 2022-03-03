#ifndef METAPP_STD_DEQUE_H_969872685611
#define METAPP_STD_DEQUE_H_969872685611

#include "metapp/metatype.h"
#include "metapp/metatypes/utils/indexablebase.h"

#include <deque>

namespace metapp {

template <typename T, typename Allocator>
struct DeclareMetaTypeBase <std::deque<T, Allocator> >
	: public DeclareMetaTypeObject <std::deque<T, Allocator> >,
		public IndexableBase<std::deque<T, Allocator>, T>
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkStdDeque;

};


} // namespace metapp


#endif

