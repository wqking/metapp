#ifndef METAPP_KNOWNTYPES_H_969872685611
#define METAPP_KNOWNTYPES_H_969872685611

#include "metapp/utils/typelist.h"
#include "metapp/compiler.h"

#include <string>

#ifdef METAPP_SUPPORT_STANDARD_17
#include <any>
#endif

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
#ifdef METAPP_SUPPORT_STANDARD_17
	std::any,
#endif
	std::string, std::wstring,
	const char *, const wchar_t *,
	char *, wchar_t *,
	char[], wchar_t[]
>;

using AllKnownTypeList = typename TypeListConcat<ArithmeticTypeList, OtherKnowTypeList>::Type;



} // namespace metapp

#endif
