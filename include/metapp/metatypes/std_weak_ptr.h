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

#ifndef METAPP_STD_WEAK_PTR_H_969872685611
#define METAPP_STD_WEAK_PTR_H_969872685611

#include "metapp/metatype.h"

#include <memory>

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <std::weak_ptr<T> >
{
	using WeakPtr = std::weak_ptr<T>;

	using UpType = T;
	static constexpr TypeKind typeKind = tkStdWeakPtr;

	static bool cast(Variant * result, const Variant * fromVar, const MetaType * toMetaType) {
		const MetaType * nonRef = getNonReferenceMetaType(toMetaType);
		if(nonRef->getTypeKind() == tkStdSharedPtr && getMetaType<WeakPtr>()->getUpType()->equal(nonRef->getUpType())) {
			if(result != nullptr) {
				*result = std::shared_ptr<T>(fromVar->get<WeakPtr &>());
			}
			return true;
		}

		return commonCast(result, fromVar, getMetaType<WeakPtr>(), toMetaType);
	}

};


} // namespace metapp


#endif

