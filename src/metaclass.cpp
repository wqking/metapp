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

extern RegisteredItem emptyRegisteredItem;
extern RegisteredItem emptyRegisteredItem;
extern RegisteredItem emptyRegisteredItem;

} // namespace internal_

RegisteredItem & MetaClass::registerConstructor(const Variant & constructor)
{
	if(constructor.getMetaType()->getMetaCallable() == nullptr) {
		errorWrongMetaType();
	}
	constructorList.push_back(RegisteredItem(constructor));
	return constructorList.back();
}

const RegisteredItemList & MetaClass::getConstructorList() const
{
	return constructorList;
}

const RegisteredItem & MetaClass::getAccessible(const std::string & name, const Flags flags) const
{
	if(hasFlag(flags, flagIncludeBase)) {
		const RegisteredItem * result = &internal_::emptyRegisteredItem;
		getMetaRepo()->traverseBases(classMetaType, [&result, &name](const MetaType * metaType) -> bool {
			const MetaClass * metaClass = metaType->getMetaClass();
			if(metaClass != nullptr) {
				result = &metaClass->doGetAccessible(name);
				if(! result->isEmpty()) {
					return false;
				}
			}
			return true;
			});
		return *result;
	}
	else {
		return doGetAccessible(name);
	}
}

RegisteredItemList MetaClass::getAccessibleList(const Flags flags) const
{
	RegisteredItemList result;
	if(hasFlag(flags, flagIncludeBase)) {
		getMetaRepo()->traverseBases(classMetaType, [&result](const MetaType * metaType) -> bool {
			const MetaClass * metaClass = metaType->getMetaClass();
			if(metaClass != nullptr) {
				metaClass->doGetAccessibleList(&result);
			}
			return true;
			});
	}
	else {
		doGetAccessibleList(&result);
	}
	return result;
}

const RegisteredItem & MetaClass::getCallable(const std::string & name, const Flags flags) const
{
	if(hasFlag(flags, flagIncludeBase)) {
		const RegisteredItem * result = &internal_::emptyRegisteredItem;
		getMetaRepo()->traverseBases(classMetaType, [&result, &name](const MetaType * metaType) -> bool {
			const MetaClass * metaClass = metaType->getMetaClass();
			if(metaClass != nullptr) {
				result = &metaClass->doGetCallable(name);
				if(! result->isEmpty()) {
					return false;
				}
			}
			return true;
			});
		return *result;
	}
	else {
		return doGetCallable(name);
	}
}

RegisteredItemList MetaClass::getCallableList(const Flags flags) const
{
	RegisteredItemList result;
	if(hasFlag(flags, flagIncludeBase)) {
		getMetaRepo()->traverseBases(classMetaType, [&result](const MetaType * metaType) -> bool {
			const MetaClass * metaClass = metaType->getMetaClass();
			if(metaClass != nullptr) {
				metaClass->doGetCallableList(&result);
			}
			return true;
			});
	}
	else {
		doGetCallableList(&result);
	}
	return result;
}

const RegisteredItem & MetaClass::getType(const std::string & name, const Flags flags) const
{
	if(hasFlag(flags, flagIncludeBase)) {
		const RegisteredItem * result = &internal_::emptyRegisteredItem;
		getMetaRepo()->traverseBases(classMetaType, [&result, &name](const MetaType * metaType) -> bool {
			const MetaClass * metaClass = metaType->getMetaClass();
			if(metaClass != nullptr) {
				result = &metaClass->doGetType(name);
				if(! result->isEmpty()) {
					return false;
				}
			}
			return true;
			});
		return *result;
	}
	else {
		return doGetType(name);
	}
}

const RegisteredItem & MetaClass::getType(const TypeKind kind, const Flags flags) const
{
	if(hasFlag(flags, flagIncludeBase)) {
		const RegisteredItem * result = &internal_::emptyRegisteredItem;
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

const RegisteredItem & MetaClass::getType(const MetaType * metaType, const Flags flags) const
{
	if(hasFlag(flags, flagIncludeBase)) {
		const RegisteredItem * result = &internal_::emptyRegisteredItem;
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

RegisteredItemList MetaClass::getTypeList(const Flags flags) const
{
	if(hasFlag(flags, flagIncludeBase)) {
		RegisteredItemList result;
		getMetaRepo()->traverseBases(classMetaType, [&result](const MetaType * metaType) -> bool {
			const MetaClass * metaClass = metaType->getMetaClass();
			if(metaClass != nullptr) {
				metaClass->doGetTypeList(&result);
			}
			return true;
			});
		return result;
	}
	else {
		return doGetTypeList();
	}
}


} // namespace metapp
