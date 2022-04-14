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

namespace {

RegisteredAccessibleList dummyRegisteredAccessibleList;
RegisteredCallableList dummyRegisteredCallableList;
RegisteredTypeList dummyRegisteredTypeList;

} // namespace

RegisteredAccessible emptyRegisteredAccessible;
RegisteredCallable emptyRegisteredCallable;
RegisteredType emptyRegisteredType;
RegisteredRepo emptyRegisteredRepo;

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
		typeData(),
		callableData(),
		accessibleData()
{
}

RegisteredAccessible & MetaRepoBase::registerAccessible(const std::string & name, const Variant & accessible)
{
	if(accessible.getMetaType()->getMetaAccessible() == nullptr) {
		errorWrongMetaType();
	}

	if(! accessibleData) {
		accessibleData = std::make_shared<AccessibleData>();
	}

	auto it = accessibleData->accessibleMap.find(name);
	if(it != accessibleData->accessibleMap.end()) {
		return *it->second;
	}
	accessibleData->accessibleList.emplace_back(name, accessible);
	RegisteredAccessible & registeredAccessible = accessibleData->accessibleList.back();
	accessibleData->accessibleMap.insert(typename decltype(accessibleData->accessibleMap)::value_type(registeredAccessible.getName(), &registeredAccessible));
	return registeredAccessible;
}

RegisteredCallable & MetaRepoBase::registerCallable(const std::string & name, const Variant & callable)
{
	if(callable.getMetaType()->getMetaCallable() == nullptr) {
		errorWrongMetaType();
	}

	if(! callableData) {
		callableData = std::make_shared<CallableData>();
	}

	callableData->callableList.emplace_back(name, callable);
	RegisteredCallable & registeredCallable = callableData->callableList.back();
	auto it = callableData->callableMap.find(name);
	if(it == callableData->callableMap.end()) {
		callableData->callableMap.insert(typename decltype(callableData->callableMap)::value_type(
			registeredCallable.getName(), CallableData::RegisteredCallablePointerList {&registeredCallable}
		));
	}
	else {
		it->second.push_back(&registeredCallable);
	}
	return registeredCallable;
}

RegisteredType & MetaRepoBase::registerType(std::string name, const MetaType * metaType)
{
	if(name.empty()) {
		name = getNameByTypeKind(metaType->getTypeKind());
	}

	if(! typeData) {
		typeData = std::make_shared<TypeData>();
	}

	auto it = typeData->typeTypeMap.find(metaType);
	if(it != typeData->typeTypeMap.end()) {
		return *it->second;
	}
	typeData->typeList.emplace_back(name, metaType);
	RegisteredType & registeredType = typeData->typeList.back();
	if(! registeredType.getName().empty()) {
		typeData->nameTypeMap[registeredType.getName()] = &registeredType;
	}
	typeData->kindTypeMap[metaType->getTypeKind()] = &registeredType;
	typeData->typeTypeMap[metaType] = &registeredType;

	return registeredType;
}

void MetaRepoBase::doGetAccessibleList(RegisteredAccessibleList * result) const
{
	if(accessibleData) {
		result->insert(result->end(), accessibleData->accessibleList.begin(), accessibleData->accessibleList.end());
	}
}

const RegisteredAccessibleList & MetaRepoBase::doGetAccessibleList() const
{
	if(accessibleData) {
		return accessibleData->accessibleList;
	}
	else {
		return dummyRegisteredAccessibleList;
	}
}

const RegisteredAccessible & MetaRepoBase::doGetAccessible(const std::string & name) const
{
	if(accessibleData) {
		auto it = accessibleData->accessibleMap.find(name);
		if(it != accessibleData->accessibleMap.end()) {
			return *it->second;
		}
	}
	return internal_::emptyRegisteredAccessible;
}

const RegisteredCallable & MetaRepoBase::doGetCallable(const std::string & name) const
{
	if(callableData) {
		auto it = callableData->callableMap.find(name);
		if(it != callableData->callableMap.end()) {
			return *it->second.at(0);
		}
	}
	return internal_::emptyRegisteredCallable;
}

void MetaRepoBase::doGetCallableList(const std::string & name, RegisteredCallableList * result) const
{
	if(callableData) {
		auto it = callableData->callableMap.find(name);
		if(it != callableData->callableMap.end()) {
			for(auto i = it->second.begin(); i != it->second.end(); ++i) {
				result->push_back(*(*i));
			}
		}
	}
}

void MetaRepoBase::doGetCallableList(RegisteredCallableList * result) const
{
	if(callableData) {
		result->insert(result->end(), callableData->callableList.begin(), callableData->callableList.end());
	}
}

const RegisteredCallableList & MetaRepoBase::doGetCallableList() const
{
	if(callableData) {
		return callableData->callableList;
	}
	else {
		return dummyRegisteredCallableList;
	}
}

const RegisteredType & MetaRepoBase::doGetType(const std::string & name) const
{
	if(typeData) {
		auto it = typeData->nameTypeMap.find(name);
		if(it != typeData->nameTypeMap.end()) {
			return *it->second;
		}
	}
	return internal_::emptyRegisteredType;
}

const RegisteredType & MetaRepoBase::doGetType(const TypeKind kind) const
{
	if(typeData) {
		auto it = typeData->kindTypeMap.find(kind);
		if(it != typeData->kindTypeMap.end()) {
			return *it->second;
		}
	}
	return internal_::emptyRegisteredType;
}

const RegisteredType & MetaRepoBase::doGetType(const MetaType * metaType) const
{
	if(typeData) {
		auto it = typeData->typeTypeMap.find(metaType);
		if(it != typeData->typeTypeMap.end()) {
			return *it->second;
		}
	}
	return internal_::emptyRegisteredType;
}

void MetaRepoBase::doGetTypeList(RegisteredTypeList * result) const
{
	if(typeData) {
		result->insert(result->end(), typeData->typeList.begin(), typeData->typeList.end());
	}
}

const RegisteredTypeList & MetaRepoBase::doGetTypeList() const
{
	if(typeData) {
		return typeData->typeList;
	}
	else {
		return dummyRegisteredTypeList;
	}
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

const RegisteredAccessible & MetaRepo::getAccessible(const std::string & name) const
{
	return doGetAccessible(name);
}

const RegisteredAccessibleList & MetaRepo::getAccessibleList() const
{
	return doGetAccessibleList();
}

const RegisteredCallable & MetaRepo::getCallable(const std::string & name) const
{
	return doGetCallable(name);
}

RegisteredCallableList MetaRepo::getCallableList(const std::string & name) const
{
	RegisteredCallableList result;
	doGetCallableList(name, &result);
	return result;
}

const RegisteredCallableList & MetaRepo::getCallableList() const
{
	return doGetCallableList();
}

const RegisteredType & MetaRepo::getType(const std::string & name) const
{
	return doGetType(name);
}

const RegisteredType & MetaRepo::getType(const TypeKind kind) const
{
	return doGetType(kind);
}

const RegisteredType & MetaRepo::getType(const MetaType * metaType) const
{
	return doGetType(metaType);
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
	return internal_::emptyRegisteredRepo;
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
