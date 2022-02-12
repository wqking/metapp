#ifndef ARITHMETIC_H_969872685611
#define ARITHMETIC_H_969872685611

#include "varpp/metatype.h"

namespace varpp {

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
struct DeclareMetaType <T,
	typename std::enable_if<TypeListIn<T, internal_::ArithmeticTypeList>::value>::type> : public DeclarePodMetaType<T>
{
	//using CastFunc = void (*)(const MetaTypeData & data, void * toData);
	// this array requires either c++17 or definition in souce file
	/*static constexpr std::array<CastFunc, TypeListCount<internal_::ArithmeticTypeList>::value> castFunctions{
	&internal_::podCast<T, bool>,
	&internal_::podCast<T, char>, &internal_::podCast<T, wchar_t>,
	&internal_::podCast<T, signed char>, &internal_::podCast<T, unsigned char>,
	&internal_::podCast<T, short>, &internal_::podCast<T, unsigned short>,
	&internal_::podCast<T, int>, &internal_::podCast<T, unsigned int>,
	&internal_::podCast<T, long>, &internal_::podCast<T, unsigned long>,
	&internal_::podCast<T, long long>, &internal_::podCast<T, unsigned long long>,
	&internal_::podCast<T, float>, &internal_::podCast<T, double>, &internal_::podCast<T, long double>
	};*/

	static constexpr TypeKind typeKind = TypeKind(tkFundamentalBegin + TypeListIndexOf<T, internal_::ArithmeticTypeList>::value);

	static bool canCast(const MetaType * toMetaType) {
		return toMetaType->getTypeKind() >= tkArithmeticBegin
			&& toMetaType->getTypeKind() <= tkArithmeticEnd;
	}

	static void cast(const MetaTypeData & data, const MetaType * toMetaType, void * toData) {
		//castFunctions[toMetaType->getTypeKind() - tkArithmeticBegin](data, toData);
		switch(toMetaType->getTypeKind()) {
		case tkBool: internal_::podCast<T, bool>(data, toData); break;
		case tkChar: internal_::podCast<T, char>(data, toData); break;
		case tkWideChar: internal_::podCast<T, wchar_t>(data, toData); break;
		case tkSignedChar: internal_::podCast<T, signed char>(data, toData); break;
		case tkUnsignedChar: internal_::podCast<T, unsigned char>(data, toData); break;
		case tkShort: internal_::podCast<T, short>(data, toData); break;
		case tkUnsignedShort: internal_::podCast<T, unsigned short>(data, toData); break;
		case tkInt: internal_::podCast<T, int>(data, toData); break;
		case tkUnsignedInt: internal_::podCast<T, unsigned int>(data, toData); break;
		case tkLong: internal_::podCast<T, long>(data, toData); break;
		case tkUnsignedLong: internal_::podCast<T, unsigned long>(data, toData); break;
		case tkLongLong: internal_::podCast<T, long long>(data, toData); break;
		case tkUnsignedLongLong: internal_::podCast<T, unsigned long long>(data, toData); break;
		case tkFloat: internal_::podCast<T, float>(data, toData); break;
		case tkDouble: internal_::podCast<T, double>(data, toData); break;
		case tkLongDouble: internal_::podCast<T, long double>(data, toData); break;
		}
	}
};


} // namespace varpp


#endif

