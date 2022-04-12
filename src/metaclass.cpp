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

extern RegisteredField emptyRegisteredField;
extern RegisteredMethod emptyRegisteredMethod;
extern RegisteredType emptyRegisteredType;

} // namespace internal_

RegisteredConstructor & MetaClass::registerConstructor(const Variant & constructor)
{
	if(constructor.getMetaType()->getMetaCallable() == nullptr) {
		errorWrongMetaType();
	}
	constructorList.push_back(RegisteredConstructor(constructor));
	return constructorList.back();
}

const RegisteredConstructorList & MetaClass::getConstructorList() const
{
	return constructorList;
}

const RegisteredField & MetaClass::getField(const std::string & name, const Flags flags) const
{
	if(hasFlag(flags, flagIncludeBase)) {
		const RegisteredField * result = &internal_::emptyRegisteredField;
		getMetaRepo()->traverseBases(classMetaType, [&result, &name](const MetaType * metaType) -> bool {
			const MetaClass * metaClass = metaType->getMetaClass();
			if(metaClass != nullptr) {
				result = &metaClass->doGetField(name);
				if(! result->isEmpty()) {
					return false;
				}
			}
			return true;
			});
		return *result;
	}
	else {
		return doGetField(name);
	}
}

RegisteredFieldList MetaClass::getFieldList(const Flags flags) const
{
	RegisteredFieldList result;
	if(hasFlag(flags, flagIncludeBase)) {
		getMetaRepo()->traverseBases(classMetaType, [&result](const MetaType * metaType) -> bool {
			const MetaClass * metaClass = metaType->getMetaClass();
			if(metaClass != nullptr) {
				metaClass->doGetFieldList(&result);
			}
			return true;
			});
	}
	else {
		doGetFieldList(&result);
	}
	return result;
}

const RegisteredMethod & MetaClass::getMethod(const std::string & name, const Flags flags) const
{
	if(hasFlag(flags, flagIncludeBase)) {
		const RegisteredMethod * result = &internal_::emptyRegisteredMethod;
		getMetaRepo()->traverseBases(classMetaType, [&result, &name](const MetaType * metaType) -> bool {
			const MetaClass * metaClass = metaType->getMetaClass();
			if(metaClass != nullptr) {
				result = &metaClass->doGetMethod(name);
				if(! result->isEmpty()) {
					return false;
				}
			}
			return true;
			});
		return *result;
	}
	else {
		return doGetMethod(name);
	}
}

RegisteredMethodList MetaClass::getMethodList(const std::string & name, const Flags flags) const
{
	RegisteredMethodList result;
	if(hasFlag(flags, flagIncludeBase)) {
		getMetaRepo()->traverseBases(classMetaType, [&result, &name](const MetaType * metaType) -> bool {
			const MetaClass * metaClass = metaType->getMetaClass();
			if(metaClass != nullptr) {
				metaClass->doGetMethodList(name, &result);
			}
			return true;
			});
	}
	else {
		doGetMethodList(name, &result);
	}
	return result;
}

RegisteredMethodList MetaClass::getMethodList(const Flags flags) const
{
	RegisteredMethodList result;
	if(hasFlag(flags, flagIncludeBase)) {
		getMetaRepo()->traverseBases(classMetaType, [&result](const MetaType * metaType) -> bool {
			const MetaClass * metaClass = metaType->getMetaClass();
			if(metaClass != nullptr) {
				metaClass->doGetMethodList(&result);
			}
			return true;
			});
	}
	else {
		doGetMethodList(&result);
	}
	return result;
}

const RegisteredType & MetaClass::getType(const std::string & name, const Flags flags) const
{
	if(hasFlag(flags, flagIncludeBase)) {
		const RegisteredType * result = &internal_::emptyRegisteredType;
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

const RegisteredType & MetaClass::getType(const TypeKind kind, const Flags flags) const
{
	if(hasFlag(flags, flagIncludeBase)) {
		const RegisteredType * result = &internal_::emptyRegisteredType;
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

const RegisteredType & MetaClass::getType(const MetaType * metaType, const Flags flags) const
{
	if(hasFlag(flags, flagIncludeBase)) {
		const RegisteredType * result = &internal_::emptyRegisteredType;
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

RegisteredTypeList MetaClass::getTypeList(const Flags flags) const
{
	if(hasFlag(flags, flagIncludeBase)) {
		RegisteredTypeList result;
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
