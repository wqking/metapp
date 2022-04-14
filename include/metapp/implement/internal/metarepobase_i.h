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
#include "metapp/registration/registeredaccessible.h"
#include "metapp/registration/registeredcallable.h"

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

	RegisteredAccessible & registerAccessible(const std::string & name, const Variant & accessible);

	RegisteredCallable & registerCallable(const std::string & name, const Variant & callable);

	template <typename T>
	RegisteredType & registerType(const std::string & name = "") {
		return registerType(name, getMetaType<T>());
	}
	RegisteredType & registerType(std::string name, const MetaType * metaType);

protected:
	void doGetAccessibleList(RegisteredAccessibleList * result) const;
	const RegisteredAccessibleList & doGetAccessibleList() const;
	const RegisteredAccessible & doGetAccessible(const std::string & name) const;

	const RegisteredCallable & doGetCallable(const std::string & name) const;
	void doGetCallableList(const std::string & name, RegisteredCallableList * result) const;
	void doGetCallableList(RegisteredCallableList * result) const;
	const RegisteredCallableList & doGetCallableList() const;

	const RegisteredType & doGetType(const std::string & name) const;
	const RegisteredType & doGetType(const TypeKind kind) const;
	const RegisteredType & doGetType(const MetaType * metaType) const;
	void doGetTypeList(RegisteredTypeList * result) const;
	const RegisteredTypeList & doGetTypeList() const;

private:
	struct TypeData
	{
		RegisteredTypeList typeList;
		std::map<
			std::reference_wrapper<const std::string>,
			RegisteredType *,
			std::less<const std::string>
		> nameTypeMap;
		std::map<TypeKind, RegisteredType *> kindTypeMap;
		std::map<const MetaType *, RegisteredType *> typeTypeMap;
	};
	std::shared_ptr<TypeData> typeData;

	struct CallableData
	{
		RegisteredCallableList callableList;
		using RegisteredCallablePointerList = std::vector<RegisteredCallable *>;
		std::map<
			std::reference_wrapper<const std::string>,
			RegisteredCallablePointerList,
			std::less<const std::string>
		> callableMap;
	};
	std::shared_ptr<CallableData> callableData;

	struct AccessibleData
	{
		RegisteredAccessibleList accessibleList;
		std::map<
			std::reference_wrapper<const std::string>,
			RegisteredAccessible *,
			std::less<const std::string>
		> accessibleMap;
	};
	std::shared_ptr<AccessibleData> accessibleData;
};


} // namespace internal_

} // namespace metapp

#endif
