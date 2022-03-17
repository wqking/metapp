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

#ifndef METAPP_FUNCTION_H_969872685611
#define METAPP_FUNCTION_H_969872685611

#include "metapp/interfaces/bases/metacallablebase.h"
#include "metapp/compiler.h"

namespace metapp {

template <typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (*)(Args...)>
	: public MetaCallableBase<RT (*)(Args...), void, RT, Args...>
{
public:
	using UpType = TypeList<RT, Args...>;
	static constexpr TypeKind typeKind = tkFunction;

};

template <typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Args...)>
	: public DeclareMetaTypeBase <RT (*)(Args...)>
{
};

#ifdef METAPP_NOEXCEPT_BELONGS_TO_FUNCTION_TYPE

template <typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (*)(Args...) noexcept>
	: public DeclareMetaTypeBase <RT (*)(Args...)>
{
};

template <typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Args...) noexcept>
	: public DeclareMetaTypeBase <RT (Args...)>
{
};

#endif


} // namespace metapp


#endif

