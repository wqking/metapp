#ifndef SHARED_PTR_H_969872685611
#define SHARED_PTR_H_969872685611

#include "metapp/metatype.h"

#include <memory>

namespace metapp {

template <typename T>
struct DeclareMetaType <std::shared_ptr<T> > : public DeclareSharedPtrMetaType<std::shared_ptr<T> >
{
public:
	using UpType = T;
	static constexpr TypeKind typeKind = tkSharedPtr;

};


} // namespace metapp


#endif

