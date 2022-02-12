#ifndef REFERENCE_H_969872685611
#define REFERENCE_H_969872685611

#include "varpp/metatype.h"

namespace varpp {

template <typename T>
struct DeclareMetaType <T &> : public DeclareMetaType<T>
{
public:
	using UpType = T;
	static constexpr TypeKind typeKind = tkReference;

	static void construct(MetaTypeData & data, const void * value) {
		data.podAs<T *>() = *(T **)value;
	}

	static const void * getAddress(const MetaTypeData & data) {
		return &data.podAs<T *>();
	}

	static bool canCast(const MetaType * toMetaType) {
		return toMetaType->getTypeKind() == tkReference;
	}

	static void cast(const MetaTypeData & data, const MetaType * /*toMetaType*/, void * toData) {
		internal_::podCast<T *, void *>(data, toData);
	}
};


} // namespace varpp


#endif

