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
#include "metapp/implement/internal/disjointview_i.h"

#include <map>
#include <deque>
#include <memory>
#include <functional>

namespace metapp {

class MetaItem
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
	MetaItem();
	MetaItem(const Type type, const std::string & name, const Variant & target);
	~MetaItem();

	bool isEmpty() const {
		return ! data;
	}

	const std::string & getName() const;
	Type getType() const;

	const Variant & asAccessible() const;
	const Variant & asCallable() const;
	const Variant & asConstructor() const;
	const MetaType * asMetaType() const;
	const MetaRepo * asMetaRepo() const;
	const Variant & asEnumValue() const;

	operator const Variant & () const;

	void registerAnnotation(const std::string & name, const Variant & value);
	const Variant & getAnnotation(const std::string & name) const;
	const std::map<std::string, Variant> & getAllAnnotations() const;

	void setTarget(const Variant & target);

private:
	const Variant & doGetVariant() const;
	void doCheckType(const Type type) const;

private:
	std::shared_ptr<Data> data;
	std::shared_ptr<std::map<std::string, Variant> > annotationMap;
};

using MetaItemList = std::deque<MetaItem>;

using MetaItemView = internal_::DisjointView<MetaItem, std::deque<MetaItem>, 4>;

namespace internal_ {

class MetaRepoBase
{
public:
	MetaRepoBase();

	MetaItem & registerAccessible(const std::string & name, const Variant & accessible);

	MetaItem & registerCallable(const std::string & name, const Variant & callable);

	template <typename T>
	MetaItem & registerType(const std::string & name = "") {
		return registerType(name, getMetaType<T>());
	}
	MetaItem & registerType(std::string name, const MetaType * metaType);

protected:
	struct ItemData
	{
		MetaItemList itemList;
		std::map<
			std::reference_wrapper<const std::string>,
			MetaItem *,
			std::less<const std::string>
		> nameItemMap;

		MetaItem & addItem(const MetaItem::Type type, const std::string & name, const Variant & target);
		const MetaItem & findItem(const std::string & name) const;
	};
	template <typename T>
	static bool doFindItemByName(const std::shared_ptr<T> & data, const std::string & name, const MetaItem * & result)
	{
		if(data) {
			auto it = data->nameItemMap.find(name);
			if(it != data->nameItemMap.end()) {
				result = it->second;
				return true;
			}
		}
		return false;
	}

	const MetaItem & doGetAccessible(const std::string & name) const;
	const MetaItemList & doGetAccessibleList() const;

	const MetaItem & doGetCallable(const std::string & name) const;
	const MetaItemList & doGetCallableList() const;

	const MetaItem & doGetType(const std::string & name) const;
	const MetaItem & doGetType(const TypeKind kind) const;
	const MetaItem & doGetType(const MetaType * metaType) const;
	const MetaItemList & doGetTypeList() const;
	
	const MetaItem & doGetItem(const std::string & name) const;

private:
	struct TypeData : ItemData
	{
		std::map<TypeKind, MetaItem *> kindTypeMap;
		std::map<const MetaType *, MetaItem *> typeTypeMap;
	};
	std::shared_ptr<TypeData> typeData;

	std::shared_ptr<ItemData> callableData;
	std::shared_ptr<ItemData> accessibleData;
};

Variant doCombineOverloadedCallable(const Variant & target, const Variant & callable);


} // namespace internal_

} // namespace metapp

#endif
