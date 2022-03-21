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

#ifndef METAPP_STD_FUNCTION_H_969872685611
#define METAPP_STD_FUNCTION_H_969872685611

#include "metapp/interfaces/bases/metacallablebase.h"

#include <functional>

namespace metapp {

template <typename RT, typename ...Args>
struct DeclareMetaTypeBase <std::function<RT (Args...)> >
	: MetaCallableBase <std::function<RT (Args...)>, void, RT, Args...>
{
	using UpType = TypeList<RT, Args...>;
	static constexpr TypeKind typeKind = tkStdFunction;

};


} // namespace metapp


#endif

