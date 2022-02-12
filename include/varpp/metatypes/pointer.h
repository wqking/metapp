#ifndef POINTER_H_969872685611
#define POINTER_H_969872685611

#include "varpp/metatype.h"

namespace varpp {

template <typename T>
struct DeclareMetaType <T *> : public DeclarePodMetaType<T *>
{
public:
	using UpType = T;

	static constexpr TypeKind typeKind = tkPointer;

	static bool canCast(const MetaType * toMetaType) {
		return toMetaType->getTypeKind() == tkPointer;
	}

	static void cast(const MetaTypeData & data, const MetaType * /*toMetaType*/, void * toData) {
		internal_::podCast<T *, void *>(data, toData);
	}
};


} // namespace varpp


#endif

