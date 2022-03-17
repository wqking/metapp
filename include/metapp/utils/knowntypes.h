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
