#ifndef SHARED_PTR_H_969872685611
#define SHARED_PTR_H_969872685611

#include "metapp/metatype.h"

#include <memory>

namespace metapp {

template <typename T>
struct DeclareMetaType <std::shared_ptr<T> > : public DeclareObjectMetaType<std::shared_ptr<T> >
{
public:
	using UpType = T;
	static constexpr TypeKind typeKind = tkSharedPtr;

	static void construct(MetaTypeData & data, const void * value) {
		data.object = std::static_pointer_cast<void>(*(std::shared_ptr<T> *)value);
	}

	static const void * getAddress(const MetaTypeData & data) {
		return &data.object;
	}

};


} // namespace metapp


#endif

