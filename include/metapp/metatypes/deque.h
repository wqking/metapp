#ifndef DEQUE_H_969872685611
#define DEQUE_H_969872685611

#include "metapp/metatype.h"

#include <deque>

namespace metapp {

template <typename T, typename Alloc>
struct BaseDeclareMetaType <std::deque<T, Alloc> > : public DeclareObjectMetaType<std::deque<T, Alloc> >
{
public:
	using UpType = T;
	static constexpr TypeKind typeKind = tkDeque;

};


} // namespace metapp


#endif

