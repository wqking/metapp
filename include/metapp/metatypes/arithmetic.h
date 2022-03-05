#ifndef METAPP_ARITHMETIC_H_969872685611
#define METAPP_ARITHMETIC_H_969872685611

#include "metapp/metatype.h"
#include "metapp/utils/typelist.h"
#include "metapp/utils/knowntypes.h"

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <T,
	typename std::enable_if<TypeListIn<ArithmeticTypeList, T>::value>::type> : public DeclareMetaTypeObject<T>
{
private:
	using super = DeclareMetaTypeObject<T>;

	static bool doCanCastArithmetic(const MetaType * toMetaType) {
		return (toMetaType->getTypeKind() >= tkArithmeticBegin
			&& toMetaType->getTypeKind() <= tkArithmeticEnd);
	}

public:
	static constexpr TypeKind typeKind = TypeKind(tkFundamentalBegin + TypeListIndexOf<ArithmeticTypeList, T>::value);

	static bool canCast(const Variant & value, const MetaType * toMetaType) {
		return doCanCastArithmetic(toMetaType) || super::canCast(value, toMetaType);
	}

	using CastFunc = Variant (*)(const Variant & value);
	template <typename ...Types>
	static const CastFunc * getCastFunctions(TypeList<Types...>) {
		static const std::array<CastFunc, TypeListCount<ArithmeticTypeList>::value> castFunctions {
			&variantCast<T, Types>...
		};
		return castFunctions.data();
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType) {
		if(doCanCastArithmetic(toMetaType)) {
			return getCastFunctions(ArithmeticTypeList())[toMetaType->getTypeKind() - tkArithmeticBegin](value);
		}
		else {
			return super::cast(value, toMetaType);
		}
	}

};


} // namespace metapp


#endif

