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
#include "metapp/allmetatypes.h"

namespace metapp {

namespace internal_ {

namespace {

RegisteredItemList dummyRegisteredItemList;

} // namespace

Variant emptyVariant;
std::string emptyString;
std::map<std::string, Variant> emptyAnnotationMap;
RegisteredItem emptyRegisteredItem;

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

InheritanceRepo::Relationship InheritanceRepo::getRelationship(const MetaType * classMetaType, const MetaType * toMetaType) const
{
	std::array<BaseDerived, maxInheritanceLevels> entryList;
	const int levels = doFindRelationship(entryList.data(), classMetaType, toMetaType);
	if(levels > 0) {
		return Relationship::base;
	}
	if(levels < 0) {
		return Relationship::derived;
	}
	return Relationship::none;
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

RegisteredItem & MetaRepoBase::registerAccessible(const std::string & name, const Variant & accessible)
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
	RegisteredItem & registeredAccessible = accessibleData->accessibleList.back();
	accessibleData->accessibleMap.insert(typename decltype(accessibleData->accessibleMap)::value_type(
		registeredAccessible.getName(), &registeredAccessible
	));
	return registeredAccessible;
}

RegisteredItem & MetaRepoBase::registerCallable(const std::string & name, const Variant & callable)
{
	if(callable.getMetaType()->getMetaCallable() == nullptr) {
		errorWrongMetaType();
	}

	if(! callableData) {
		callableData = std::make_shared<CallableData>();
	}

	auto it = callableData->callableMap.find(name);
	if(it != callableData->callableMap.end()) {
		const Variant & target = it->second->getTarget();
		if(getNonReferenceMetaType(target)->getTypeKind() == tkOverloadedFunction) {
			target.get<OverloadedFunction &>().addCallable(callable);
		}
		else {
			Variant newTarget = OverloadedFunction();
			OverloadedFunction & overloadedFunction = newTarget.get<OverloadedFunction &>();
			overloadedFunction.addCallable(target);
			overloadedFunction.addCallable(callable);
			const_cast<Variant &>(target) = newTarget;
		}
		return *it->second;
	}
	callableData->callableList.emplace_back(name, callable);
	RegisteredItem & registeredCallable = callableData->callableList.back();
	callableData->callableMap.insert(typename decltype(callableData->callableMap)::value_type(
		registeredCallable.getName(), &registeredCallable
	));
	return registeredCallable;
}

RegisteredItem & MetaRepoBase::registerType(std::string name, const MetaType * metaType)
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
	RegisteredItem & registeredType = typeData->typeList.back();
	if(! registeredType.getName().empty()) {
		typeData->nameTypeMap[registeredType.getName()] = &registeredType;
	}
	typeData->kindTypeMap[metaType->getTypeKind()] = &registeredType;
	typeData->typeTypeMap[metaType] = &registeredType;

	return registeredType;
}

void MetaRepoBase::doGetAccessibleList(RegisteredItemList * result) const
{
	if(accessibleData) {
		result->insert(result->end(), accessibleData->accessibleList.begin(), accessibleData->accessibleList.end());
	}
}

const RegisteredItemList & MetaRepoBase::doGetAccessibleList() const
{
	if(accessibleData) {
		return accessibleData->accessibleList;
	}
	else {
		return dummyRegisteredItemList;
	}
}

const RegisteredItem & MetaRepoBase::doGetAccessible(const std::string & name) const
{
	if(accessibleData) {
		auto it = accessibleData->accessibleMap.find(name);
		if(it != accessibleData->accessibleMap.end()) {
			return *it->second;
		}
	}
	return internal_::emptyRegisteredItem;
}

const RegisteredItem & MetaRepoBase::doGetCallable(const std::string & name) const
{
	if(callableData) {
		auto it = callableData->callableMap.find(name);
		if(it != callableData->callableMap.end()) {
			return *it->second;
		}
	}
	return internal_::emptyRegisteredItem;
}

void MetaRepoBase::doGetCallableList(RegisteredItemList * result) const
{
	if(callableData) {
		result->insert(result->end(), callableData->callableList.begin(), callableData->callableList.end());
	}
}

const RegisteredItemList & MetaRepoBase::doGetCallableList() const
{
	if(callableData) {
		return callableData->callableList;
	}
	else {
		return dummyRegisteredItemList;
	}
}

const RegisteredItem & MetaRepoBase::doGetType(const std::string & name) const
{
	if(typeData) {
		auto it = typeData->nameTypeMap.find(name);
		if(it != typeData->nameTypeMap.end()) {
			return *it->second;
		}
	}
	return internal_::emptyRegisteredItem;
}

const RegisteredItem & MetaRepoBase::doGetType(const TypeKind kind) const
{
	if(typeData) {
		auto it = typeData->kindTypeMap.find(kind);
		if(it != typeData->kindTypeMap.end()) {
			return *it->second;
		}
	}
	return internal_::emptyRegisteredItem;
}

const RegisteredItem & MetaRepoBase::doGetType(const MetaType * metaType) const
{
	if(typeData) {
		auto it = typeData->typeTypeMap.find(metaType);
		if(it != typeData->typeTypeMap.end()) {
			return *it->second;
		}
	}
	return internal_::emptyRegisteredItem;
}

void MetaRepoBase::doGetTypeList(RegisteredItemList * result) const
{
	if(typeData) {
		result->insert(result->end(), typeData->typeList.begin(), typeData->typeList.end());
	}
}

const RegisteredItemList & MetaRepoBase::doGetTypeList() const
{
	if(typeData) {
		return typeData->typeList;
	}
	else {
		return dummyRegisteredItemList;
	}
}

} // namespace internal_


void RegisteredItem::registerAnnotation(const std::string & name, const Variant & value)
{
	if(! annotationMap) {
		annotationMap = std::make_shared<std::map<std::string, Variant> >();
	}
	annotationMap->insert(std::make_pair(name, value));
}

const Variant & RegisteredItem::getAnnotation(const std::string & name) const
{
	if(annotationMap) {
		auto it = annotationMap->find(name);
		if(it != annotationMap->end()) {
			return it->second;
		}
	}
	return internal_::emptyVariant;
}

const std::map<std::string, Variant> & RegisteredItem::getAllAnnotations() const
{
	if(annotationMap) {
		return *annotationMap;
	}
	return internal_::emptyAnnotationMap;
}

const std::string & RegisteredItem::getName() const
{
	return data ? data->name : internal_::emptyString;
}

const Variant & RegisteredItem::getTarget() const
{
	return data ? data->target : internal_::emptyVariant;
}


MetaRepo * getMetaRepo()
{
	static MetaRepo globalRepo;

	return &globalRepo;
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

const RegisteredItem & MetaRepo::getAccessible(const std::string & name) const
{
	return doGetAccessible(name);
}

const RegisteredItemList & MetaRepo::getAccessibleList() const
{
	return doGetAccessibleList();
}

const RegisteredItem & MetaRepo::getCallable(const std::string & name) const
{
	return doGetCallable(name);
}

const RegisteredItemList & MetaRepo::getCallableList() const
{
	return doGetCallableList();
}

const RegisteredItem & MetaRepo::getType(const std::string & name) const
{
	return doGetType(name);
}

const RegisteredItem & MetaRepo::getType(const TypeKind kind) const
{
	return doGetType(kind);
}

const RegisteredItem & MetaRepo::getType(const MetaType * metaType) const
{
	return doGetType(metaType);
}

const RegisteredItemList & MetaRepo::getTypeList() const
{
	return doGetTypeList();
}

RegisteredItem & MetaRepo::registerRepo(const std::string & name, MetaRepo * repo)
{
	if(repo == nullptr) {
		repo = new MetaRepo();
	}
	repoList.emplace_back(name, repo);
	RegisteredItem & registeredRepo = repoList.back();
	repoMap[registeredRepo.getName()] = &registeredRepo;

	return registeredRepo;
}

const RegisteredItem & MetaRepo::getRepo(const std::string & name) const
{
	auto it = repoMap.find(name);
	if(it != repoMap.end()) {
		return *it->second;
	}
	return internal_::emptyRegisteredItem;
}

const RegisteredItemList & MetaRepo::getRepoList() const
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
