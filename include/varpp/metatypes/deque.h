#ifndef DEQUE_H_969872685611
#define DEQUE_H_969872685611

#include "varpp/metatype.h"

#include <deque>

namespace varpp {

template <typename T, typename Alloc>
struct DeclareMetaType <std::deque<T, Alloc> > : public DeclareObjectMetaType<std::deque<T, Alloc> >
{
public:
	using UpType = T;
	static constexpr TypeKind typeKind = tkDeque;

};


} // namespace varpp


#endif

