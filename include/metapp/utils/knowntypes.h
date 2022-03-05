#ifndef METAPP_KNOWNTYPES_H_969872685611
#define METAPP_KNOWNTYPES_H_969872685611

#include "metapp/utils/typelist.h"

#include <string>

namespace metapp {

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

using OtherKnowTypeList = TypeList<
	std::string, std::wstring,
	const char *, const wchar_t 
>;

using AllKnownTypeList = typename TypeListConcat<ArithmeticTypeList, OtherKnowTypeList>::Type;



} // namespace metapp

#endif
