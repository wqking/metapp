#ifndef VOID_H_969872685611
#define VOID_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <>
struct BaseDeclareMetaType <void> : public DeclareMetaTypeBase<void>
{
	static constexpr TypeKind typeKind = tkVoid;

	static void constructDefault(MetaTypeData & /*data*/) {
	}

	static void constructWith(MetaTypeData & /*data*/, const void * /*value*/) {
	}

};


} // namespace metapp


#endif

