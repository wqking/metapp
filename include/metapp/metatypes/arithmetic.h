#ifndef ARITHMETIC_H_969872685611
#define ARITHMETIC_H_969872685611

#include "metapp/metatype.h"
#include "metapp/utilities//typelist.h"

namespace metapp {

namespace internal_ {

using ArithmeticTypeList = TypeList<
	bool,
	char, wchar_t,
	signed char, unsigned char,
	short, unsigned short,
	int, unsigned int,
	long, unsigned long,
	long long, unsigned long long,
	float, double, long double
>;

} // namespace internal_

template <typename T>
struct DeclareMetaTypeBase <T,
	typename std::enable_if<TypeListIn<internal_::ArithmeticTypeList, T>::value>::type> : public DeclareMetaTypeObject<T>
{
private:
	using super = DeclareMetaTypeObject<T>;

	static bool doCanCastArithmetic(const MetaType * toMetaType) {
		return (toMetaType->getTypeKind() >= tkArithmeticBegin
			&& toMetaType->getTypeKind() <= tkArithmeticEnd);
	}

public:
	static constexpr TypeKind typeKind = TypeKind(tkFundamentalBegin + TypeListIndexOf<internal_::ArithmeticTypeList, T>::value);

	static bool canCast(const Variant & value, const MetaType * toMetaType) {
		return doCanCastArithmetic(toMetaType) || super::canCast(value, toMetaType);
	}

	using CastFunc = Variant (*)(const Variant & value);
	static const CastFunc * getCastFunctions() {
		static const std::array<CastFunc, TypeListCount<internal_::ArithmeticTypeList>::value> castFunctions {
			&variantCast<T, bool>,
			&variantCast<T, char>, &variantCast<T, wchar_t>,
			&variantCast<T, signed char>, &variantCast<T, unsigned char>,
			&variantCast<T, short>, &variantCast<T, unsigned short>,
			&variantCast<T, int>, &variantCast<T, unsigned int>,
			&variantCast<T, long>, &variantCast<T, unsigned long>,
			&variantCast<T, long long>, &variantCast<T, unsigned long long>,
			&variantCast<T, float>, &variantCast<T, double>, &variantCast<T, long double>
		};
		return castFunctions.data();
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType) {
		if(doCanCastArithmetic(toMetaType)) {
			return getCastFunctions()[toMetaType->getTypeKind() - tkArithmeticBegin](value);
		}
		else {
			return super::cast(value, toMetaType);
		}
	}

};


} // namespace metapp


#endif

