#ifndef METAPP_ARITHMETIC_H_969872685611
#define METAPP_ARITHMETIC_H_969872685611

#include "metapp/metatype.h"
#include "metapp/utils/typelist.h"
#include "metapp/utils/knowntypes.h"

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <T,
	typename std::enable_if<TypeListIn<ArithmeticTypeList, T>::value>::type>
{
	static constexpr TypeKind typeKind = TypeKind(tkFundamentalBegin + TypeListIndexOf<ArithmeticTypeList, T>::value);

};


} // namespace metapp


#endif

