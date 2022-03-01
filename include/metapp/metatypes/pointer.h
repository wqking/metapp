#ifndef POINTER_H_969872685611
#define POINTER_H_969872685611

#include "metapp/metatype.h"
#include "metapp/metaaccessible.h"
#include "metapp/metatypes/utils/declareutil.h"

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <T *> : public DeclareMetaTypeObject<T *>
{
private:
	using super = DeclareMetaTypeObject<T *>;

public:
	using UpType = T;

	static constexpr TypeKind typeKind = tkPointer;

	static bool canCast(const MetaType * toMetaType) {
		return toMetaType->getTypeKind() == tkPointer || super::canCast(toMetaType);
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType) {
		if(toMetaType->getTypeKind() == tkPointer) {
			return Variant(toMetaType, value.getMetaTypeData());
		}
		else {
			return super::cast(value, toMetaType);
		}
	}

};

template <>
struct DeclareMetaTypeBase <std::nullptr_t> : public DeclareMetaTypeBase<void *>
{
};


} // namespace metapp


#endif

