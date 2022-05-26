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
#include "metapp/utilities/utility.h"
#include "metapp/compiler.h"

namespace metapp {

template <typename FullType, typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeMemberFunctionBase : MetaCallableBase<FullType, Class, RT, Args...>
{
private:
	using super = MetaCallableBase<FullType, Class, RT, Args...>;

public:
	using UpType = TypeList <Class, RT, Args...>;
	static constexpr TypeKind typeKind = tkMemberFunction;

	static const MetaCallable * getMetaCallable() {
		static const MetaCallable metaCallable(
			&DeclareMetaTypeMemberFunctionBase::metaCallableGetClassType,
			&DeclareMetaTypeMemberFunctionBase::metaCallableGetParameterCountInfo,
			&DeclareMetaTypeMemberFunctionBase::metaCallableGetReturnType,
			&DeclareMetaTypeMemberFunctionBase::metaCallableGetParameterType,
			&DeclareMetaTypeMemberFunctionBase::metaCallableRankInvoke,
			&DeclareMetaTypeMemberFunctionBase::metaCallableCanInvoke,
			&DeclareMetaTypeMemberFunctionBase::metaCallableInvoke
		);
		return &metaCallable;
	}

	static bool canAccess(const Variant & callable, const Variant & instance)
	{
		return getPointedType(instance)->getConstness().canConvertTo(
			getNonReferenceMetaType(callable)->getConstness()
		);
	}

	static int metaCallableRankInvoke(const Variant & callable, const Variant & instance, const ArgumentSpan & arguments)
	{
		if(! canAccess(callable, instance)) {
			return 0;
		}
		return super::metaCallableRankInvoke(callable, instance, arguments);
	}

	static bool metaCallableCanInvoke(const Variant & callable, const Variant & instance, const ArgumentSpan & arguments)
	{
		if(! canAccess(callable, instance)) {
			return 0;
		}
		return super::metaCallableRankInvoke(callable, instance, arguments);
	}

};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...)>
	: DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...), Class, RT, Args...>
{
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) const>
	: DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) const, Class, RT, Args...>
{
	static constexpr TypeFlags typeFlags = tfConst;
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) volatile>
	: DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) volatile, Class, RT, Args...>
{
	static constexpr TypeFlags typeFlags = tfVolatile;
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) const volatile>
	: DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) const volatile, Class, RT, Args...>
{
	static constexpr TypeFlags typeFlags = tfConst | tfVolatile;
};

#ifdef METAPP_NOEXCEPT_BELONGS_TO_FUNCTION_TYPE

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) noexcept>
	: DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) noexcept, Class, RT, Args...>
{
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) const noexcept>
	: DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) const noexcept, Class, RT, Args...>
{
	static constexpr TypeFlags typeFlags = tfConst;
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) volatile noexcept>
	: DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) volatile noexcept, Class, RT, Args...>
{
	static constexpr TypeFlags typeFlags = tfVolatile;
};

template <typename Class, typename RT, typename ...Args>
struct DeclareMetaTypeBase <RT (Class::*)(Args...) const volatile noexcept>
	: DeclareMetaTypeMemberFunctionBase <RT (Class::*)(Args...) const volatile noexcept, Class, RT, Args...>
{
	static constexpr TypeFlags typeFlags = tfConst | tfVolatile;
};

#endif


} // namespace metapp


#endif

