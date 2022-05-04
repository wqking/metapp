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
#include "metapp/interfaces/metaclass.h"

namespace metapp {

namespace internal_ {

extern MetaItem emptyMetaItem;
extern MetaItem emptyMetaItem;
extern MetaItem emptyMetaItem;

} // namespace internal_

MetaItem & MetaClass::registerConstructor(const Variant & constructor)
{
	if(constructor.getMetaType()->getMetaCallable() == nullptr) {
		errorWrongMetaType();
	}

	if(constructorItem.isEmpty()) {
		constructorItem = MetaItem(MetaItem::Type::constructor, "", constructor);
	}
	else {
		const Variant & target = constructorItem.asConstructor();
		constructorItem.setTarget(internal_::doCombineOverloadedCallable(target, constructor));
	}
	return constructorItem;
}

const MetaItem & MetaClass::getConstructor() const
{
	return constructorItem;
}

const MetaItem & MetaClass::getAccessible(const std::string & name, const Flags flags) const
{
	return doFindItemByName(&MetaClass::doGetAccessible, name, flags);
}

MetaItemView MetaClass::getAccessibleView(const Flags flags) const
{
	return doBuildMetaItemView(&MetaClass::doGetAccessibleList, flags);
}

const MetaItem & MetaClass::getCallable(const std::string & name, const Flags flags) const
{
	return doFindItemByName(&MetaClass::doGetCallable, name, flags);
}

MetaItemView MetaClass::getCallableView(const Flags flags) const
{
	return doBuildMetaItemView(&MetaClass::doGetCallableList, flags);
}

const MetaItem & MetaClass::getConstant(const std::string & name, const Flags flags) const
{
	return doFindItemByName(&MetaClass::doGetConstant, name, flags);
}

MetaItemView MetaClass::getConstantView(const Flags flags) const
{
	return doBuildMetaItemView(&MetaClass::doGetConstantList, flags);
}

const MetaItem & MetaClass::getType(const std::string & name, const Flags flags) const
{
	return doFindItemByName(&MetaClass::doGetType, name, flags);
}

const MetaItem & MetaClass::getType(const TypeKind kind, const Flags flags) const
{
	if(hasFlag(flags, flagIncludeBase)) {
		const MetaItem * result = &internal_::emptyMetaItem;
		getMetaRepo()->traverseBases(classMetaType, [&result, kind](const MetaType * metaType) -> bool {
			const MetaClass * metaClass = metaType->getMetaClass();
			if(metaClass != nullptr) {
				result = &metaClass->doGetType(kind);
				if(! result->isEmpty()) {
					return false;
				}
			}
			return true;
			});
		return *result;
	}
	else {
		return doGetType(kind);
	}
}

const MetaItem & MetaClass::getType(const MetaType * metaType, const Flags flags) const
{
	if(hasFlag(flags, flagIncludeBase)) {
		const MetaItem * result = &internal_::emptyMetaItem;
		getMetaRepo()->traverseBases(classMetaType, [&result, metaType](const MetaType * mt) -> bool {
			const MetaClass * metaClass = mt->getMetaClass();
			if(metaClass != nullptr) {
				result = &metaClass->doGetType(metaType);
				if(! result->isEmpty()) {
					return false;
				}
			}
			return true;
			});
		return *result;
	}
	else {
		return doGetType(metaType);
	}
}

MetaItemView MetaClass::getTypeView(const Flags flags) const
{
	return doBuildMetaItemView(&MetaClass::doGetTypeList, flags);
}

const MetaItem & MetaClass::getItem(const std::string & name) const
{
	return doGetItem(name);
}

MetaItemView MetaClass::doBuildMetaItemView(
		const MetaItemList & (MetaClass::*listGetter)() const,
		const Flags flags
	) const
{
	MetaItemView view;
	if(hasFlag(flags, flagIncludeBase)) {
		getMetaRepo()->traverseBases(classMetaType, [&view, &listGetter](const MetaType * metaType) -> bool {
			const MetaClass * metaClass = metaType->getMetaClass();
			if(metaClass != nullptr) {
				view.addContainer(&(metaClass->*listGetter)());
			}
			return true;
			});
	}
	else {
		view.addContainer(&(this->*listGetter)());
	}
	return view;
}

const MetaItem & MetaClass::doFindItemByName(
		const MetaItem & (MetaClass::*itemGetter)(const std::string &) const,
		const std::string & name,
		const Flags flags
	) const
{
	if(hasFlag(flags, flagIncludeBase)) {
		const MetaItem * result = &internal_::emptyMetaItem;
		getMetaRepo()->traverseBases(classMetaType, [&result, &itemGetter, &name](const MetaType * metaType) -> bool {
			const MetaClass * metaClass = metaType->getMetaClass();
			if(metaClass != nullptr) {
				result = &(metaClass->*itemGetter)(name);
				if(! result->isEmpty()) {
					return false;
				}
			}
			return true;
			});
		return *result;
	}
	else {
		return (this->*itemGetter)(name);
	}
}


} // namespace metapp
