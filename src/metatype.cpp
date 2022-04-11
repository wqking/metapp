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
#include "metapp/metarepo.h"
#include "metapp/variant.h"
#include "metapp/utilities/utility.h"

namespace metapp {

namespace internal_ {

enum class TristateBool
{
	yes,
	no,
	unknown
};

TristateBool doCastObject(
	Variant * result,
	const Variant & value,
	const MetaType * fromMetaType,
	const MetaType * toMetaType
)
{
	const MetaType * fromUpType = fromMetaType;
	const MetaType * toUpType = toMetaType;
	if(toMetaType->isReference()) {
		toUpType = toMetaType->getUpType();
		if(fromMetaType->isReference()) {
			fromUpType = fromMetaType->getUpType();
		}
	}
	else if(toMetaType->isPointer() && fromMetaType->isPointer()) {
		toUpType = toMetaType->getUpType();
		fromUpType = fromMetaType->getUpType();
	}
	else {
		return TristateBool::unknown;
	}
	if(fromUpType->isClass() && toUpType->isClass()) {
		const MetaRepo * metaRepo = getMetaRepo();
		if(metaRepo->isClassInHierarchy(fromUpType) && metaRepo->isClassInHierarchy(toUpType)) {
			if(metaRepo->getRelationship(fromUpType, toUpType) != InheritanceRelationship::none) {
				if(result != nullptr) {
					void * instance = nullptr;
					if(fromMetaType->isPointer()) {
						instance = value.get<void *>();
					}
					else {
						instance = value.getAddress();
					}
					instance = metaRepo->cast(instance, fromUpType, toUpType);
					if(toMetaType->isReference()) {
						Variant temp = Variant::create<int &>(*(int *)instance);
						*result = Variant::retype(toMetaType, temp);
					}
					else {
						Variant temp = Variant::create<void *>(instance);
						*result = Variant::retype(toMetaType, temp);
					}
				}
				return TristateBool::yes;
			}
		}
	}
	return TristateBool::unknown;
}

TristateBool doCastPointerReference(
	Variant * result,
	const Variant & value,
	const MetaType * fromMetaType,
	const MetaType * toMetaType
)
{
	if((fromMetaType->isReference() && toMetaType->isReference())
		|| (fromMetaType->isPointer() && toMetaType->isPointer())) {
		const MetaType * fromUpType = fromMetaType->getUpType();
		const MetaType * toUpType = toMetaType->getUpType();
		bool matched = false;
		while(fromUpType != nullptr && toUpType != nullptr) {
			if(fromUpType->getUnifiedType() == toUpType->getUnifiedType()) {
				matched = true;
				break;
			}
			if(fromUpType->isPointer() && toUpType->isPointer()) {
				fromUpType = fromUpType->getUpType();
				toUpType = toUpType->getUpType();
				continue;
			}
			break;
		}
		if(matched) {
			if(result != nullptr) {
				*result = Variant::retype(toMetaType, value);
			}
			return TristateBool::yes;
		}
	}

	// This must be after the previous reference/pointer checking,
	// because retype is prefered to cast when the up type is the same
	if(fromMetaType->isReference() && toMetaType->isReference()) {
		if(fromMetaType->getUpType()->cast(result, value, toMetaType->getUpType())) {
			return TristateBool::yes;
		}
	}

	const TristateBool tristateResult = doCastObject(result, value, fromMetaType, toMetaType);
	if(tristateResult != TristateBool::unknown) {
		return tristateResult;
	}

	if(! fromMetaType->isReference() && toMetaType->isReference()
		) {
		if(fromMetaType->cast(result, value, toMetaType->getUpType())) {
			return TristateBool::yes;
		}
	}
	if(fromMetaType->isReference() && ! toMetaType->isReference()
		) {
		if(fromMetaType->getUpType()->cast(result, value, toMetaType)) {
			return TristateBool::yes;
		}
	}

	if(getNonReferenceMetaType(fromMetaType)->getUnifiedType() == getNonReferenceMetaType(toMetaType)->getUnifiedType()) {
		if(result != nullptr) {
			*result = Variant::retype(toMetaType, value);
		}
		return TristateBool::yes;
	}

	return TristateBool::unknown;
}

bool doCast(
	Variant * result,
	const Variant & value,
	const MetaType * fromMetaType,
	const MetaType * toMetaType
)
{
	const internal_::TristateBool tristate = internal_::doCastPointerReference(result, value, fromMetaType, toMetaType);
	if(tristate != internal_::TristateBool::unknown) {
		return tristate == internal_::TristateBool::yes;
	}

	if(toMetaType->castFrom(result, value, fromMetaType)) {
		return true;
	}

	return false;
}


TypeKind UnifiedType::getTypeKind() const noexcept
{
	return typeKind;
}

const MetaClass * UnifiedType::getMetaClass() const
{
	return static_cast<const MetaClass *>(doGetMetaInterface(internal_::mikMetaClass));
}

const MetaCallable * UnifiedType::getMetaCallable() const
{
	return static_cast<const MetaCallable *>(doGetMetaInterface(internal_::mikMetaCallable));
}

const MetaAccessible * UnifiedType::getMetaAccessible() const
{
	return static_cast<const MetaAccessible *>(doGetMetaInterface(internal_::mikMetaAccessible));
}

const MetaEnum * UnifiedType::getMetaEnum() const
{
	return static_cast<const MetaEnum *>(doGetMetaInterface(internal_::mikMetaEnum));
}

const MetaIndexable * UnifiedType::getMetaIndexable() const
{
	return static_cast<const MetaIndexable *>(doGetMetaInterface(internal_::mikMetaIndexable));
}

const MetaIterable * UnifiedType::getMetaIterable() const
{
	return static_cast<const MetaIterable *>(doGetMetaInterface(internal_::mikMetaIterable));
}

const MetaStreaming * UnifiedType::getMetaStreaming() const
{
	return static_cast<const MetaStreaming *>(doGetMetaInterface(internal_::mikMetaStreaming));
}

const MetaMap * UnifiedType::getMetaMap() const
{
	return static_cast<const MetaMap *>(doGetMetaInterface(internal_::mikMetaMap));
}

const MetaMember * UnifiedType::getMetaMember() const
{
	return static_cast<const MetaMember *>(doGetMetaInterface(internal_::mikMetaMember));
}

const void * UnifiedType::getMetaUser() const
{
	return static_cast<const void *>(doGetMetaInterface(internal_::mikMetaUser));
}

const void * UnifiedType::doGetMetaInterface(const internal_::MetaInterfaceKind kind) const
{
	if((kind & metaMethodTable.metaInterfaceData.kinds) != 0) {
		if(metaMethodTable.metaInterfaceData.items[0].kind == kind) {
			return metaMethodTable.metaInterfaceData.items[0].getter();
		}
		const uint32_t count = (metaMethodTable.metaInterfaceData.kinds & metaInterfaceCountMask);
		if(count > 1) {
			for(uint16_t i = 1; i < count; ++i) {
				if(metaMethodTable.metaInterfaceData.items[i].kind == kind) {
					return metaMethodTable.metaInterfaceData.items[i].getter();
				}
			}
		}
	}
	return nullptr;
}

void * UnifiedType::constructData(MetaTypeData * data, const void * copyFrom) const
{
	return metaMethodTable.constructData(data, copyFrom);
}

void UnifiedType::destroy(void * instance) const
{
	metaMethodTable.destroy(instance);
}

bool UnifiedType::cast(Variant * result, const Variant & value, const MetaType * toMetaType) const
{
	return metaMethodTable.cast(result, value, toMetaType);
}

bool UnifiedType::castFrom(Variant * result, const Variant & value, const MetaType * fromMetaType) const
{
	if(metaMethodTable.castFrom == nullptr) {
		return false;
	}
	return metaMethodTable.castFrom(result, value, fromMetaType);
}


} // namespace internal_


void * DeclareMetaTypeVoidBase::constructData(MetaTypeData * /*data*/, const void * /*value*/)
{
	return nullptr;
}

void DeclareMetaTypeVoidBase::destroy(void * /*instance*/)
{
}

void DeclareMetaTypeVoidBase::toReference(Variant * result, const Variant & value)
{
	*result = value;
}

bool DeclareMetaTypeVoidBase::cast(Variant * /*result*/, const Variant & /*value*/, const MetaType * /*toMetaType*/)
{
	return false;
}

bool DeclareMetaTypeVoidBase::castFrom(Variant * /*result*/, const Variant & /*value*/, const MetaType * /*fromMetaType*/)
{
	return false;
}


MetaType::MetaType(
	const internal_::UnifiedType * (*doGetUnifiedType)(),
	const internal_::MetaTable & metaTable,
	const internal_::UpTypeData & upTypeData,
	const TypeFlags typeFlags
) noexcept :
	doGetUnifiedType(doGetUnifiedType),
#ifdef METAPP_DEBUG_ENABLED
	debugUnifiedType(doGetUnifiedType()),
#endif
	metaTable(metaTable),
	upTypeData(upTypeData),
	typeFlags(typeFlags)
{
}

const void * MetaType::getUnifiedType() const noexcept
{
	return doGetUnifiedType();
}

const MetaType * MetaType::getUpType() const
{
	return upTypeData.upTypeList[1];
}

const MetaType * MetaType::getUpType(const size_t i) const
{
	return upTypeData.upTypeList[i + 1];
}

size_t MetaType::getUpTypeCount() const noexcept
{
	if(upTypeData.upTypeList == nullptr) {
		return 0;
	}
	return (size_t)upTypeData.upTypeList[0];
}

TypeKind MetaType::getTypeKind() const noexcept
{
	return doGetUnifiedType()->getTypeKind();
}

const MetaClass * MetaType::getMetaClass() const
{
	return doGetUnifiedType()->getMetaClass();
}

const MetaCallable * MetaType::getMetaCallable() const
{
	return doGetUnifiedType()->getMetaCallable();
}

const MetaAccessible * MetaType::getMetaAccessible() const
{
	return doGetUnifiedType()->getMetaAccessible();
}

const MetaEnum * MetaType::getMetaEnum() const
{
	return doGetUnifiedType()->getMetaEnum();
}

const MetaIndexable * MetaType::getMetaIndexable() const
{
	return doGetUnifiedType()->getMetaIndexable();
}

const MetaIterable * MetaType::getMetaIterable() const
{
	return doGetUnifiedType()->getMetaIterable();
}

const MetaStreaming * MetaType::getMetaStreaming() const
{
	return doGetUnifiedType()->getMetaStreaming();
}

const MetaMap * MetaType::getMetaMap() const
{
	return doGetUnifiedType()->getMetaMap();
}

const MetaMember * MetaType::getMetaMember() const
{
	return doGetUnifiedType()->getMetaMember();
}

const void * MetaType::getMetaUser() const
{
	return doGetUnifiedType()->getMetaUser();
}

void * MetaType::construct() const
{
	return constructData(nullptr, nullptr);
}

void * MetaType::copyConstruct(const void * copyFrom) const
{
	return constructData(nullptr, copyFrom);
}

void * MetaType::constructData(MetaTypeData * data, const void * copyFrom) const
{
	return doGetUnifiedType()->constructData(data, copyFrom);
}

void MetaType::destroy(void * instance) const
{
	doGetUnifiedType()->destroy(instance);
}

void MetaType::toReference(Variant * result, const Variant & value) const
{
	metaTable.toReference(result, value);
}

bool MetaType::cast(Variant * result, const Variant & value, const MetaType * toMetaType) const
{
	return doGetUnifiedType()->cast(result, value, toMetaType);
}

bool MetaType::castFrom(Variant * result, const Variant & value, const MetaType * fromMetaType) const
{
	return doGetUnifiedType()->castFrom(result, value, fromMetaType);
}


} // namespace metapp
