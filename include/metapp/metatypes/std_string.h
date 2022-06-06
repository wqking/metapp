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

#ifndef METAPP_STD_STRING_H_969872685611
#define METAPP_STD_STRING_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/bases/metastreamablebase.h"
#include "metapp/utilities/utility.h"

#include <string>

namespace metapp {

template <>
struct DeclareMetaTypeBase <std::string> : MetaStreamableBase <std::string>
{
	static constexpr TypeKind typeKind = tkStdString;

};

template <>
struct DeclareMetaTypeBase <std::wstring>
{
	static constexpr TypeKind typeKind = tkStdWideString;

};


} // namespace metapp


#endif

