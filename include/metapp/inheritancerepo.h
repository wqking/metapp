#ifndef METAPP_INHERITANCEREPO_H_969872685611
#define METAPP_INHERITANCEREPO_H_969872685611

#include "metapp/variant.h"
#include "metapp/metatype.h"

#include <vector>
#include <unordered_map>
#include <type_traits>
#include <cmath>

namespace metapp {

enum class InheritanceRelationship
{
	none,
	base,
	derived
};

class InheritanceRepo
{
private:
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

	static constexpr size_t maxInheritanceLevels = 256;

public:
	template <typename Class, typename ...Bases>
	void addBase()
	{
		// Simulate C++17 fold expression in C++11
		using U = int[];
		U { (doAddBase<Class, Bases>(), 0)... };
	}

	template <typename Class>
	size_t getBaseCount() const
	{
		return getBaseCount(doGetNormalizedMetaType<Class>());
	}

	size_t getBaseCount(const MetaType * classMetaType) const
	{
		return doGetClassInfo(classMetaType->getUnifiedType())->baseList.size();
	}

	template <typename Class>
	const MetaType * getBaseMetaType(const size_t index) const
	{
		return getBaseMetaType(doGetNormalizedMetaType<Class>(), index);
	}

	const MetaType * getBaseMetaType(const MetaType * classMetaType, const size_t index) const
	{
		return doGetClassInfo(classMetaType->getUnifiedType())->baseList[index].targetMetaType;
	}

	template <typename Class>
	size_t getDerivedCount() const
	{
		return getDerivedCount(doGetNormalizedMetaType<Class>());
	}

	size_t getDerivedCount(const MetaType * classMetaType) const
	{
		return doGetClassInfo(classMetaType->getUnifiedType())->derivedList.size();
	}

	template <typename Class>
	const MetaType * getDerivedMetaType(const size_t index) const
	{
		return getDerivedMetaType(doGetNormalizedMetaType<Class>(), index);
	}

	const MetaType * getDerivedMetaType(const MetaType * classMetaType, const size_t index) const
	{
		return doGetClassInfo(classMetaType->getUnifiedType())->derivedList[index].targetMetaType;
	}

	template <typename Class>
	void * castToBase(void * instance, const size_t baseIndex) const
	{
		return castToBase(instance, doGetNormalizedMetaType<Class>(), baseIndex);
	}

	void * castToBase(void * instance, const MetaType * classMetaType, const size_t baseIndex) const
	{
		if(instance == nullptr) {
			return nullptr;
		}
		return doGetClassInfo(classMetaType->getUnifiedType())->baseList[baseIndex].cast(instance);
	}

	template <typename Class>
	void * castToDerived(void * instance, const size_t derivedIndex) const
	{
		return castToDerived(instance, doGetNormalizedMetaType<Class>(), derivedIndex);
	}

	void * castToDerived(void * instance, const MetaType * classMetaType, const size_t derivedIndex) const
	{
		if(instance == nullptr) {
			return nullptr;
		}
		return doGetClassInfo(classMetaType->getUnifiedType())->derivedList[derivedIndex].cast(instance);
	}

	template <typename Class, typename ToClass>
	void * cast(void * instance) const
	{
		return cast(instance, doGetNormalizedMetaType<Class>(), doGetNormalizedMetaType<ToClass>());
	}

	void * cast(void * instance, const MetaType * classMetaType, const MetaType * toMetaType) const
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

	template <typename Class, typename ToClass>
	InheritanceRelationship getRelationship() const
	{
		return getRelationship(doGetNormalizedMetaType<Class>(), doGetNormalizedMetaType<ToClass>());
	}

	InheritanceRelationship getRelationship(const MetaType * classMetaType, const MetaType * toMetaType) const
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

	template <typename Class>
	bool doesClassExist() const
	{
		return doesClassExist(doGetNormalizedMetaType<Class>());
	}

	bool doesClassExist(const MetaType * classMetaType) const
	{
		return doFindClassInfo(classMetaType->getUnifiedType()) != nullptr;
	}

	// for test purpose, don't call it in production code
	void clear()
	{
		classInfoMap.clear();
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

	template <typename T>
	static const MetaType * doGetNormalizedMetaType()
	{
		using U = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
		return getMetaType<U>();
	}

	const ClassInfo * doFindClassInfo(const void * type) const
	{
		auto it = classInfoMap.find(type);
		if(it != classInfoMap.end()) {
			return &it->second;
		}
		return nullptr;
	}

	const ClassInfo * doGetClassInfo(const void * type) const
	{
		const ClassInfo * result = doFindClassInfo(type);
		if(result == nullptr) {
			return doGetDummyClassInfo();
		}
		return result;
	}

	static const ClassInfo * doGetDummyClassInfo()
	{
		static ClassInfo classInfo {};
		return &classInfo;
	}

	int doFindRelationship(
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

	int doFindBaseClass(
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

	int doFindDerivedClass(
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

	template <typename From, typename To>
	static void * castObject(void * pointer)
	{
		return doCastObject<From, To>(pointer);
	}

	template <typename From, typename To>
	static void * doCastObject(void * pointer,
		typename std::enable_if<internal_::CanStaticCast<From *, To *>::value>::type * = nullptr)
	{
		return static_cast<To *>((From *)pointer);
	}

	template <typename From, typename To>
	static void * doCastObject(void * pointer,
		typename std::enable_if<
		! internal_::CanStaticCast<From *, To *>::value
		&& internal_::CanDynamicCast<From *, To *>::value
		>::type * = nullptr)
	{
		return dynamic_cast<To *>((From *)pointer);
	}

	template <typename From, typename To>
	static void * doCastObject(void * /*pointer*/,
		typename std::enable_if<
		! internal_::CanStaticCast<From *, To *>::value
		&& ! internal_::CanDynamicCast<From *, To *>::value
		>::type * = nullptr)
	{
		errorBadCast();
		return nullptr;
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
