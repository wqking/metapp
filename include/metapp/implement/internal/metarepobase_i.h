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

#ifndef METAPP_METAREPOBASE_I_H_969872685611
#define METAPP_METAREPOBASE_I_H_969872685611

#include "metapp/metatype.h"
#include "metapp/exception.h"
#include "metapp/implement/internal/util_i.h"
#include "metapp/registration/registeredtype.h"
#include "metapp/registration/registeredfield.h"
#include "metapp/registration/registeredmethod.h"

#include <map>
#include <deque>
#include <memory>
#include <functional>

namespace metapp {

namespace internal_ {

class MetaRepoBase
{
public:
	MetaRepoBase();

	RegisteredField & registerField(const std::string & name, const Variant & field);

	RegisteredMethod & registerMethod(const std::string & name, const Variant & method);

	template <typename T>
	RegisteredType & registerType(const std::string & name = "") {
		return registerType(name, getMetaType<T>());
	}
	RegisteredType & registerType(std::string name, const MetaType * metaType);

protected:
	void doGetFieldList(RegisteredFieldList * result) const;
	const RegisteredFieldList & doGetFieldList() const;
	const RegisteredField & doGetField(const std::string & name) const;

	const RegisteredMethod & doGetMethod(const std::string & name) const;
	void doGetMethodList(const std::string & name, RegisteredMethodList * result) const;
	void doGetMethodList(RegisteredMethodList * result) const;
	const RegisteredMethodList & doGetMethodList() const;

	const RegisteredType & doGetType(const std::string & name) const;
	const RegisteredType & doGetType(const TypeKind kind) const;
	void doGetTypeList(RegisteredTypeList * result) const;
	const RegisteredTypeList & doGetTypeList() const;

private:
	RegisteredTypeList typeList;
	std::map<
		std::reference_wrapper<const std::string>,
		const RegisteredType *,
		std::less<const std::string>
	> nameTypeMap;
	std::map<TypeKind, const RegisteredType *> kindTypeMap;

	RegisteredMethodList methodList;
	using RegisteredMethodPointerList = std::vector<RegisteredMethod *>;
	std::map<
		std::reference_wrapper<const std::string>,
		RegisteredMethodPointerList,
		std::less<const std::string>
	> methodMap;

	RegisteredFieldList fieldList;
	std::map<
		std::reference_wrapper<const std::string>,
		RegisteredField *,
		std::less<const std::string>
	> fieldMap;
};


} // namespace internal_

} // namespace metapp

#endif
