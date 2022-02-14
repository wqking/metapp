#ifndef ARITHMETIC_H_969872685611
#define ARITHMETIC_H_969872685611

#include "metapp/metatype.h"
#include "metapp/typelist.h"

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
struct BaseDeclareMetaType <T,
	typename std::enable_if<TypeListIn<T, internal_::ArithmeticTypeList>::value>::type> : public DeclarePodMetaType<T>
{
	static constexpr TypeKind typeKind = TypeKind(tkFundamentalBegin + TypeListIndexOf<T, internal_::ArithmeticTypeList>::value);

	static bool canCast(const MetaType * toMetaType) {
		return toMetaType->getTypeKind() >= tkArithmeticBegin
			&& toMetaType->getTypeKind() <= tkArithmeticEnd;
	}

	using CastFunc = void (*)(const MetaTypeData & data, void * toData);
	static const std::array<CastFunc, TypeListCount<internal_::ArithmeticTypeList>::value> & getCastFunctions() {
		static const std::array<CastFunc, TypeListCount<internal_::ArithmeticTypeList>::value> castFunctions {
			&podCast<T, bool>,
			&podCast<T, char>, &podCast<T, wchar_t>,
			&podCast<T, signed char>, &podCast<T, unsigned char>,
			&podCast<T, short>, &podCast<T, unsigned short>,
			&podCast<T, int>, &podCast<T, unsigned int>,
			&podCast<T, long>, &podCast<T, unsigned long>,
			&podCast<T, long long>, &podCast<T, unsigned long long>,
			&podCast<T, float>, &podCast<T, double>, &podCast<T, long double>
		};
		return castFunctions;
	}

	static void cast(const MetaTypeData & data, const MetaType * toMetaType, void * toData) {
		getCastFunctions()[toMetaType->getTypeKind() - tkArithmeticBegin](data, toData);
	}

};


} // namespace metapp


#endif

