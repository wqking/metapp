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

#include "metapp/metatype.h"
#include "metapp/implement/internal/metarepobase_i.h"

namespace metapp {

namespace internal_ {

MetaRepoBase::MetaRepoBase()
	:
		nameTypeMap(),
		kindTypeMap(),
		methodMap(),
		fieldMap()
{
}

void MetaRepoBase::registerType(const MetaType * metaType, std::string name)
{
	if(name.empty()) {
		name = getNameByTypeKind(metaType->getTypeKind());
	}
	if(! name.empty()) {
		nameTypeMap[name] = metaType;
	}
	kindTypeMap[metaType->getTypeKind()] = std::make_pair(name, metaType);
}

const MetaType * MetaRepoBase::getTypeByName(const std::string & name) const
{
	auto it = nameTypeMap.find(name);
	if(it != nameTypeMap.end()) {
		return it->second;
	}
	return nullptr;
}

const MetaType * MetaRepoBase::getTypeByKind(const TypeKind kind) const
{
	auto it = kindTypeMap.find(kind);
	if(it != kindTypeMap.end()) {
		return it->second.second;
	}
	return nullptr;
}

std::string MetaRepoBase::getNameByKind(const TypeKind kind) const
{
	auto it = kindTypeMap.find(kind);
	if(it != kindTypeMap.end()) {
		return it->second.first;
	}
	return std::string();
}

std::vector<std::string> MetaRepoBase::getTypeNameList() const
{
	return internal_::getMapKeys(nameTypeMap);
}

RegisteredMethod & MetaRepoBase::registerMethod(const std::string & name, const Variant & method)
{
	if(method.getMetaType()->getMetaCallable() == nullptr) {
		errorWrongMetaType();
	}

	RegisteredMethod registeredMethod{ name, method };
	auto it = methodMap.find(name);
	if(it == methodMap.end()) {
		auto i = methodMap.insert(typename decltype(methodMap)::value_type(registeredMethod.getName(), NamedMethodList()));
		i.first->second.push_back(registeredMethod);
		return i.first->second.back();
	}
	else {
		it->second.push_back(registeredMethod);
		return it->second.back();
	}
}

RegisteredField & MetaRepoBase::registerField(const std::string & name, const Variant & field)
{
	if(field.getMetaType()->getMetaAccessible() == nullptr) {
		errorWrongMetaType();
	}

	auto it = fieldMap.find(name);
	if(it != fieldMap.end()) {
		return it->second;
	}
	RegisteredField registeredField(name, field);
	auto result = fieldMap.insert(typename decltype(fieldMap)::value_type(registeredField.getName(), registeredField));
	return result.first->second;
}

void MetaRepoBase::doGetFieldList(RegisteredFieldList * result) const
{
	for(auto it = fieldMap.begin(); it != fieldMap.end(); ++it) {
		result->push_back(it->second);
	}
}

const RegisteredField & MetaRepoBase::doGetField(const std::string & name) const
{
	auto it = fieldMap.find(name);
	if(it != fieldMap.end()) {
		return it->second;
	}
	return RegisteredField::getEmpty();
}

const RegisteredMethod & MetaRepoBase::doGetMethod(const std::string & name) const
{
	auto it = methodMap.find(name);
	if(it != methodMap.end()) {
		return it->second.at(0);
	}
	return RegisteredMethod::getEmpty();
}

void MetaRepoBase::doGetMethodList(const std::string & name, RegisteredMethodList * result) const
{
	auto it = methodMap.find(name);
	if(it != methodMap.end()) {
		for(auto i = it->second.begin(); i != it->second.end(); ++i) {
			result->push_back(*i);
		}
	}
}

void MetaRepoBase::doGetMethodList(RegisteredMethodList * result) const
{
	for(auto it = methodMap.begin(); it != methodMap.end(); ++it) {
		for(auto i = it->second.begin(); i != it->second.end(); ++i) {
			result->push_back(*i);
		}
	}
}


} // namespace internal_

} // namespace metapp
