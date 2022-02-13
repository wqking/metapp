#ifndef LIST_H_969872685611
#define LIST_H_969872685611

#include "metapp/metatype.h"

#include <list>

namespace metapp {

template <typename T, typename Alloc>
struct BaseDeclareMetaType <std::list<T, Alloc> > : public DeclareObjectMetaType<std::list<T, Alloc> >
{
public:
	using UpType = T;
	static constexpr TypeKind typeKind = tkList;

};


} // namespace metapp


#endif

