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
#include "metapp/interfaces/metacallable.h"
#include "metapp/metatype.h"
#include "metapp/utilities/typelist.h"
#include "metapp/exception.h"

#include <type_traits>
#include <string>

namespace metapp {

std::string getNameByTypeKind(const TypeKind typeKind);

inline const MetaType * getNonReferenceMetaType(const MetaType * metaType)
{
	if(metaType->isReference()) {
		metaType = metaType->getUpType();
	}
	return metaType;
}

inline const MetaType * getNonReferenceMetaType(const Variant & value)
{
	return getNonReferenceMetaType(value.getMetaType());
}

inline const MetaType * getReferredMetaType(const MetaType * metaType)
{
	if(metaType->isPointer() || metaType->isReference()) {
		metaType = metaType->getUpType();
	}
	return metaType;
}

inline const MetaType * getReferredMetaType(const Variant & value)
{
	return getReferredMetaType(value.getMetaType());
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

class MetaRepo;

void dumpMetaType(std::ostream & stream, const metapp::MetaType * metaType, const MetaRepo * metaRepository = nullptr);


} // namespace metapp

#endif
