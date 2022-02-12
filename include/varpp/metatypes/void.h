#ifndef VOID_H_969872685611
#define VOID_H_969872685611

#include "varpp/metatype.h"

namespace varpp {

template <>
struct DeclareMetaType <void> : public DeclarePodMetaType<void>
{
	static constexpr TypeKind typeKind = tkVoid;

	static void construct(VariantData & /*data*/, const void * /*value*/) {
	}

	static const void * getAddress(const VariantData & /*data*/) {
		return nullptr;
	}
};


} // namespace varpp


#endif

