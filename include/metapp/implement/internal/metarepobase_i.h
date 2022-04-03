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
private:
	using NamedMethodList = std::vector<RegisteredMethod>;

public:
	using VariantList = std::vector<std::reference_wrapper<const Variant> >;
	using NameList = std::vector<std::reference_wrapper<const std::string> >;

public:
	MetaRepoBase();

	void registerType(const MetaType * metaType, std::string name = "");

	template <typename T>
	void registerType(const std::string & name = "") {
		registerType(getMetaType<T>(), name);
	}

	const MetaType * getTypeByName(const std::string & name) const;
	const MetaType * getTypeByKind(const TypeKind kind) const;
	std::string getNameByKind(const TypeKind kind) const;

	std::vector<std::string> getTypeNameList() const;

	RegisteredMethod & registerMethod(const std::string & name, const Variant & method);
	RegisteredField & registerField(const std::string & name, const Variant & field);

protected:
	void doGetFieldList(RegisteredFieldList * result) const;
	const RegisteredField & doGetField(const std::string & name) const;

	const RegisteredMethod & doGetMethod(const std::string & name) const;
	void doGetMethodList(const std::string & name, RegisteredMethodList * result) const;
	void doGetMethodList(RegisteredMethodList * result) const;

private:
	std::map<std::string, const MetaType *> nameTypeMap;
	std::map<TypeKind, std::pair<std::string, const MetaType *> > kindTypeMap;

	std::map<
		std::reference_wrapper<const std::string>,
		NamedMethodList,
		std::less<const std::string>
	> methodMap;

	std::map<
		std::reference_wrapper<const std::string>,
		RegisteredField,
		std::less<const std::string>
	> fieldMap;
};


} // namespace internal_

} // namespace metapp

#endif
