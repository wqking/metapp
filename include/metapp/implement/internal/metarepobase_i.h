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
#include "metapp/methodlist.h"
#include "metapp/exception.h"
#include "metapp/implement/internal/util_i.h"

#include <map>
#include <memory>
#include <functional>

namespace metapp {

class MemberInfo
{
public:
	MemberInfo()
		: name(), member()
	{
	}

	MemberInfo(const std::string * name, const Variant * member)
		: name(name), member(member)
	{
	}

	bool isValid() const {
		return member != nullptr;
	}

	const std::string & getName() const {
		assert(isValid());
		return *name;
	}

protected:
	const Variant & doGet() const {
		assert(isValid());
		return *member;
	}

private:
	const std::string * name;
	const Variant * member;
};

class FieldInfo : public MemberInfo
{
public:
	using MemberInfo::MemberInfo;

	const Variant & getField() const {
		return doGet();
	}

};

class MethodInfo : public MemberInfo
{
public:
	using MemberInfo::MemberInfo;

	const Variant & getMethod() const {
		return doGet();
	}

};

namespace internal_ {

class MetaRepoBase
{
public:
	MetaRepoBase()
		:
			nameTypeMap(),
			kindTypeMap(),
			methodListMap(),
			fieldMap()
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
		auto it = methodListMap.find(name);
		if(it == methodListMap.end()) {
			it = methodListMap.insert(std::make_pair(name, MethodList())).first;
		}
		it->second.addMethod(method);
	}

	MethodList getMethodList(const std::string & name) const {
		auto it = methodListMap.find(name);
		if(it == methodListMap.end()) {
			return MethodList();
		}
		return it->second;
	}

	void addField(const std::string & name, const Variant & field) {
		if(field.getMetaType()->getMetaAccessible() == nullptr) {
			errorWrongMetaType();
			return;
		}
		fieldMap[name]= field;
	}

	const Variant & getField(const std::string & name) const {
		auto field = internal_::getPointerFromMap(fieldMap, name);
		return field != nullptr ? *field : getEmptyVariant();
	}

protected:
	void doGetFields(std::vector<FieldInfo> & result) const {
		for(auto it = std::begin(fieldMap); it != std::end(fieldMap); ++it) {
			result.emplace_back(&it->first, &it->second);
		}
	}

	void doGetMethods(std::vector<MethodInfo> & result) const {
		for(auto it = std::begin(methodListMap); it != std::end(methodListMap); ++it) {
			const std::string & name = it->first;
			const MethodList & methodList = it->second;
			const size_t count = methodList.getCount();
			for(size_t i = 0; i < count; ++i) {
				result.emplace_back(&name, &methodList.get(i));
			}
		}
	}

	FieldInfo doGetField(const std::string & name) const {
		auto it = fieldMap.find(name);
		if(it != fieldMap.end()) {
			return FieldInfo(&it->first, &it->second);
		}
		return FieldInfo();
	}

private:
	std::map<std::string, const MetaType *> nameTypeMap;
	std::map<TypeKind, std::pair<std::string, const MetaType *> > kindTypeMap;
	std::map<std::string, MethodList> methodListMap;
	std::map<std::string, Variant> fieldMap;
};


} // namespace internal_

} // namespace metapp

#endif
