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
#include "metapp/allmetatypes.h"

namespace metapp {

namespace internal_ {

Variant emptyVariant;
std::string emptyString;
MetaItem emptyMetaItem;
MetaItemList emptyMetaItemList;

BaseView InheritanceRepo::getBases(const MetaType * classMetaType) const
{
	return BaseView(&doGetClassInfo(classMetaType)->baseList);
}

BaseView InheritanceRepo::getDeriveds(const MetaType * classMetaType) const
{
	return BaseView(&doGetClassInfo(classMetaType)->derivedList);
}

void * InheritanceRepo::castToBase(void * instance, const MetaType * classMetaType, const int baseIndex) const
{
	if(instance == nullptr) {
		return nullptr;
	}
	return doGetClassInfo(classMetaType)->baseList[baseIndex].cast(instance);
}

void * InheritanceRepo::castToDerived(void * instance, const MetaType * classMetaType, const int derivedIndex) const
{
	if(instance == nullptr) {
		return nullptr;
	}
	return doGetClassInfo(classMetaType)->derivedList[derivedIndex].cast(instance);
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
	return doFindClassInfo(classMetaType) != nullptr;
}

const InheritanceRepo::ClassInfo * InheritanceRepo::doFindClassInfo(const MetaType * type) const
{
	auto it = classInfoMap.find(type);
	if(it != classInfoMap.end()) {
		return &it->second;
	}
	return nullptr;
}

const InheritanceRepo::ClassInfo * InheritanceRepo::doGetClassInfo(const MetaType * type) const
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
	const ClassInfo * currentClassInfo = doFindClassInfo(fromMetaType);
	if(currentClassInfo == nullptr) {
		return 0;
	}
	if(doFindClassInfo(toMetaType) == nullptr) {
		return 0;
	}
	int levels = doFindBaseClass(entryList, currentClassInfo, toMetaType, 0);
	if(levels > 0) {
		return levels;
	}
	levels = doFindDerivedClass(entryList, currentClassInfo, toMetaType, 0);
	return -levels;
}

int InheritanceRepo::doFindBaseClass(
	BaseDerived * entryList,
	const ClassInfo * currentClassInfo,
	const MetaType * targetBaseMetaType,
	const int level) const
{
	assert(level < maxInheritanceLevels);

	const int count = static_cast<int>(currentClassInfo->baseList.size());
	for(int i = 0; i < count; ++i) {
		entryList[level] = currentClassInfo->baseList[i];
		if(currentClassInfo->baseList[i].targetMetaType->equal(targetBaseMetaType)) {
			return level + 1;
		}
		const ClassInfo * baseClassInfo = doGetClassInfo(currentClassInfo->baseList[i].targetMetaType);
		const int nextLevel = doFindBaseClass(entryList, baseClassInfo, targetBaseMetaType, level + 1);
		if(nextLevel > 0) {
			return nextLevel;
		}
	}
	return 0;
}

int InheritanceRepo::doFindDerivedClass(
	BaseDerived * entryList,
	const ClassInfo * currentClassInfo,
	const MetaType * targetDerivedMetaType,
	const int level) const
{
	assert(level < maxInheritanceLevels);

	const int count = static_cast<int>(currentClassInfo->derivedList.size());
	for(int i = 0; i < count; ++i) {
		entryList[level] = currentClassInfo->derivedList[i];
		if(currentClassInfo->derivedList[i].targetMetaType->equal(targetDerivedMetaType)) {
			return level + 1;
		}
		const ClassInfo * derivedClassInfo = doGetClassInfo(currentClassInfo->derivedList[i].targetMetaType);
		const int nextLevel = doFindDerivedClass(entryList, derivedClassInfo, targetDerivedMetaType, level + 1);
		if(nextLevel > 0) {
			return nextLevel;
		}
	}
	return 0;
}

MetaItem & MetaRepoBase::ItemData::addItem(const MetaItem::Type type, const std::string & name, const Variant & target)
{
	itemList.emplace_back(type, name, target);
	MetaItem & item = itemList.back();
	if(! item.getName().empty()) {
		nameItemMap.insert(typename decltype(nameItemMap)::value_type(
			item.getName(), &item
		));
	}
	return item;
}

const MetaItem & MetaRepoBase::ItemData::findItem(const std::string & name) const
{
	auto it = nameItemMap.find(name);
	if(it != nameItemMap.end()) {
		return *it->second;
	}
	
	return internal_::emptyMetaItem;
}

MetaRepoBase::MetaRepoBase()
	:
		accessibleData(),
		callableData(),
		constantData(),
		typeData()
{
}

MetaItem & MetaRepoBase::registerAccessible(const std::string & name, const Variant & accessible)
{
	if(accessible.getMetaType()->getMetaAccessible() == nullptr) {
		raiseException<WrongMetaTypeException>();
	}

	if(! accessibleData) {
		accessibleData = std::make_shared<ItemData>();
	}

	auto it = accessibleData->nameItemMap.find(name);
	if(it != accessibleData->nameItemMap.end()) {
		return *it->second;
	}
	return accessibleData->addItem(MetaItem::Type::accessible, name, accessible);
}

MetaItem & MetaRepoBase::registerCallable(const std::string & name, const Variant & callable)
{
	if(callable.getMetaType()->getMetaCallable() == nullptr) {
		raiseException<WrongMetaTypeException>();
	}

	if(! callableData) {
		callableData = std::make_shared<ItemData>();
	}

	auto it = callableData->nameItemMap.find(name);
	if(it != callableData->nameItemMap.end()) {
		const Variant & target = it->second->asCallable();
		it->second->setTarget(doCombineOverloadedCallable(target, callable));
		return *it->second;
	}
	return callableData->addItem(MetaItem::Type::callable, name, callable);
}

MetaItem & MetaRepoBase::registerVariable(const std::string & name, const Variant & variable)
{
	if(! constantData) {
		constantData = std::make_shared<ItemData>();
	}

	auto it = constantData->nameItemMap.find(name);
	if(it != constantData->nameItemMap.end()) {
		return *it->second;
	}
	return constantData->addItem(MetaItem::Type::variable, name, variable);
}

MetaItem & MetaRepoBase::registerType(std::string name, const MetaType * metaType)
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
	MetaItem & registeredType = typeData->addItem(MetaItem::Type::metaType, name, metaType);
	typeData->kindTypeMap[metaType->getTypeKind()] = &registeredType;
	typeData->typeTypeMap[metaType] = &registeredType;

	return registeredType;
}

const MetaItem & MetaRepoBase::doGetAccessible(const std::string & name) const
{
	return doFindItemByName(accessibleData, name);
}

const MetaItemList & MetaRepoBase::doGetAccessibleList() const
{
	return doGetItemList(accessibleData);
}

const MetaItem & MetaRepoBase::doGetCallable(const std::string & name) const
{
	return doFindItemByName(callableData, name);
}

const MetaItemList & MetaRepoBase::doGetCallableList() const
{
	return doGetItemList(callableData);
}

const MetaItem & MetaRepoBase::doGetVariable(const std::string & name) const
{
	return doFindItemByName(constantData, name);
}

const MetaItemList & MetaRepoBase::doGetVariableList() const
{
	return doGetItemList(constantData);
}

const MetaItem & MetaRepoBase::doGetType(const std::string & name) const
{
	return doFindItemByName(typeData, name);
}

const MetaItem & MetaRepoBase::doGetType(const TypeKind kind) const
{
	if(typeData) {
		auto it = typeData->kindTypeMap.find(kind);
		if(it != typeData->kindTypeMap.end()) {
			return *it->second;
		}
	}
	return internal_::emptyMetaItem;
}

const MetaItem & MetaRepoBase::doGetType(const MetaType * metaType) const
{
	if(typeData) {
		auto it = typeData->typeTypeMap.find(metaType);
		if(it != typeData->typeTypeMap.end()) {
			return *it->second;
		}
	}
	return internal_::emptyMetaItem;
}

const MetaItemList & MetaRepoBase::doGetTypeList() const
{
	return doGetItemList(typeData);
}

const MetaItem & MetaRepoBase::doGetItem(const std::string & name) const
{
	const MetaItem * result = &internal_::emptyMetaItem;
	doFindItemByName(accessibleData, name, result)
		|| doFindItemByName(callableData, name, result)
		|| doFindItemByName(typeData, name, result)
	;
	return *result;
}

Variant doCombineOverloadedCallable(const Variant & target, const Variant & callable)
{
	if(getNonReferenceMetaType(target)->getTypeKind() == tkOverloadedFunction) {
		target.get<OverloadedFunction &>().addCallable(callable);
		return target;
	}
	else {
		Variant newTarget = OverloadedFunction();
		OverloadedFunction & overloadedFunction = newTarget.get<OverloadedFunction &>();
		overloadedFunction.addCallable(target);
		overloadedFunction.addCallable(callable);
		return newTarget;
	}
}

MetaRepoList * doGetMetaRepoList()
{
	static MetaRepoList metaRepoList;

	return &metaRepoList;
}

} // namespace internal_


MetaRepo::MetaRepo()
	:
		internal_::MetaRepoBase(),
		internal_::InheritanceRepo(),
		repoData(),
		previous(nullptr),
		next(nullptr)
{
	internal_::doGetMetaRepoList()->addMetaRepo(this);
}

MetaRepo::~MetaRepo()
{
	internal_::doGetMetaRepoList()->removeMetaRepo(this);
}

const MetaItem & MetaRepo::getAccessible(const std::string & name) const
{
	return doGetAccessible(name);
}

MetaItemView MetaRepo::getAccessibleView() const
{
	return MetaItemView(&doGetAccessibleList());
}

const MetaItem & MetaRepo::getCallable(const std::string & name) const
{
	return doGetCallable(name);
}

MetaItemView MetaRepo::getCallableView() const
{
	return MetaItemView(&doGetCallableList());
}

const MetaItem & MetaRepo::getVariable(const std::string & name) const
{
	return doGetVariable(name);
}

MetaItemView MetaRepo::getVariableView() const
{
	return MetaItemView(&doGetVariableList());
}

const MetaItem & MetaRepo::getType(const std::string & name) const
{
	return doGetType(name);
}

const MetaItem & MetaRepo::getType(const TypeKind kind) const
{
	return doGetType(kind);
}

const MetaItem & MetaRepo::getType(const MetaType * metaType) const
{
	return doGetType(metaType);
}

MetaItemView MetaRepo::getTypeView() const
{
	return MetaItemView(&doGetTypeList());
}

MetaItem & MetaRepo::registerRepo(const std::string & name, Variant repo)
{
	if(repo.isEmpty()) {
		repo = Variant(MetaRepo());
	}
	if(! repoData) {
		repoData = std::make_shared<ItemData>();
	}
	return repoData->addItem(MetaItem::Type::metaRepo, name, repo);
}

const MetaItem & MetaRepo::getRepo(const std::string & name) const
{
	return doFindItemByName(repoData, name);
}

MetaItemView MetaRepo::getRepoView() const
{
	return repoData ? MetaItemView(&repoData->itemList) : MetaItemView();
}

const MetaItem & MetaRepo::getItem(const std::string & name) const
{
	const MetaItem * result = &doGetItem(name);
	if(result->isEmpty()) {
		doFindItemByName(repoData, name, result);
	}
	return *result;
}


const MetaRepoList * getMetaRepoList()
{
	return internal_::doGetMetaRepoList();
}

MetaRepoList::MetaRepoList()
	:
		head(nullptr),
		tail(nullptr)
{
}

MetaRepoList::~MetaRepoList()
{
}

void MetaRepoList::addMetaRepo(MetaRepo * repo)
{
	if(head == nullptr) {
		head = repo;
		tail = repo;
	}
	else {
		repo->previous = tail;
		tail->next = repo;
		tail = repo;
	}
}

void MetaRepoList::removeMetaRepo(MetaRepo * repo)
{
	if(repo->next != nullptr) {
		repo->next->previous = repo->previous;
	}
	if(repo->previous != nullptr) {
		repo->previous->next = repo->next;
	}
	if(head == repo) {
		head = repo->next;
	}
	if(tail == repo) {
		tail = repo->previous;
	}
}

const MetaRepo * MetaRepoList::findMetaRepoForHierarchy(const MetaType * classMetaType) const
{
	const MetaRepo * repo = head;
	while(repo != nullptr) {
		if(repo->isClassInHierarchy(classMetaType)) {
			return repo;
		}
		repo = repo->next;
	}
	return nullptr;
}


} // namespace metapp
