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
#include "metapp/metarepo.h"
#include "metapp/implement/internal/metarepobase_i.h"
#include "metapp/implement/internal/inheritancerepo_i.h"

namespace metapp {

namespace internal_ {

InheritanceRepo::TypesView InheritanceRepo::getBases(const MetaType * classMetaType) const
{
	return TypesView(doGetClassInfo(classMetaType->getUnifiedType())->baseList);
}

InheritanceRepo::TypesView InheritanceRepo::getDerives(const MetaType * classMetaType) const
{
	return TypesView(doGetClassInfo(classMetaType->getUnifiedType())->derivedList);
}

void * InheritanceRepo::castToBase(void * instance, const MetaType * classMetaType, const size_t baseIndex) const
{
	if(instance == nullptr) {
		return nullptr;
	}
	return doGetClassInfo(classMetaType->getUnifiedType())->baseList[baseIndex].cast(instance);
}

void * InheritanceRepo::castToDerived(void * instance, const MetaType * classMetaType, const size_t derivedIndex) const
{
	if(instance == nullptr) {
		return nullptr;
	}
	return doGetClassInfo(classMetaType->getUnifiedType())->derivedList[derivedIndex].cast(instance);
}

void * InheritanceRepo::cast(void * instance, const MetaType * classMetaType, const MetaType * toMetaType) const
{
	if(instance == nullptr) {
		return nullptr;
	}
	std::array<BaseDerived, maxInheritanceLevels> entryList;
	const int levels = std::abs(doFindRelationship(entryList.data(), classMetaType, toMetaType));
	if(levels > 0) {
		for(int i = 0; i < levels; ++i) {
			instance = entryList[i].cast(instance);
		}
		return instance;
	}
	return nullptr;
}

InheritanceRelationship InheritanceRepo::getRelationship(const MetaType * classMetaType, const MetaType * toMetaType) const
{
	std::array<BaseDerived, maxInheritanceLevels> entryList;
	const int levels = doFindRelationship(entryList.data(), classMetaType, toMetaType);
	if(levels > 0) {
		return InheritanceRelationship::base;
	}
	if(levels < 0) {
		return InheritanceRelationship::derived;
	}
	return InheritanceRelationship::none;
}

bool InheritanceRepo::isClassInHierarchy(const MetaType * classMetaType) const
{
	return doFindClassInfo(classMetaType->getUnifiedType()) != nullptr;
}

const InheritanceRepo::ClassInfo * InheritanceRepo::doFindClassInfo(const void * type) const
{
	auto it = classInfoMap.find(type);
	if(it != classInfoMap.end()) {
		return &it->second;
	}
	return nullptr;
}

const InheritanceRepo::ClassInfo * InheritanceRepo::doGetClassInfo(const void * type) const
{
	const ClassInfo * result = doFindClassInfo(type);
	if(result == nullptr) {
		return doGetDummyClassInfo();
	}
	return result;
}

const InheritanceRepo::ClassInfo * InheritanceRepo::doGetDummyClassInfo()
{
	static ClassInfo classInfo {};
	return &classInfo;
}

int InheritanceRepo::doFindRelationship(
	BaseDerived * entryList,
	const MetaType * fromMetaType,
	const MetaType * toMetaType) const
{
	const void * fromUnifiedType = fromMetaType->getUnifiedType();
	const ClassInfo * currentClassInfo = doFindClassInfo(fromUnifiedType);
	if(currentClassInfo == nullptr) {
		return 0;
	}
	const void * toUnifiedType = toMetaType->getUnifiedType();
	if(doFindClassInfo(toUnifiedType) == nullptr) {
		return 0;
	}
	int levels = doFindBaseClass(entryList, currentClassInfo, toUnifiedType, 0);
	if(levels > 0) {
		return levels;
	}
	levels = doFindDerivedClass(entryList, currentClassInfo, toUnifiedType, 0);
	return -levels;
}

int InheritanceRepo::doFindBaseClass(
	BaseDerived * entryList,
	const ClassInfo * currentClassInfo,
	const void * targetBaseUnifiedType,
	const int level) const
{
	assert(level < maxInheritanceLevels);

	const size_t count = currentClassInfo->baseList.size();
	if(count == 0) {
		return 0;
	}
	for(size_t i = 0; i < count; ++i) {
		entryList[level] = currentClassInfo->baseList[i];
		if(currentClassInfo->baseList[i].targetMetaType->getUnifiedType() == targetBaseUnifiedType) {
			return level + 1;
		}
		const ClassInfo * baseClassInfo = doGetClassInfo(currentClassInfo->baseList[i].targetMetaType->getUnifiedType());
		const int nextLevel = doFindBaseClass(entryList, baseClassInfo, targetBaseUnifiedType, level + 1);
		if(nextLevel > 0) {
			return nextLevel;
		}
	}
	return 0;
}

int InheritanceRepo::doFindDerivedClass(
	BaseDerived * entryList,
	const ClassInfo * currentClassInfo,
	const void * targetDerivedUnifiedType,
	const int level) const
{
	assert(level < maxInheritanceLevels);

	const size_t count = currentClassInfo->derivedList.size();
	if(count == 0) {
		return 0;
	}
	for(size_t i = 0; i < count; ++i) {
		entryList[level] = currentClassInfo->derivedList[i];
		if(currentClassInfo->derivedList[i].targetMetaType->getUnifiedType() == targetDerivedUnifiedType) {
			return level + 1;
		}
		const ClassInfo * derivedClassInfo = doGetClassInfo(currentClassInfo->derivedList[i].targetMetaType->getUnifiedType());
		const int nextLevel = doFindDerivedClass(entryList, derivedClassInfo, targetDerivedUnifiedType, level + 1);
		if(nextLevel > 0) {
			return nextLevel;
		}
	}
	return 0;
}


MetaRepoBase::MetaRepoBase()
	:
		typeList(),
		nameTypeMap(),
		kindTypeMap(),
		methodList(),
		methodMap(),
		fieldList(),
		fieldMap()
{
}

RegisteredField & MetaRepoBase::registerField(const std::string & name, const Variant & field)
{
	if(field.getMetaType()->getMetaAccessible() == nullptr) {
		errorWrongMetaType();
	}

	auto it = fieldMap.find(name);
	if(it != fieldMap.end()) {
		return *it->second;
	}
	fieldList.emplace_back(name, field);
	RegisteredField & registeredField = fieldList.back();
	fieldMap.insert(typename decltype(fieldMap)::value_type(registeredField.getName(), &registeredField));
	return registeredField;
}

RegisteredMethod & MetaRepoBase::registerMethod(const std::string & name, const Variant & method)
{
	if(method.getMetaType()->getMetaCallable() == nullptr) {
		errorWrongMetaType();
	}

	methodList.emplace_back(name, method);
	RegisteredMethod & registeredMethod = methodList.back();
	auto it = methodMap.find(name);
	if(it == methodMap.end()) {
		methodMap.insert(typename decltype(methodMap)::value_type(
			registeredMethod.getName(), RegisteredMethodPointerList {&registeredMethod}
		));
	}
	else {
		it->second.push_back(&registeredMethod);
	}
	return registeredMethod;
}

RegisteredType & MetaRepoBase::registerType(std::string name, const MetaType * metaType)
{
	if(name.empty()) {
		name = getNameByTypeKind(metaType->getTypeKind());
	}

	typeList.emplace_back(name, metaType);
	RegisteredType & registeredType = typeList.back();
	if(! registeredType.getName().empty()) {
		nameTypeMap[registeredType.getName()] = &registeredType;
	}
	kindTypeMap[metaType->getTypeKind()] = &registeredType;

	return registeredType;
}

void MetaRepoBase::doGetFieldList(RegisteredFieldList * result) const
{
	result->insert(result->end(), fieldList.begin(), fieldList.end());
}

const RegisteredFieldList & MetaRepoBase::doGetFieldList() const
{
	return fieldList;
}

const RegisteredField & MetaRepoBase::doGetField(const std::string & name) const
{
	auto it = fieldMap.find(name);
	if(it != fieldMap.end()) {
		return *it->second;
	}
	return RegisteredField::getEmpty();
}

const RegisteredMethod & MetaRepoBase::doGetMethod(const std::string & name) const
{
	auto it = methodMap.find(name);
	if(it != methodMap.end()) {
		return *it->second.at(0);
	}
	return RegisteredMethod::getEmpty();
}

void MetaRepoBase::doGetMethodList(const std::string & name, RegisteredMethodList * result) const
{
	auto it = methodMap.find(name);
	if(it != methodMap.end()) {
		for(auto i = it->second.begin(); i != it->second.end(); ++i) {
			result->push_back(*(*i));
		}
	}
}

void MetaRepoBase::doGetMethodList(RegisteredMethodList * result) const
{
	result->insert(result->end(), methodList.begin(), methodList.end());
}

const RegisteredMethodList & MetaRepoBase::doGetMethodList() const
{
	return methodList;
}

const RegisteredType & MetaRepoBase::doGetType(const std::string & name) const
{
	auto it = nameTypeMap.find(name);
	if(it != nameTypeMap.end()) {
		return *it->second;
	}
	return RegisteredType::getEmpty();
}

const RegisteredType & MetaRepoBase::doGetType(const TypeKind kind) const
{
	auto it = kindTypeMap.find(kind);
	if(it != kindTypeMap.end()) {
		return *it->second;
	}
	return RegisteredType::getEmpty();
}

void MetaRepoBase::doGetTypeList(RegisteredTypeList * result) const
{
	result->insert(result->end(), typeList.begin(), typeList.end());
}

const RegisteredTypeList & MetaRepoBase::doGetTypeList() const
{
	return typeList;
}

} // namespace internal_


MetaRepo * getMetaRepo()
{
	static MetaRepo repo;
	return &repo;
}

MetaRepo::MetaRepo()
	:
		internal_::MetaRepoBase(),
		internal_::InheritanceRepo(),
		repoList(),
		repoMap()
{
	registerBuiltinTypes();
}

const RegisteredField & MetaRepo::getField(const std::string & name) const
{
	return doGetField(name);
}

const RegisteredFieldList & MetaRepo::getFieldList() const
{
	return doGetFieldList();
}

const RegisteredMethod & MetaRepo::getMethod(const std::string & name) const
{
	return doGetMethod(name);
}

RegisteredMethodList MetaRepo::getMethodList(const std::string & name) const
{
	RegisteredMethodList result;
	doGetMethodList(name, &result);
	return result;
}

const RegisteredMethodList & MetaRepo::getMethodList() const
{
	return doGetMethodList();
}

const RegisteredType & MetaRepo::getType(const std::string & name) const
{
	return doGetType(name);
}

const RegisteredType & MetaRepo::getType(const TypeKind kind) const
{
	return doGetType(kind);
}

const RegisteredTypeList & MetaRepo::getTypeList() const
{
	return doGetTypeList();
}

RegisteredRepo & MetaRepo::registerRepo(const std::string & name, MetaRepo * repo)
{
	if(repo == nullptr) {
		repo = new MetaRepo();
	}
	repoList.emplace_back(name, repo);
	RegisteredRepo & registeredRepo = repoList.back();
	repoMap[registeredRepo.getName()] = &registeredRepo;

	return registeredRepo;
}

const RegisteredRepo & MetaRepo::getRepo(const std::string & name) const
{
	auto it = repoMap.find(name);
	if(it != repoMap.end()) {
		return *it->second;
	}
	return RegisteredRepo::getEmpty();
}

const RegisteredRepoList & MetaRepo::getRepoList() const
{
	return repoList;
}

void MetaRepo::registerBuiltinTypes()
{
	registerType<void>();
	registerType<bool>();
	registerType<char>();
	registerType<wchar_t>();
	registerType<signed char>();
	registerType<unsigned char>();
	registerType<short>();
	registerType<unsigned short>();
	registerType<int>();
	registerType<unsigned int>();
	registerType<long>();
	registerType<unsigned long>();
	registerType<long long>();
	registerType<unsigned long long>();
	registerType<float>();
	registerType<double>();
	registerType<long double>();
	registerType<std::string>();
	registerType<std::wstring>();
}


} // namespace metapp
