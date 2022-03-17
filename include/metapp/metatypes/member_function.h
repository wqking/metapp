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

#ifndef METAPP_MEMBER_FUNCTION_H_969872685611
#define METAPP_MEMBER_FUNCTION_H_969872685611

#include "metapp/interfaces/bases/metacallablebase.h"
#include "metapp/interfaces/bases/metamemberbase.h"
#include "metapp/compiler.h"

namespace metapp {

template <typename FullType, typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeMemberFunctionBase
	: public MetaCallableBase<FullType, Class, RT, Args...>,
		public MetaMemberBase<Class>
{
public:
	using UpType = TypeList <Class, RT, Args...>;
	static constexpr TypeKind typeKind = tkMemberFunction;

};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...)>
	: public DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...), Class, RT, Args...>
{
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) const>
	: public DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) const, Class, RT, Args...>
{
	static constexpr TypeFlags typeFlags = tfConst;
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) volatile>
	: public DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) volatile, Class, RT, Args...>
{
	static constexpr TypeFlags typeFlags = tfVolatile;
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) const volatile>
	: public DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) const volatile, Class, RT, Args...>
{
	static constexpr TypeFlags typeFlags = tfConst | tfVolatile;
};

#ifdef METAPP_NOEXCEPT_BELONGS_TO_FUNCTION_TYPE

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) noexcept>
	: public DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) noexcept, Class, RT, Args...>
{
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) const noexcept>
	: public DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) const noexcept, Class, RT, Args...>
{
	static constexpr TypeFlags typeFlags = tfConst;
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) volatile noexcept>
	: public DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) volatile noexcept, Class, RT, Args...>
{
	static constexpr TypeFlags typeFlags = tfVolatile;
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) const volatile noexcept>
	: public DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) const volatile noexcept, Class, RT, Args...>
{
	static constexpr TypeFlags typeFlags = tfConst | tfVolatile;
};

#endif


} // namespace metapp


#endif

