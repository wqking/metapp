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

#ifndef METAPP_VARIANT_METATYPE_H_969872685611
#define METAPP_VARIANT_METATYPE_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <>
struct DeclareMetaTypeBase <Variant>
{
	using Common = CommonDeclareMetaType <Variant>;
	static constexpr TypeKind typeKind = tkVariant;

	static bool cast(Variant * result, const Variant & value, const MetaType * toMetaType) {
		if(Common::cast(result, value, toMetaType)) {
			return true;
		}
		else {
			const Variant & ref = value.get<const Variant &>();
			return ref.getMetaType()->cast(result, ref, toMetaType);
		}
	}

	static bool castFrom(Variant * result, const Variant & value, const MetaType * /*fromMetaType*/)
	{
		if(result != nullptr) {
			*result = value;
		}
		return true;
	}

};


} // namespace metapp


#endif

