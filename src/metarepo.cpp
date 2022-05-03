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

MetaItemList dummyMetaItemList;

} // namespace

Variant emptyVariant;
std::string emptyString;
std::map<std::string, Variant> emptyAnnotationMap;
MetaItem emptyMetaItem;

BaseView InheritanceRepo::getBases(const MetaType * classMetaType) const
{
	return BaseView(&doGetClassInfo(classMetaType->getUnifiedType())->baseList);
}

BaseView InheritanceRepo::getDerives(const MetaType * classMetaType) const
{
	return BaseView(&doGetClassInfo(classMetaType->getUnifiedType())->derivedList);
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
		typeData(),
		callableData(),
		accessibleData()
{
}

MetaItem & MetaRepoBase::registerAccessible(const std::string & name, const Variant & accessible)
{
	if(accessible.getMetaType()->getMetaAccessible() == nullptr) {
		errorWrongMetaType();
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
		errorWrongMetaType();
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
	MetaItem & registeredType = typeData->addItem(MetaItem::Type::metaType, name, Variant::create(metaType));
	typeData->kindTypeMap[metaType->getTypeKind()] = &registeredType;
	typeData->typeTypeMap[metaType] = &registeredType;

	return registeredType;
}

const MetaItem & MetaRepoBase::doGetAccessible(const std::string & name) const
{
	if(accessibleData) {
		return accessibleData->findItem(name);
	}
	return internal_::emptyMetaItem;
}

const MetaItemList & MetaRepoBase::doGetAccessibleList() const
{
	if(accessibleData) {
		return accessibleData->itemList;
	}
	else {
		return dummyMetaItemList;
	}
}

const MetaItem & MetaRepoBase::doGetCallable(const std::string & name) const
{
	if(callableData) {
		return callableData->findItem(name);
	}
	return internal_::emptyMetaItem;
}

const MetaItemList & MetaRepoBase::doGetCallableList() const
{
	if(callableData) {
		return callableData->itemList;
	}
	else {
		return dummyMetaItemList;
	}
}

const MetaItem & MetaRepoBase::doGetType(const std::string & name) const
{
	if(typeData) {
		return typeData->findItem(name);
	}
	return internal_::emptyMetaItem;
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
	if(typeData) {
		return typeData->itemList;
	}
	else {
		return dummyMetaItemList;
	}
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


} // namespace internal_


MetaItem::MetaItem()
	: data()
{
}

MetaItem::MetaItem(const Type type, const std::string & name, const Variant & target)
	: data(std::make_shared<Data>(type, name, target))
{
}

MetaItem::~MetaItem()
{
}

void MetaItem::registerAnnotation(const std::string & name, const Variant & value)
{
	if(! annotationMap) {
		annotationMap = std::make_shared<std::map<std::string, Variant> >();
	}
	annotationMap->insert(std::make_pair(name, value));
}

const Variant & MetaItem::getAnnotation(const std::string & name) const
{
	if(annotationMap) {
		auto it = annotationMap->find(name);
		if(it != annotationMap->end()) {
			return it->second;
		}
	}
	return internal_::emptyVariant;
}

const std::map<std::string, Variant> & MetaItem::getAllAnnotations() const
{
	if(annotationMap) {
		return *annotationMap;
	}
	return internal_::emptyAnnotationMap;
}

const std::string & MetaItem::getName() const
{
	return data ? data->name : internal_::emptyString;
}

MetaItem::Type MetaItem::getType() const
{
	return data ? data->type : MetaItem::Type::none;
}

const Variant & MetaItem::asAccessible() const
{
	doCheckType(Type::accessible);
	return doGetVariant();
}

const Variant & MetaItem::asCallable() const
{
	doCheckType(Type::callable);
	return doGetVariant();
}

const Variant & MetaItem::asConstructor() const
{
	doCheckType(Type::constructor);
	return doGetVariant();
}

const MetaType * MetaItem::asMetaType() const
{
	doCheckType(Type::metaType);
	return doGetVariant().get<const MetaType *>();
}

const MetaRepo * MetaItem::asMetaRepo() const
{
	doCheckType(Type::metaRepo);
	return doGetVariant().get<const MetaRepo *>();
}

const Variant & MetaItem::asEnumValue() const
{
	doCheckType(Type::enumValue);
	return doGetVariant();
}

MetaItem::operator const Variant & () const
{
	return doGetVariant();
}

void MetaItem::setTarget(const Variant & target)
{
	if(data) {
		data->target = target;
	}
}

const Variant & MetaItem::doGetVariant() const
{
	return data ? data->target : internal_::emptyVariant;
}

void MetaItem::doCheckType(const Type type) const
{
	if(getType() != type) {
		errorIllegalArgument();
	}
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
		repoData()
{
	registerBuiltinTypes();
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

MetaItem & MetaRepo::registerRepo(const std::string & name, MetaRepo * repo)
{
	if(repo == nullptr) {
		repo = new MetaRepo();
	}
	if(! repoData) {
		repoData = std::make_shared<ItemData>();
	}
	return repoData->addItem(MetaItem::Type::metaRepo, name, repo);
}

const MetaItem & MetaRepo::getRepo(const std::string & name) const
{
	if(repoData) {
		return repoData->findItem(name);
	}
	return internal_::emptyMetaItem;
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
