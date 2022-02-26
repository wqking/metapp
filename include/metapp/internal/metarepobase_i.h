#ifndef METAREPOBASE_I_H_969872685611
#define METAREPOBASE_I_H_969872685611

#include "metapp/metatype.h"
#include "metapp/methodlist.h"
#include "metapp/internal/util_i.h"

#include <map>
#include <memory>

namespace metapp {

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

	void addMethod(const std::string & name, const Variant & method) {
		if(method.getMetaType()->getMetaCallable() == nullptr) {
			return;
		}
		std::shared_ptr<MethodList> methodList;
		auto it = methodListMap.find(name);
		if(it == methodListMap.end()) {
			methodList = std::make_shared<MethodList>();
			methodListMap.insert(std::make_pair(name, methodList));
		}
		else {
			methodList = it->second;
		}
		methodList->addMethod(method);
	}

	const MethodList * getMethodList(const std::string & name) const {
		auto it = methodListMap.find(name);
		if(it == methodListMap.end()) {
			return nullptr;
		}
		return it->second.get();
	}

	void addField(const std::string & name, const Variant & field) {
		fieldMap[name]= field;
	}

	const Variant * getField(const std::string & name) const {
		return internal_::getPointerFromMap(fieldMap, name);
	}

private:
	std::map<std::string, const MetaType *> nameTypeMap;
	std::map<TypeKind, std::pair<std::string, const MetaType *> > kindTypeMap;
	std::map<std::string, std::shared_ptr<MethodList> > methodListMap;
	std::map<std::string, Variant> fieldMap;
};


} // namespace internal_

} // namespace metapp

#endif
