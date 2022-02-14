#ifndef SHARED_PTR_H_969872685611
#define SHARED_PTR_H_969872685611

#include "metapp/metatype.h"

#include <memory>

namespace metapp {

template <typename T>
struct BaseDeclareMetaType <std::shared_ptr<T> > : DeclareMetaTypeBase<std::shared_ptr<T> >
{
public:
	using UpType = T;
	static constexpr TypeKind typeKind = tkSharedPtr;

	static void constructDefault(MetaTypeData & data) {
		data.object = std::static_pointer_cast<void>(std::shared_ptr<T>());
	}

	static void constructWith(MetaTypeData & data, const void * value) {
		data.object = std::static_pointer_cast<void>(*(std::shared_ptr<T> *)value);
	}

	static const void * getAddress(const MetaTypeData & data) {
		return &data.object;
	}

};


} // namespace metapp


#endif

