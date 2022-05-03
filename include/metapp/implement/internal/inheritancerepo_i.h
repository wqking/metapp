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

#ifndef METAPP_INHERITANCEREPO_H_969872685611
#define METAPP_INHERITANCEREPO_H_969872685611

#include "metapp/variant.h"
#include "metapp/metatype.h"
#include "metapp/implement/internal/disjointview_i.h"

#include <vector>
#include <deque>
#include <set>
#include <unordered_map>
#include <type_traits>
#include <cmath>
#include <algorithm>

namespace metapp {

namespace internal_ {

struct BaseDerived
{
	const MetaType * metaType;
	void * (*cast)(void * pointer);
};

class InheritanceRepo
{
private:
	struct BaseDerived
	{
		const MetaType * targetMetaType;
		void * (*cast)(void * pointer);

		operator const MetaType * const & () const {
			return targetMetaType;
		}
	};

	struct ClassInfo
	{
		std::deque<BaseDerived> baseList;
		std::deque<BaseDerived> derivedList;
	};

	static constexpr int maxInheritanceLevels = 256;

public:
	enum class Relationship
	{
		none,
		base,
		derived
	};

	using TypesView = internal_::DisjointView<const MetaType *, std::deque<BaseDerived>, 1>;

public:
	template <typename Class, typename ...Bases>
	void registerBase()
	{
		// Simulate C++17 fold expression in C++11
		using U = int[];
		U x { (doAddBase<Class, Bases>(), 0)... };
		(void)x; // avoid warning : expression result unused
	}

	template <typename Class>
	TypesView getBases() const
	{
		return getBases(doGetNormalizedMetaType<Class>());
	}

	TypesView getBases(const MetaType * classMetaType) const;

	template <typename Class>
	TypesView getDerives() const
	{
		return getDerives(doGetNormalizedMetaType<Class>());
	}

	TypesView getDerives(const MetaType * classMetaType) const;

	template <typename Class>
	void * castToBase(void * instance, const size_t baseIndex) const
	{
		return castToBase(instance, doGetNormalizedMetaType<Class>(), baseIndex);
	}

	void * castToBase(void * instance, const MetaType * classMetaType, const size_t baseIndex) const;

	template <typename Class>
	void * castToDerived(void * instance, const size_t derivedIndex) const
	{
		return castToDerived(instance, doGetNormalizedMetaType<Class>(), derivedIndex);
	}

	void * castToDerived(void * instance, const MetaType * classMetaType, const size_t derivedIndex) const;

	template <typename Class, typename ToClass>
	void * cast(void * instance) const
	{
		return cast(instance, doGetNormalizedMetaType<Class>(), doGetNormalizedMetaType<ToClass>());
	}

	void * cast(void * instance, const MetaType * classMetaType, const MetaType * toMetaType) const;

	template <typename Class, typename ToClass>
	Relationship getRelationship() const
	{
		return getRelationship(doGetNormalizedMetaType<Class>(), doGetNormalizedMetaType<ToClass>());
	}

	Relationship getRelationship(const MetaType * classMetaType, const MetaType * toMetaType) const;

	template <typename Class>
	bool isClassInHierarchy() const
	{
		return isClassInHierarchy(doGetNormalizedMetaType<Class>());
	}

	bool isClassInHierarchy(const MetaType * classMetaType) const;

	template <typename FT>
	bool traverseBases(
		const MetaType * metaType,
		FT && callback) const
	{
		std::set<const MetaType *> metaTypeSet;
		return doTraverseBases(metaType, std::forward<FT>(callback), metaTypeSet);
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
		static_assert(std::is_base_of<Base, Class>::value, "Class must derive from Base");

		using C = typename std::remove_cv<typename std::remove_reference<Class>::type>::type;
		using B = typename std::remove_cv<typename std::remove_reference<Base>::type>::type;
		const MetaType * classMetaType = getMetaType<C>();
		const MetaType * baseMetaType = getMetaType<B>();
		const void * classUnifiedType = classMetaType->getUnifiedType();
		const void * baseUnifiedType = baseMetaType->getUnifiedType();

		auto & thisClassInfo = classInfoMap[classUnifiedType];
		if(std::find_if(
			thisClassInfo.baseList.begin(),
			thisClassInfo.baseList.end(),
			[baseMetaType](const BaseDerived & item) {
				return item.targetMetaType == baseMetaType;
			}) == thisClassInfo.baseList.end()) {
			thisClassInfo.baseList.push_back({ baseMetaType, &castObject<C, B> });
		}
		
		auto & baseClassInfo = classInfoMap[baseUnifiedType];
		if(std::find_if(
			baseClassInfo.derivedList.begin(),
			baseClassInfo.derivedList.end(),
			[classMetaType](const BaseDerived & item) {
				return item.targetMetaType == classMetaType;
			}) == baseClassInfo.derivedList.end()) {
			baseClassInfo.derivedList.push_back({ classMetaType, &castObject<B, C> });
		}
	}

	template <typename T>
	static const MetaType * doGetNormalizedMetaType()
	{
		using U = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
		return getMetaType<U>();
	}

	const ClassInfo * doFindClassInfo(const void * type) const;
	const ClassInfo * doGetClassInfo(const void * type) const;

	static const ClassInfo * doGetDummyClassInfo();

	int doFindRelationship(
		BaseDerived * entryList,
		const MetaType * fromMetaType,
		const MetaType * toMetaType) const;

	int doFindBaseClass(
		BaseDerived * entryList,
		const ClassInfo * currentClassInfo,
		const void * targetBaseUnifiedType,
		const int level) const;

	int doFindDerivedClass(
		BaseDerived * entryList,
		const ClassInfo * currentClassInfo,
		const void * targetDerivedUnifiedType,
		const int level) const;

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

	template <typename FT>
	bool doTraverseBases(
		const MetaType * metaType,
		FT && callback,
		std::set<const MetaType *> & metaTypeSet) const
	{
		if(! metaTypeSet.insert(metaType).second) {
			return true;
		}
		if(! callback(metaType)) {
			return false;
		}
		const TypesView baseView = getBases(metaType);
		for(auto baseMetaType : baseView) {
			if(! doTraverseBases(baseMetaType, callback, metaTypeSet)) {
				return false;
			}
		}
		return true;
	}

private:
	std::unordered_map<const void *, ClassInfo> classInfoMap;
};

} // namespace internal_

} // namespace metapp

#endif
