#ifndef METAPP_INHERITANCEREPO_H_969872685611
#define METAPP_INHERITANCEREPO_H_969872685611

#include "metapp/variant.h"
#include "metapp/metatype.h"

#include <unordered_map>
#include <type_traits>

namespace metapp {

class InheritanceRepo
{
private:
	template <typename From, typename To>
	static void * castObject(void * pointer)
	{
		return static_cast<To *>((From *)pointer);
	}

	struct BaseDerived
	{
		const MetaType * targetMetaType;
		void * (*cast)(void * pointer);
	};

	struct ClassInfo
	{
		std::vector<BaseDerived> baseList;
		std::vector<BaseDerived> derivedList;
	};

public:
	template <typename Class, typename ...Bases>
	void addBase()
	{
		// Simulate C++17 fold expression in C++11
		using U = int[];
		U { (doAddBase<Class, Bases>(), 0)... };
	}

	size_t getBaseCount(const MetaType * classMetaType)
	{
		return findClassInfo(classMetaType->getUnifiedType())->baseList.size();
	}

	const MetaType * getBaseMetaType(const MetaType * classMetaType, const size_t index)
	{
		return findClassInfo(classMetaType->getUnifiedType())->baseList[index].targetMetaType;
	}

	size_t getDerivedCount(const MetaType * classMetaType)
	{
		return findClassInfo(classMetaType->getUnifiedType())->derivedList.size();
	}

	const MetaType * getDerivedMetaType(const MetaType * classMetaType, const size_t index)
	{
		return findClassInfo(classMetaType->getUnifiedType())->derivedList[index].targetMetaType;
	}

	void * castToBase(const MetaType * classMetaType, void * pointer, const size_t baseIndex) const {
		return findClassInfo(classMetaType->getUnifiedType())->baseList[baseIndex].cast(pointer);
	}

	void * castToDerived(const MetaType * classMetaType, void * pointer, const size_t derivedIndex) const {
		return findClassInfo(classMetaType->getUnifiedType())->derivedList[derivedIndex].cast(pointer);
	}

private:
	template <typename Class, typename Base>
	void doAddBase()
	{
		using C = typename std::remove_cv<typename std::remove_reference<Class>::type>::type;
		using B = typename std::remove_cv<typename std::remove_reference<Base>::type>::type;
		const MetaType * classMetaType = getMetaType<C>();
		const MetaType * baseMetaType = getMetaType<B>();
		const void * classUnifiedType = classMetaType->getUnifiedType();
		const void * baseUnifiedType = baseMetaType->getUnifiedType();

		auto & thisClassInfo = classInfoMap[classUnifiedType];
		thisClassInfo.baseList.push_back({ baseMetaType, &castObject<C, B> });
		
		auto & baseClassInfo = classInfoMap[baseUnifiedType];
		baseClassInfo.derivedList.push_back({ classMetaType, &castObject<B, C> });
	}

	const ClassInfo * findClassInfo(const void * type) const
	{
		auto it = classInfoMap.find(type);
		if(it != classInfoMap.end()) {
			return &it->second;
		}
		return doGetDummyClassInfo();
	}

	static const ClassInfo * doGetDummyClassInfo()
	{
		static ClassInfo classInfo {};
		return &classInfo;
	}

private:
	std::unordered_map<const void *, ClassInfo> classInfoMap;
};

inline InheritanceRepo * getInheritanceRepo()
{
	static InheritanceRepo repo;
	return &repo;
}


} // namespace metapp

#endif
