#ifndef VOID_H_969872685611
#define VOID_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <>
struct DeclareMetaTypeBase <void> : public DeclareMetaTypeRoot<void>
{
	static constexpr TypeKind typeKind = tkVoid;

	static const char * getName() {
		return "void";
	}

	static void constructDefault(MetaTypeData & /*data*/) {
	}

	static void constructWith(MetaTypeData & /*data*/, const void * /*value*/) {
	}

};


} // namespace metapp


#endif

