#ifndef POINTER_H_969872685611
#define POINTER_H_969872685611

#include "varpp/metatype.h"

namespace varpp {

template <typename T>
struct DeclareMetaType <T *> : public DeclarePodMetaType<T *>
{
public:
	// Seems MSVC treats T as pointer, so we need to remove the pointer.
	// All we need to remove all pointers, if there are more than one.
	using UpType = typename std::remove_pointer<T>::type;

	static constexpr TypeKind typeKind = tkPointer;

	static bool canCast(const MetaType * toMetaType) {
		return toMetaType->getVarType() == tkPointer;
	}

	static void cast(const VariantData & data, const MetaType * /*toMetaType*/, void * toData) {
		internal_::podCast<T *, void *>(data, toData);
	}
};


} // namespace varpp


#endif

