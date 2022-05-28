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

#include "metapp/metaitem.h"
#include "metapp/metatype.h"
#include "metapp/metarepo.h"
#include "metapp/allmetatypes.h"

namespace metapp {

namespace internal_ {

std::map<std::string, Variant> emptyAnnotationMap;

extern Variant emptyVariant;
extern std::string emptyString;

} // namespace internal_

MetaItem::MetaItem()
	: data()
{
}

MetaItem::MetaItem(const Type type, const std::string & name, const Variant & target)
	: data(std::make_shared<Data>(type, name, target))
{
}

MetaItem::~MetaItem()
{
}

const std::string & MetaItem::getName() const
{
	return data ? data->name : internal_::emptyString;
}

MetaItem::Type MetaItem::getType() const
{
	return data ? data->type : MetaItem::Type::none;
}

const Variant & MetaItem::asAccessible() const
{
	doCheckType(Type::accessible);
	return doGetVariant();
}

const Variant & MetaItem::asCallable() const
{
	doCheckType(Type::callable);
	return doGetVariant();
}

const Variant & MetaItem::asConstant() const
{
	doCheckType(Type::constant);
	return doGetVariant();
}

const Variant & MetaItem::asConstructor() const
{
	doCheckType(Type::constructor);
	return doGetVariant();
}

const MetaType * MetaItem::asMetaType() const
{
	doCheckType(Type::metaType);
	return doGetVariant().get<const MetaType *>();
}

const MetaRepo * MetaItem::asMetaRepo() const
{
	doCheckType(Type::metaRepo);
	return doGetVariant().get<const MetaRepo *>();
}

const Variant & MetaItem::asEnumValue() const
{
	doCheckType(Type::enumValue);
	return doGetVariant();
}

void MetaItem::registerAnnotation(const std::string & name, const Variant & value)
{
	if(! annotationMap) {
		annotationMap = std::make_shared<std::map<std::string, Variant> >();
	}
	annotationMap->insert(std::make_pair(name, value));
}

const Variant & MetaItem::getAnnotation(const std::string & name) const
{
	if(annotationMap) {
		auto it = annotationMap->find(name);
		if(it != annotationMap->end()) {
			return it->second;
		}
	}
	return internal_::emptyVariant;
}

const std::map<std::string, Variant> & MetaItem::getAllAnnotations() const
{
	if(annotationMap) {
		return *annotationMap;
	}
	return internal_::emptyAnnotationMap;
}

void MetaItem::setTarget(const Variant & target)
{
	if(data) {
		data->target = target;
	}
}

const Variant & MetaItem::doGetVariant() const
{
	return data ? data->target : internal_::emptyVariant;
}

void MetaItem::doCheckType(const Type type) const
{
	if(getType() != type) {
		raiseException<WrongMetaTypeException>();
	}
}


} // namespace metapp
