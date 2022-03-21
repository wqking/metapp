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

#ifndef METAPP_CV_H_969872685611
#define METAPP_CV_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <typename T>
struct DeclareMetaTypeRoot <const T> : DeclareMetaType<T>
{
	static constexpr TypeFlags typeFlags = tfConst;
};

template <typename T>
struct DeclareMetaTypeRoot <volatile T> : DeclareMetaType<T>
{
	static constexpr TypeFlags typeFlags = tfVolatile;
};

template <typename T>
struct DeclareMetaTypeRoot <const volatile T> : DeclareMetaType<T>
{
	static constexpr TypeFlags typeFlags = tfConst | tfVolatile;
};


} // namespace metapp


#endif

