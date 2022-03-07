#ifndef METAPP_VARIANT_METATYPE_H_969872685611
#define METAPP_VARIANT_METATYPE_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <>
struct DeclareMetaTypeBase <Variant>
{
	using Fallback = DeclareMetaTypeObject <Variant>;
	static constexpr TypeKind typeKind = tkVariant;

	static bool canCast(const Variant & value, const MetaType * toMetaType) {
		return Fallback::canCast(value, toMetaType)
			|| value.get<Variant &>().canCast(toMetaType)
		;
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType) {
		if(Fallback::canCast(value, toMetaType)) {
			return Fallback::cast(value, toMetaType);
		}
		else {
			return value.get<Variant &>().cast(toMetaType);
		}
	}

};


} // namespace metapp


#endif

