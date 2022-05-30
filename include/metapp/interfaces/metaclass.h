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

#include "metapp/metaitem.h"
#include "metapp/implement/internal/metarepobase_i.h"

#include <memory>

namespace metapp {

class MetaClass : public internal_::MetaRepoBase
{
public:
	using Flags = int;
	static constexpr Flags flagNone = 0;
	static constexpr Flags flagIncludeBase = (1 << 0);

public:
	template <typename FT>
	MetaClass(const MetaType * classMetaType, FT callback)
		:
			internal_::MetaRepoBase(),
			classMetaType(classMetaType)
	{
		callback(*this);
	}

	MetaItem & registerConstructor(const Variant & constructor);
	const MetaItem & getConstructor() const;

	const MetaItem & getAccessible(const std::string & name, const Flags flags = flagIncludeBase) const;
	MetaItemView getAccessibleView(const Flags flags = flagIncludeBase) const;

	const MetaItem & getCallable(const std::string & name, const Flags flags = flagIncludeBase) const;
	MetaItemView getCallableView(const Flags flags = flagIncludeBase) const;

	const MetaItem & getVariable(const std::string & name, const Flags flags = flagIncludeBase) const;
	MetaItemView getVariableView(const Flags flags = flagIncludeBase) const;

	const MetaItem & getType(const std::string & name, const Flags flags = flagIncludeBase) const;
	const MetaItem & getType(const TypeKind kind, const Flags flags = flagIncludeBase) const;
	const MetaItem & getType(const MetaType * metaType, const Flags flags = flagIncludeBase) const;
	MetaItemView getTypeView(const Flags flags = flagIncludeBase) const;

	const MetaItem & getItem(const std::string & name) const;

private:
	bool hasFlag(const Flags flags, const Flags flag) const {
		return (flags & flag) != 0;
	}

	MetaItemView doBuildMetaItemView(
		const MetaItemList & (MetaClass::*listGetter)() const,
		const Flags flags
	) const;

	const MetaItem & doFindItemByName(
		const MetaItem & (MetaClass::*itemGetter)(const std::string &) const,
		const std::string & name,
		const Flags flags
	) const;

private:
	const MetaType * classMetaType;
	MetaItem constructorItem;
};


} // namespace metapp

#endif
