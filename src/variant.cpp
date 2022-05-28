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
#include "metapp/interfaces/metastreaming.h"
#include "metapp/interfaces/metaaccessible.h"

namespace metapp {

void * VariantData::getAddress() const
{
	switch(getStorageType()) {
	case storageObject:
		return (void *)(object.get());

	case storageBuffer:
		return (void *)(buffer.data());

	case storageSharedPtr:
		return (void *)&object;

	case storageReference:
		return *(void **)(buffer.data());
	}

	return nullptr;
}


Variant Variant::retype(const MetaType * metaType, const Variant & var)
{
	Variant result;
	result.metaType = metaType;
	result.data = var.data;
	return result;
}

Variant Variant::takeFrom(const MetaType * metaType, void * instance)
{
	Variant result;

	result.metaType = metaType;
	result.data.constructObject(std::shared_ptr<void>(instance, [metaType](void * p) {
		metaType->destroy(p);
	}));

	return result;
}

Variant Variant::takeFrom(const Variant & var)
{
	const MetaType * metaType = var.getMetaType();
	if(metaType->isPointer()) {
		metaType = metaType->getUpType();
	}
	return takeFrom(metaType, var.get<void *>());
}

Variant::Variant() noexcept
	: 
		metaType(metapp::getMetaType<void>()),
		data()
{
}

Variant::Variant(const MetaType * metaType, const void * copyFrom)
	:
		metaType(metaType)
{
	metaType->constructData(&data, copyFrom, nullptr);
}

Variant::Variant(const Variant & other) noexcept
	:
		metaType(other.metaType),
		data(other.data)
{
}

Variant::Variant(Variant && other) noexcept
	:
		metaType(std::move(other.metaType)),
		data(std::move(other.data))
{
}

Variant & Variant::operator = (const Variant & other) noexcept
{
	if(this != &other) {
		metaType = other.metaType;
		data = other.data;
	}

	return *this;
}

Variant & Variant::operator = (Variant && other) noexcept
{
	if(this != &other) {
		metaType = std::move(other.metaType);
		data = std::move(other.data);
	}

	return *this;
}

bool Variant::canGet(const MetaType * toMetaType) const
{
	const MetaType * fromMetaType = metaType;
	if(fromMetaType->isReference() && toMetaType->isReference()) {
		return true;
	}
	fromMetaType = getNonReferenceMetaType(fromMetaType);
	toMetaType = getNonReferenceMetaType(toMetaType);
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
	return metaType->cast(nullptr, *this, toMetaType);
}

Variant Variant::cast(const MetaType * toMetaType) const
{
	Variant result;
	if(! metaType->cast(&result, *this, toMetaType)) {
		raiseException<BadCastException>();
	}
	return result;
}

Variant Variant::castSilently(const MetaType * toMetaType) const
{
	Variant result;
	metaType->cast(&result, *this, toMetaType);
	return result;
}

bool Variant::isEmpty() const noexcept
{
	return metaType->isVoid();
}

Variant Variant::clone() const
{
	Variant result;
	result.metaType = metaType;
	result.metaType->constructData(&result.data, getAddress(), nullptr);
	return result;
}

Variant & Variant::assign(const Variant & other)
{
	Variant casted = other.cast(metaType);
	void * myAddress = getAddress();
	const MetaType * mt = getNonReferenceMetaType(metaType);
	mt->dtor(myAddress);
	mt->placementCopyConstruct(casted.getAddress(), myAddress);
	return *this;
}

void Variant::swap(Variant & other) noexcept
{
	using std::swap;

	swap(metaType, other.metaType);
	swap(data, other.data);
}

std::istream & operator >> (std::istream & stream, Variant & value)
{
	auto metaStreaming = getNonReferenceMetaType(value.metaType)->getMetaStreaming();
	if(metaStreaming == nullptr) {
		raiseException<UnsupportedException>("No >> input streaming operator.");
		return stream;
	}
	metaStreaming->streamIn(stream, value);
	return stream;
}

std::ostream & operator << (std::ostream & stream, const Variant & value)
{
	auto metaStreaming = getNonReferenceMetaType(value.metaType)->getMetaStreaming();
	if(metaStreaming == nullptr) {
		raiseException<UnsupportedException>("No << output streaming operator.");
		return stream;
	}
	metaStreaming->streamOut(stream, value);
	return stream;
}

void swap(Variant & a, Variant & b) noexcept
{
	a.swap(b);
}

TypeKind getTypeKind(const Variant & v)
{
	return v.getMetaType()->getTypeKind();
}


} // namespace metapp
