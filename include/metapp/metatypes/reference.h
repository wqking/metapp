#ifndef REFERENCE_H_969872685611
#define REFERENCE_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <typename T>
struct BaseDeclareMetaType <T &> : public DeclareMetaTypeBase<T>
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

	static void cast(const MetaTypeData & data, const MetaType * /*toMetaType*/, void * toData) {
		internal_::podCast<T *, void *>(data, toData);
	}
};


} // namespace metapp


#endif

