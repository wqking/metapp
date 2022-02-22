#ifndef METACLASS_H_969872685611
#define METACLASS_H_969872685611

#include "metapp/metatype.h"

#include <vector>
#include <unordered_map>
#include <memory>

namespace metapp {

class MethodList
{
public:
	void addMethod(const Variant & method) {
		methodList.push_back(method);
	}

	size_t getCount() const {
		return methodList.size();
	}

	const Variant & get(const size_t index) const {
		return methodList[index];
	}

	const Variant * findCallable(const Variant * arguments, const size_t argumentCount) const {
		int maxRank = 0;
		const Variant * callable = nullptr;
		for(const Variant & method : methodList) {
			const int rank = method.getMetaType()->rankInvoke(arguments, argumentCount);
			if(rank > maxRank) {
				maxRank = rank;
				callable = &method;
			}
		}
		return callable;
	}

private:
	std::vector<Variant> methodList;
};

template <typename Map>
auto getPointerFromMap(const Map & map, const std::string & name)
	-> const typename Map::mapped_type *
{
	auto it = map.find(name);
	if(it == map.end()) {
		return nullptr;
	}
	return &it->second;
}

class MetaClass
{
public:
	explicit MetaClass(const MetaType * classMetaType)
		:
			classMetaType(classMetaType),
			baseList(),
			derivedList(),
			constructorList(),
			methodListMap(),
			fieldMap()
	{
	}

	size_t getBaseCount() const {
		return baseList.size();
	}

	const MetaType * getBase(const size_t index) const {
		return baseList.at(index);
	}

	void addConstructor(const Variant & constructor) {
		if(! constructor.getMetaType()->isCallable()) {
			return;
		}
		if(! constructorList) {
			constructorList = std::make_shared<MethodList>();
		}
		constructorList->addMethod(constructor);
	}
	
	const MethodList * getConstructorList() const {
		return constructorList.get();
	}

	void addMethod(const std::string & name, const Variant & method) {
		if(! method.getMetaType()->isCallable()) {
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
		return getPointerFromMap(fieldMap, name);
	}

protected:
	template <typename This, typename Base>
	void doAddBase() {
		const MetaType * base = getMetaType<Base>();
		const MetaClass * baseMetaClass = base->getMetaClass();
		if(baseMetaClass != nullptr) {
			baseMetaClass->addDerived(classMetaType);
		}

		baseList.push_back(base);
	}

private:
	void addDerived(const MetaType * derived) const
	{
		derivedList.push_back(derived);
	}

private:
	const MetaType * classMetaType;
	std::vector<const MetaType *> baseList;
	mutable std::vector<const MetaType *> derivedList;
	std::shared_ptr<MethodList> constructorList;
	std::unordered_map<std::string, std::shared_ptr<MethodList> > methodListMap;
	std::unordered_map<std::string, Variant> fieldMap;
};

template <typename Class>
class DeclareMetaClass : public MetaClass
{
private:
	using super = MetaClass;

public:
	DeclareMetaClass()
		: super(getMetaType<Class>())
	{
	}

	template <typename FT>
	DeclareMetaClass(FT callback)
		: super(getMetaType<Class>())
	{
		callback(*this);
	}

	template <typename ...Types>
	void addBase() {
		// Simulate C++17 fold expression in C++11
		using U = int[];
		U { (doAddBase<Class, Types>(), 0)... };
	}

};

} // namespace metapp

#endif
