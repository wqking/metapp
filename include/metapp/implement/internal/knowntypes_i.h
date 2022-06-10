// metapp library
// 
// Copyright (C) 2022 Wang Qi (wqking)
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef METAPP_KNOWNTYPES_I_H_969872685611
#define METAPP_KNOWNTYPES_I_H_969872685611

#include "metapp/utilities/typelist.h"
#include "metapp/compiler.h"

#include <string>

#ifdef METAPP_SUPPORT_STANDARD_17
#include <any>
#endif

namespace metapp {

namespace internal_ {

#ifdef METAPP_SUPPORT_CHAR8_T
using metappChar8_t = char8_t;
#else
using metappChar8_t = char;
#endif

using IntegralTypeList = TypeList<
	bool,
	char, wchar_t,
	metappChar8_t,
	char16_t, char32_t,
	signed char, unsigned char,
	short, unsigned short,
	int, unsigned int,
	long, unsigned long,
	long long, unsigned long long
>;

using FloatTypeList = TypeList<
	float, double, long double
>;

using ArithmeticTypeList = typename TypeListConcat<
	IntegralTypeList,
	FloatTypeList
>::Type;



} // namespace internal_

} // namespace metapp

#endif
