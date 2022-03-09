#ifndef METAPP_VARIANT_METATYPE_H_969872685611
#define METAPP_VARIANT_METATYPE_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <>
struct DeclareMetaTypeBase <Variant>
{
	using Common = CommonDeclareMetaType <Variant>;
	static constexpr TypeKind typeKind = tkVariant;

	static bool canCast(const Variant & value, const MetaType * toMetaType) {
		return Common::canCast(value, toMetaType)
			|| value.get<Variant &>().canCast(toMetaType)
		;
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType) {
		if(Common::canCast(value, toMetaType)) {
			return Common::cast(value, toMetaType);
		}
		else {
			return value.get<Variant &>().cast(toMetaType);
		}
	}

};


} // namespace metapp


#endif

