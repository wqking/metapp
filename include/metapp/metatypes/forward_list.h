#ifndef FORWARD_LIST_H_969872685611
#define FORWARD_LIST_H_969872685611

#include "metapp/metatype.h"

#include <forward_list>

namespace metapp {

template <typename T, typename Alloc>
struct BaseDeclareMetaType <std::forward_list<T, Alloc> > : public DeclareObjectMetaType<std::forward_list<T, Alloc> >
{
public:
	using UpType = T;
	static constexpr TypeKind typeKind = tkForwardList;

};


} // namespace metapp


#endif

