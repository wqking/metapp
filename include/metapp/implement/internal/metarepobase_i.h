#ifndef METAPP_METAREPOBASE_I_H_969872685611
#define METAPP_METAREPOBASE_I_H_969872685611

#include "metapp/metatype.h"
#include "metapp/methodlist.h"
#include "metapp/exception.h"
#include "metapp/implement/internal/util_i.h"

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

	std::vector<std::string> getTypeNameList() const {
		return internal_::getMapKeys(nameTypeMap);
	}

	void addMethod(const std::string & name, const Variant & method) {
		if(method.getMetaType()->getMetaCallable() == nullptr) {
			errorWrongMetaType();
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

	std::vector<std::string> getMethodNameList() const {
		return internal_::getMapKeys(methodListMap);
	}

	void addField(const std::string & name, const Variant & field) {
		if(field.getMetaType()->getMetaAccessible() == nullptr) {
			errorWrongMetaType();
			return;
		}
		fieldMap[name]= field;
	}

	const Variant * getField(const std::string & name) const {
		return internal_::getPointerFromMap(fieldMap, name);
	}

	std::vector<std::string> getFieldNameList() const {
		return internal_::getMapKeys(fieldMap);
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
