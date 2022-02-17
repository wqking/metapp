#ifndef REFERENCE_H_969872685611
#define REFERENCE_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <T &> : public DeclareMetaTypeRoot<T &>
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkReference;

	static void constructDefault(MetaTypeData & data) {
		data.podAs<T *>() = (T *)nullptr;
	}

	static void constructWith(MetaTypeData & data, const void * value) {
		data.podAs<T *>() = (T *)value;
	}

	static const void * getAddress(const MetaTypeData & data) {
		return data.podAs<void *>();
	}

	static bool canCast(const MetaType * toMetaType) {
		return toMetaType->getTypeKind() == tkReference;
	}

	static Variant cast(const MetaTypeData & data, const MetaType * /*toMetaType*/) {
		return podCast<T *, void *>(data);
	}
};


} // namespace metapp


#endif

