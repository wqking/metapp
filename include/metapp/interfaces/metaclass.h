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

#ifndef METAPP_METACLASS_H_969872685611
#define METAPP_METACLASS_H_969872685611

#include "metapp/implement/internal/metaclass_i.h"
#include "metapp/implement/internal/metarepobase_i.h"

#include <vector>
#include <memory>

namespace metapp {

class MetaClass : public internal_::MetaRepoBase
{
public:
	explicit MetaClass(const MetaType * classMetaType)
		:
			classMetaType(classMetaType),
			baseList(),
			derivedList(),
			constructorList()
	{
	}

	size_t getBaseCount() const {
		return baseList.size();
	}

	const MetaType * getBase(const size_t index) const {
		return baseList.at(index).metaType;
	}

	size_t getDerivedCount() const {
		return derivedList.size();
	}

	const MetaType * getDerived(const size_t index) const {
		return derivedList.at(index).metaType;
	}

	void * castToBase(void * pointer, const size_t baseIndex) const {
		return baseList[baseIndex].cast(pointer);
	}

	void * castToDerived(void * pointer, const size_t derivedIndex) const {
		return derivedList[derivedIndex].cast(pointer);
	}

	void addConstructor(const Variant & constructor) {
		if(constructor.getMetaType()->getMetaCallable() == nullptr) {
			errorWrongMetaType();
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

protected:
	template <typename This, typename Base>
	void doAddBase() {
		const MetaType * base = getMetaType<Base>();
		const MetaClass * baseMetaClass = base->getMetaClass();
		if(baseMetaClass != nullptr) {
			baseMetaClass->derivedList.push_back({ classMetaType, &internal_::classCast<Base, This> });
		}

		baseList.push_back({ base, &internal_::classCast<This, Base> });
	}

private:
	const MetaType * classMetaType;
	std::vector<internal_::BaseDerived> baseList;
	mutable std::vector<internal_::BaseDerived> derivedList;
	std::shared_ptr<MethodList> constructorList;
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
