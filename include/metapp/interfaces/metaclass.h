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

	RegisteredConstructor & addConstructor(const Variant & constructor) {
		if(constructor.getMetaType()->getMetaCallable() == nullptr) {
			errorWrongMetaType();
		}
		constructorList.push_back(RegisteredConstructor(constructor));
		return constructorList.back();
	}
	
	const RegisteredConstructorList & getConstructorList() const {
		return constructorList;
	}

	const RegisteredField & getField(const std::string & name, const Flags flags = flagIncludeBase) const {
		if(hasFlag(flags, flagIncludeBase)) {
			const RegisteredField * result = &RegisteredField::getEmpty();
			getInheritanceRepo()->traverse(classMetaType, [&result, &name](const MetaType * metaType) -> bool {
				const MetaClass * metaClass = metaType->getMetaClass();
				if(metaClass != nullptr) {
					result = &metaClass->doGetField(name);
					if(! result->isEmpty()) {
						return false;
					}
				}
				return true;
				});
			return *result;
		}
		else {
			return doGetField(name);
		}
	}

	RegisteredFieldList getFieldList(const Flags flags = flagIncludeBase) const {
		RegisteredFieldList result;
		if(hasFlag(flags, flagIncludeBase)) {
			getInheritanceRepo()->traverse(classMetaType, [&result](const MetaType * metaType) -> bool {
				const MetaClass * metaClass = metaType->getMetaClass();
				if(metaClass != nullptr) {
					metaClass->doGetFieldList(&result);
				}
				return true;
			});
		}
		else {
			doGetFieldList(&result);
		}
		return result;
	}

	const RegisteredMethod & getMethod(const std::string & name, const Flags flags = flagIncludeBase) const {
		if(hasFlag(flags, flagIncludeBase)) {
			const RegisteredMethod * result = &RegisteredMethod::getEmpty();
			getInheritanceRepo()->traverse(classMetaType, [&result, &name](const MetaType * metaType) -> bool {
				const MetaClass * metaClass = metaType->getMetaClass();
				if(metaClass != nullptr) {
					result = &metaClass->doGetMethod(name);
					if(! result->isEmpty()) {
						return false;
					}
				}
				return true;
			});
			return *result;
		}
		else {
			return doGetMethod(name);
		}
	}

	RegisteredMethodList getMethodList(const std::string & name, const Flags flags = flagIncludeBase) const {
		RegisteredMethodList result;
		if(hasFlag(flags, flagIncludeBase)) {
			getInheritanceRepo()->traverse(classMetaType, [&result, &name](const MetaType * metaType) -> bool {
				const MetaClass * metaClass = metaType->getMetaClass();
				if(metaClass != nullptr) {
					metaClass->doGetMethodList(name, &result);
				}
				return true;
			});
		}
		else {
			doGetMethodList(name, &result);
		}
		return result;
	}

	RegisteredMethodList getMethodList(const Flags flags = flagIncludeBase) const {
		RegisteredMethodList result;
		if(hasFlag(flags, flagIncludeBase)) {
			getInheritanceRepo()->traverse(classMetaType, [&result](const MetaType * metaType) -> bool {
				const MetaClass * metaClass = metaType->getMetaClass();
				if(metaClass != nullptr) {
					metaClass->doGetMethodList(&result);
				}
				return true;
			});
		}
		else {
			doGetMethodList(&result);
		}
		return result;
	}

private:
	bool hasFlag(const Flags flags, const Flags flag) const {
		return (flags & flag) != 0;
	}

private:
	const MetaType * classMetaType;
	std::vector<RegisteredConstructor> constructorList;
};


} // namespace metapp

#endif
