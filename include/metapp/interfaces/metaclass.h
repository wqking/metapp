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

#ifndef METAPP_METACLASS_H_969872685611
#define METAPP_METACLASS_H_969872685611

#include "metapp/implement/internal/metaclass_i.h"
#include "metapp/implement/internal/metarepobase_i.h"
#include "metapp/metarepo.h"
#include "metapp/registration/registeredconstructor.h"

#include <vector>
#include <memory>

namespace metapp {

class MetaClass : public internal_::MetaRepoBase
{
private:
	using super = internal_::MetaRepoBase;

public:
	using Flags = int;
	static constexpr Flags flagIncludeBase = (1 << 0);

public:
	template <typename FT>
	MetaClass(const MetaType * classMetaType, FT callback)
		:
			super(),
			classMetaType(classMetaType),
			constructorList()
	{
		callback(*this);
	}

	RegisteredConstructor & registerConstructor(const Variant & constructor);
	
	const RegisteredConstructorList & getConstructorList() const;

	const RegisteredField & getField(const std::string & name, const Flags flags = flagIncludeBase) const;
	RegisteredFieldList getFieldList(const Flags flags = flagIncludeBase) const;

	const RegisteredMethod & getMethod(const std::string & name, const Flags flags = flagIncludeBase) const;
	RegisteredMethodList getMethodList(const std::string & name, const Flags flags = flagIncludeBase) const;
	RegisteredMethodList getMethodList(const Flags flags = flagIncludeBase) const;

private:
	bool hasFlag(const Flags flags, const Flags flag) const {
		return (flags & flag) != 0;
	}

private:
	const MetaType * classMetaType;
	RegisteredConstructorList constructorList;
};


} // namespace metapp

#endif
