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

#ifndef METAPP_UTILITY_H_969872685611
#define METAPP_UTILITY_H_969872685611

#include "metapp/variant.h"
#include "metapp/metatype.h"
#include "metapp/utilities/typelist.h"
#include "metapp/exception.h"
#include "metapp/implement/internal/util_i.h"

#include <type_traits>
#include <string>

namespace metapp {

inline const MetaType * getNonReferenceMetaType(const MetaType * metaType)
{
	if(metaType->isReference()) {
		metaType = metaType->getUpType();
	}
	return metaType;
}

inline const MetaType * getNonReferenceMetaType(const Variant & var)
{
	return getNonReferenceMetaType(var.getMetaType());
}

inline constexpr bool typeKindIsArithmetic(const TypeKind typeKind)
{
	return typeKind >= tkArithmeticBegin && typeKind <= tkArithmeticEnd;
}

inline constexpr bool typeKindIsIntegral(const TypeKind typeKind)
{
	return typeKind >= tkIntegerBegin && typeKind <= tkIntegerEnd;
}

inline constexpr bool typeKindIsSignedIntegral(const TypeKind typeKind)
{
	return typeKind == tkInt
		|| typeKind == tkLong
		|| typeKind == tkLongLong
		|| (typeKind >= tkChar && typeKind <= tkSignedChar)
		|| typeKind == tkShort
	;
}

inline constexpr bool typeKindIsUnsignedIntegral(const TypeKind typeKind)
{
	return typeKindIsIntegral(typeKind)
		&& ! typeKindIsSignedIntegral(typeKind)
	;
}

inline constexpr bool typeKindIsReal(const TypeKind typeKind)
{
	return typeKind >= tkRealBegin && typeKind <= tkRealEnd;
}

template <typename ...Ts>
constexpr bool isSameMetaType(const MetaType * metaType)
{
	return internal_::SameMetaTypeChecker<TypeList<Ts...> >::isSame(metaType);
}

template <typename Signature, typename Class>
auto selectOverload(Signature (Class::*func)) -> decltype(func)
{
	return func;
}

template <typename Signature>
auto selectOverload(Signature * func) -> decltype(func)
{
	return func;
}

void * getPointer(const Variant & var);
const MetaType * getPointedType(const Variant & var);
std::pair<void *, const MetaType *> getPointerAndType(const Variant & var);

Variant depointer(const Variant & var);
Variant dereference(const Variant & var);

inline bool isMutable(const Variant & var)
{
	return ! getPointedType(var)->isConst();
}

inline void requireMutable(const Variant & var)
{
	if(! isMutable(var)) {
		raiseException<UnwritableException>();
	}
}

std::string getNameByTypeKind(const TypeKind typeKind);

class MetaRepo;
void dumpMetaType(std::ostream & stream, const MetaType * metaType, const MetaRepo * metaRepo = nullptr);


} // namespace metapp

#endif
