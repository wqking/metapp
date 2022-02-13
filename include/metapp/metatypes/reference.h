#ifndef REFERENCE_H_969872685611
#define REFERENCE_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <typename T>
struct DeclareMetaType <T &> : public DeclareReferenceMetaType<T &>
{
public:
	using UpType = T;
	static constexpr TypeKind typeKind = tkReference;

	static bool canCast(const MetaType * toMetaType) {
		return toMetaType->getTypeKind() == tkReference;
	}

	static void cast(const MetaTypeData & data, const MetaType * /*toMetaType*/, void * toData) {
		internal_::podCast<T *, void *>(data, toData);
	}
};


} // namespace metapp


#endif

