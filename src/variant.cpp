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

#include "metapp/variant.h"
#include "metapp/utilities/utility.h"
#include "metapp/interfaces/metastreamable.h"
#include "metapp/interfaces/metaaccessible.h"
#include "metapp/allmetatypes.h"

namespace metapp {

Variant Variant::retype(const MetaType * metaType, const Variant & var)
{
	return Variant(metaType, var.data);
}

Variant Variant::takeFrom(const MetaType * metaType, void * instance)
{
	return Variant(metaType, VariantData(std::shared_ptr<void>(instance, [metaType](void * p) {
			metaType->destroy(p);
		}),
		VariantData::StorageTagObject()
	));
}

Variant Variant::takeFrom(const Variant & var)
{
	const MetaType * metaType = var.getMetaType();
	if(metaType->isPointer()) {
		metaType = metaType->getUpType();
	}
	return takeFrom(metaType, var.get<void *>());
}

bool Variant::canGet(const MetaType * toMetaType_) const
{
	const MetaType * fromMetaType = metaType;
	if(fromMetaType->isReference() && toMetaType_->isReference()) {
		return true;
	}
	fromMetaType = getNonReferenceMetaType(fromMetaType);
	const MetaType * toMetaType = getNonReferenceMetaType(toMetaType_);
	if(toMetaType->getTypeKind() == tkVariant) {
		return true;
	}
	if(fromMetaType->getTypeKind() == tkVariant) {
		return get<const Variant &>().canGet(toMetaType_);
	}
	if(fromMetaType->isPointer() && toMetaType->isPointer()) {
		return true;
	}
	if(fromMetaType->isArray() && toMetaType->isArray()) {
		return true;
	}
	return fromMetaType->equal(toMetaType);
}

bool Variant::canCast(const MetaType * toMetaType) const
{
	if(getNonReferenceMetaType(toMetaType)->equal(getNonReferenceMetaType(metaType))) {
		return true;
	}
	return metaType->cast(nullptr, this, toMetaType);
}

Variant Variant::cast(const MetaType * toMetaType) const
{
	if(getNonReferenceMetaType(toMetaType)->equal(getNonReferenceMetaType(metaType))) {
		return *this;
	}
	Variant result;
	if(! metaType->cast(&result, this, toMetaType)) {
		raiseException<BadCastException>();
	}
	return result;
}

Variant Variant::castSilently(const MetaType * toMetaType) const
{
	if(getNonReferenceMetaType(toMetaType)->equal(getNonReferenceMetaType(metaType))) {
		return *this;
	}
	Variant result;
	metaType->cast(&result, this, toMetaType);
	return result;
}

Variant Variant::clone() const
{
	return Variant(metaType, metaType->constructVariantData(getAddress(), CopyStrategy::copy));
}

Variant & Variant::assign(const Variant & other)
{
	const MetaType * mt = getNonReferenceMetaType(metaType);
	if(mt->getTypeKind() == tkVariant) {
		void * myAddress = getAddress();
		mt->dtor(myAddress);
		mt->placementCopyConstruct(myAddress, &other);
	}
	else {
		const Variant casted = other.cast(metaType);
		void * myAddress = getAddress();
		mt->dtor(myAddress);
		mt->placementCopyConstruct(myAddress, casted.getAddress());
	}
	return *this;
}

std::istream & operator >> (std::istream & stream, Variant & value)
{
	auto metaStreamable = getNonReferenceMetaType(value.metaType)->getMetaStreamable();
	if(metaStreamable == nullptr) {
		raiseException<UnsupportedException>("No >> input streaming operator.");
		return stream;
	}
	metaStreamable->streamIn(stream, value);
	return stream;
}

std::ostream & operator << (std::ostream & stream, const Variant & value)
{
	auto metaStreamable = getNonReferenceMetaType(value.metaType)->getMetaStreamable();
	if(metaStreamable == nullptr) {
		raiseException<UnsupportedException>("No << output streaming operator.");
		return stream;
	}
	metaStreamable->streamOut(stream, value);
	return stream;
}


} // namespace metapp
