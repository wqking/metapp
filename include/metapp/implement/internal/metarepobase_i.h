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

#ifndef METAPP_METAREPOBASE_I_H_969872685611
#define METAPP_METAREPOBASE_I_H_969872685611

#include "metapp/metatype.h"
#include "metapp/exception.h"
#include "metapp/implement/internal/util_i.h"
#include "metapp/registration/registeredfield.h"
#include "metapp/registration/registeredmethod.h"

#include <map>
#include <deque>
#include <memory>
#include <functional>

namespace metapp {

namespace internal_ {

class MetaRepoBase
{
private:
	using XXXMethodList = std::vector<Variant>;
	struct NamedMethodList
	{
		std::string name;
		std::vector<RegisteredMethod> methods;
	};

public:
	using VariantList = std::vector<std::reference_wrapper<const Variant> >;
	using NameList = std::vector<std::reference_wrapper<const std::string> >;

public:
	MetaRepoBase()
		:
			nameTypeMap(),
			kindTypeMap(),
			methodMap(),
			namedMethodListList(),
			fieldMap(),
			fieldList()
	{
	}

	void addType(const MetaType * metaType, std::string name = "") {
		if(name.empty()) {
			name = getNameByTypeKind(metaType->getTypeKind());
		}
		if(! name.empty()) {
			nameTypeMap[name] = metaType;
		}
		kindTypeMap[metaType->getTypeKind()] = std::make_pair(name, metaType);
	}

	template <typename T>
	void addType(const std::string & name = "") {
		addType(getMetaType<T>(), name);
	}

	const MetaType * getTypeByName(const std::string & name) const {
		auto it = nameTypeMap.find(name);
		if(it != nameTypeMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	const MetaType * getTypeByKind(const TypeKind kind) const {
		auto it = kindTypeMap.find(kind);
		if(it != kindTypeMap.end()) {
			return it->second.second;
		}
		return nullptr;
	}

	std::string getNameByKind(const TypeKind kind) const {
		auto it = kindTypeMap.find(kind);
		if(it != kindTypeMap.end()) {
			return it->second.first;
		}
		return std::string();
	}

	std::vector<std::string> getTypeNameList() const {
		return internal_::getMapKeys(nameTypeMap);
	}

	void addMethod(const std::string & name, const Variant & method) {
		if(method.getMetaType()->getMetaCallable() == nullptr) {
			errorWrongMetaType();
			return;
		}

		auto it = methodMap.find(name);
		NamedMethodList * methodList = nullptr;
		if(it == methodMap.end()) {
			namedMethodListList.push_back({ name, std::vector<RegisteredMethod>() });
			methodList = &namedMethodListList.back();
			methodMap.insert(typename decltype(methodMap)::value_type(methodList->name, *methodList));
		}
		else {
			methodList = &it->second.get();
		}
		methodList->methods.push_back(RegisteredMethod(name, method));
	}

	RegisteredField & addField(const std::string & name, const Variant & field) {
		if(field.getMetaType()->getMetaAccessible() == nullptr) {
			errorWrongMetaType();
		}

		auto it = fieldMap.find(name);
		if(it != fieldMap.end()) {
			return it->second;
		}
		fieldList.push_back(RegisteredField(name, field));
		RegisteredField & registeredField = fieldList.back();
		fieldMap.insert(typename decltype(fieldMap)::value_type(registeredField.getName(), registeredField));
		return registeredField;
	}

protected:
	void doGetFieldList(RegisteredFieldList * result) const {
		for(auto it = fieldList.begin(); it != fieldList.end(); ++it) {
			result->push_back(std::ref(*it));
		}
	}

	const RegisteredField & doGetField(const std::string & name) const {
		auto it = fieldMap.find(name);
		if(it != fieldMap.end()) {
			return it->second;
		}
		return RegisteredField::getEmpty();
	}

	const RegisteredMethod & doGetMethod(const std::string & name) const {
		auto it = methodMap.find(name);
		if(it != methodMap.end()) {
			return it->second.get().methods[0];
		}
		return RegisteredMethod::getEmpty();
	}

	void doGetMethodList(const std::string & name, RegisteredMethodList * result) const {
		auto it = methodMap.find(name);
		if(it != methodMap.end()) {
			for(auto i = it->second.get().methods.begin(); i != it->second.get().methods.end(); ++i) {
				result->push_back(std::ref(*i));
			}
		}
	}

	void doGetMethodList(RegisteredMethodList * result) const {
		for(auto it = namedMethodListList.begin(); it != namedMethodListList.end(); ++it) {
			for(auto i = it->methods.begin(); i != it->methods.end(); ++i) {
				result->push_back(std::ref(*i));
			}
		}
	}

private:
	std::map<std::string, const MetaType *> nameTypeMap;
	std::map<TypeKind, std::pair<std::string, const MetaType *> > kindTypeMap;

	std::map<
		std::reference_wrapper<const std::string>,
		std::reference_wrapper<NamedMethodList>,
		std::less<const std::string>
	> methodMap;
	std::deque<NamedMethodList> namedMethodListList;

	std::map<
		std::reference_wrapper<const std::string>,
		std::reference_wrapper<RegisteredField>,
		std::less<const std::string>
	> fieldMap;
	std::deque<RegisteredField> fieldList;
};


} // namespace internal_

} // namespace metapp

#endif
