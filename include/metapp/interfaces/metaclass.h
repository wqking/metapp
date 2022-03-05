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
