#ifndef VARIANT_METATYPE_H_969872685611
#define VARIANT_METATYPE_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <>
struct DeclareMetaTypeBase <Variant>
	: public DeclareMetaTypeObject <Variant>
{
private:
	using super = DeclareMetaTypeObject <Variant>;

public:
	static constexpr TypeKind typeKind = tkVariant;

	static bool canCast(const Variant & value, const MetaType * toMetaType) {
		return super::canCast(value, toMetaType)
			|| value.get<Variant &>().canCast(toMetaType)
		;
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType) {
		if(super::canCast(value, toMetaType)) {
			return super::cast(value, toMetaType);
		}
		else {
			return value.get<Variant &>().cast(toMetaType);
		}
	}

};


} // namespace metapp


#endif

