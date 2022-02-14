#ifndef ARITHMETIC_H_969872685611
#define ARITHMETIC_H_969872685611

#include "metapp/metatype.h"

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
			&internal_::podCast<T, bool>,
			&internal_::podCast<T, char>, &internal_::podCast<T, wchar_t>,
			&internal_::podCast<T, signed char>, &internal_::podCast<T, unsigned char>,
			&internal_::podCast<T, short>, &internal_::podCast<T, unsigned short>,
			&internal_::podCast<T, int>, &internal_::podCast<T, unsigned int>,
			&internal_::podCast<T, long>, &internal_::podCast<T, unsigned long>,
			&internal_::podCast<T, long long>, &internal_::podCast<T, unsigned long long>,
			&internal_::podCast<T, float>, &internal_::podCast<T, double>, &internal_::podCast<T, long double>
		};
		return castFunctions;
	}

	static void cast(const MetaTypeData & data, const MetaType * toMetaType, void * toData) {
		getCastFunctions()[toMetaType->getTypeKind() - tkArithmeticBegin](data, toData);
	}

	static void streamIn(std::istream & stream, MetaTypeData & data) {
		internal_::podStreamIn<T>(stream, data);
	}

	static void streamOut(std::ostream & stream, const MetaTypeData & data) {
		internal_::podStreamOut<T>(stream, data);
	}

};


} // namespace metapp


#endif

