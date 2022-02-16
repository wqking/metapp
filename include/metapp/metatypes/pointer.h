#ifndef POINTER_H_969872685611
#define POINTER_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <T *> : public DeclarePodMetaType<T *>
{
	using UpType = T;

	static constexpr TypeKind typeKind = tkPointer;

	static bool canCast(const MetaType * toMetaType) {
		return toMetaType->getTypeKind() == tkPointer;
	}

	static void cast(const MetaTypeData & data, const MetaType * /*toMetaType*/, void * toData) {
		podCast<T *, void *>(data, toData);
	}
};

template <>
struct DeclareMetaTypeBase <std::nullptr_t> : public DeclareMetaTypeBase<void *>
{
};


} // namespace metapp


#endif

