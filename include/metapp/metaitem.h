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

#ifndef METAPP_METAITEM_H_969872685611
#define METAPP_METAITEM_H_969872685611

#include "metapp/variant.h"

namespace metapp {

class MetaRepo;

class MetaItem
{
public:
	enum class Type {
		none,
		accessible,
		callable,
		variable,
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
	const Variant & asVariable() const;
	const Variant & asConstructor() const;
	const MetaType * asMetaType() const;
	const MetaRepo * asMetaRepo() const;
	const Variant & asEnumValue() const;

	const Variant & getTarget() const {
		return doGetVariant();
	}

	operator const Variant & () const {
		return doGetVariant();
	}

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


} // namespace metapp


#endif
