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
#include "metapp/inheritancerepo.h"

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

	void addConstructor(const Variant & constructor) {
		if(constructor.getMetaType()->getMetaCallable() == nullptr) {
			errorWrongMetaType();
			return;
		}
		if(! constructorList) {
			constructorList = std::make_shared<MethodList>();
		}
		constructorList->addMethod(constructor);
	}
	
	const MethodList * getConstructorList() const {
		return constructorList.get();
	}

	FieldInfo getField(const std::string & name, const Flags flags = flagIncludeBase) const {
		if(hasFlag(flags, flagIncludeBase)) {
			FieldInfo result;
			getInheritanceRepo()->traverse(classMetaType, [&result, &name](const MetaType * metaType) -> bool {
				const MetaClass * metaClass = metaType->getMetaClass();
				if(metaClass != nullptr) {
					result = metaClass->doGetField(name);
					if(result.isValid()) {
						return false;
					}
				}
				return true;
				});
			return result;
		}
		else {
			return doGetField(name);
		}
	}

	std::vector<FieldInfo> getFieldList(const Flags flags = flagIncludeBase) const {
		std::vector<FieldInfo> result;
		if(hasFlag(flags, flagIncludeBase)) {
			getInheritanceRepo()->traverse(classMetaType, [&result](const MetaType * metaType) -> bool {
				const MetaClass * metaClass = metaType->getMetaClass();
				if(metaClass != nullptr) {
					metaClass->doGetFieldList(result);
				}
				return true;
			});
		}
		else {
			doGetFieldList(result);
		}
		return result;
	}

	MethodInfo getMethod(const std::string & name, const Flags flags = flagIncludeBase) const {
		if(hasFlag(flags, flagIncludeBase)) {
			MethodInfo result;
			getInheritanceRepo()->traverse(classMetaType, [&result, &name](const MetaType * metaType) -> bool {
				const MetaClass * metaClass = metaType->getMetaClass();
				if(metaClass != nullptr) {
					result = metaClass->doGetMethod(name);
					if(result.isValid()) {
						return false;
					}
				}
				return true;
			});
			return result;
		}
		else {
			return doGetMethod(name);
		}
	}

	std::vector<MethodInfo> getMethodList(const std::string & name, const Flags flags = flagIncludeBase) const {
		std::vector<MethodInfo> result;
		if(hasFlag(flags, flagIncludeBase)) {
			getInheritanceRepo()->traverse(classMetaType, [&result, &name](const MetaType * metaType) -> bool {
				const MetaClass * metaClass = metaType->getMetaClass();
				if(metaClass != nullptr) {
					metaClass->doGetMethodList(name, result);
				}
				return true;
			});
		}
		else {
			doGetMethodList(name, result);
		}
		return result;
	}

	std::vector<MethodInfo> getMethodList(const Flags flags = flagIncludeBase) const {
		std::vector<MethodInfo> result;
		if(hasFlag(flags, flagIncludeBase)) {
			getInheritanceRepo()->traverse(classMetaType, [&result](const MetaType * metaType) -> bool {
				const MetaClass * metaClass = metaType->getMetaClass();
				if(metaClass != nullptr) {
					metaClass->doGetMethodList(result);
				}
				return true;
			});
		}
		else {
			doGetMethodList(result);
		}
		return result;
	}

private:
	bool hasFlag(const Flags flags, const Flags flag) const {
		return (flags & flag) != 0;
	}

private:
	const MetaType * classMetaType;
	std::shared_ptr<MethodList> constructorList;
};


} // namespace metapp

#endif
