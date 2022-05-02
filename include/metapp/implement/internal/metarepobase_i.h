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

#include <map>
#include <deque>
#include <memory>
#include <functional>

namespace metapp {

class RegisteredItem
{
public:
	enum class Type {
		none,
		accessible,
		callable,
		constructor,
		metaRepo,
		metaType,
		enumValue
	};

private:
	struct Data
	{
		Data(const Type type, const std::string & name, const Variant & target)
			: type(type), name(name), target(target)
		{
		}

		Type type;
		std::string name;
		Variant target;
	};

public:
	RegisteredItem()
		: data()
	{
	}

	RegisteredItem(const Type type, const std::string & name, const Variant & target)
		: data(std::make_shared<Data>(type, name, target))
	{
	}

	bool isEmpty() const {
		return ! data;
	}

	const std::string & getName() const;
	const Variant & getTarget() const;
	Type getType() const;

	operator const Variant & () const;

	void registerAnnotation(const std::string & name, const Variant & value);
	const Variant & getAnnotation(const std::string & name) const;
	const std::map<std::string, Variant> & getAllAnnotations() const;

private:
	std::shared_ptr<Data> data;
	std::shared_ptr<std::map<std::string, Variant> > annotationMap;
};

using RegisteredItemList = std::deque<RegisteredItem>;

namespace internal_ {

class MetaRepoBase
{
public:
	MetaRepoBase();

	RegisteredItem & registerAccessible(const std::string & name, const Variant & accessible);

	RegisteredItem & registerCallable(const std::string & name, const Variant & callable);

	template <typename T>
	RegisteredItem & registerType(const std::string & name = "") {
		return registerType(name, getMetaType<T>());
	}
	RegisteredItem & registerType(std::string name, const MetaType * metaType);

protected:
	void doGetAccessibleList(RegisteredItemList * result) const;
	const RegisteredItemList & doGetAccessibleList() const;
	const RegisteredItem & doGetAccessible(const std::string & name) const;

	const RegisteredItem & doGetCallable(const std::string & name) const;
	void doGetCallableList(RegisteredItemList * result) const;
	const RegisteredItemList & doGetCallableList() const;

	const RegisteredItem & doGetType(const std::string & name) const;
	const RegisteredItem & doGetType(const TypeKind kind) const;
	const RegisteredItem & doGetType(const MetaType * metaType) const;
	void doGetTypeList(RegisteredItemList * result) const;
	const RegisteredItemList & doGetTypeList() const;

private:
	struct TypeData
	{
		RegisteredItemList typeList;
		std::map<
			std::reference_wrapper<const std::string>,
			RegisteredItem *,
			std::less<const std::string>
		> nameTypeMap;
		std::map<TypeKind, RegisteredItem *> kindTypeMap;
		std::map<const MetaType *, RegisteredItem *> typeTypeMap;
	};
	std::shared_ptr<TypeData> typeData;

	struct CallableData
	{
		RegisteredItemList callableList;
		std::map<
			std::reference_wrapper<const std::string>,
			RegisteredItem *,
			std::less<const std::string>
		> callableMap;
	};
	std::shared_ptr<CallableData> callableData;

	struct AccessibleData
	{
		RegisteredItemList accessibleList;
		std::map<
			std::reference_wrapper<const std::string>,
			RegisteredItem *,
			std::less<const std::string>
		> accessibleMap;
	};
	std::shared_ptr<AccessibleData> accessibleData;
};


} // namespace internal_

} // namespace metapp

#endif
