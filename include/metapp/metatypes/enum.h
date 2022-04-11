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

#ifndef METAPP_ENUM_H_969872685611
#define METAPP_ENUM_H_969872685611

#include "metapp/metatype.h"
#include "metapp/cast.h"
#include "metapp/interfaces/metaenum.h"
#include "metapp/implement/internal/knowntypes_i.h"

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <T, typename std::enable_if<std::is_enum<T>::value>::type>
	: CastFromToTypes<T, internal_::IntegralTypeList>
{
	using UpType = typename std::underlying_type<T>::type;
	static constexpr TypeKind typeKind = tkEnum;
};


} // namespace metapp


#endif

