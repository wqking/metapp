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
		const MetaRepo * metaRepo = getMetaRepoList()->findMetaRepo(fromUpType);
		if(metaRepo != nullptr && metaRepo->isClassInHierarchy(toUpType)) {
			if(metaRepo->getRelationship(fromUpType, toUpType) != MetaRepo::Relationship::none) {
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
			if(fromUpType->equal(toUpType)) {
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

	if(getNonReferenceMetaType(fromMetaType)->equal(getNonReferenceMetaType(toMetaType))) {
		if(result != nullptr) {
			*result = Variant::retype(toMetaType, value);
		}
		return TristateBool::yes;
	}

	return TristateBool::unknown;
}

const void * UnifiedType::getMetaInterface(const MetaInterfaceKind kind) const
{
	if((kind & metaMethodTable.metaInterfaceData.kinds) != 0) {
		if(metaMethodTable.metaInterfaceData.items[0].kind == kind) {
			return metaMethodTable.metaInterfaceData.items[0].getter();
		}
		const uint32_t count = (metaMethodTable.metaInterfaceData.kinds & metaInterfaceCountMask);
		if(count > 1) {
			for(uint32_t i = 1; i < count; ++i) {
				if(metaMethodTable.metaInterfaceData.items[i].kind == kind) {
					return metaMethodTable.metaInterfaceData.items[i].getter();
				}
			}
		}
	}
	return nullptr;
}

void * UnifiedType::constructData(VariantData * data, const void * copyFrom) const
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

template <typename T>
int compareTwoValues(T a, T b)
{
	if(a < b) {
		return -1;
	}
	if(a > b) {
		return 1;
	}
	return 0;
}

} // namespace internal_


void * DeclareMetaTypeVoidBase::constructData(VariantData * /*data*/, const void * /*value*/)
{
	return nullptr;
}

void DeclareMetaTypeVoidBase::destroy(void * /*instance*/)
{
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
		const void * (*doGetUnifiedData)(),
		const internal_::UnifiedType * unifiedType,
		const TypeFlags typeFlags
	) noexcept
	:
		doGetUnifiedData(doGetUnifiedData),
		unifiedType(unifiedType),
		typeFlags(typeFlags)
{
}

bool MetaType::equal(const MetaType * other) const
{
	if(this == other) {
		return true;
	}
	if(other == nullptr) {
		return false;
	}
	if(getRawType() == other->getRawType()) {
		return true;
	}
	if(getModule() == other->getModule()) {
		return false;
	}
	if(getTypeKind() != other->getTypeKind() || getUpTypeCount() != other->getUpTypeCount()) {
		return false;
	}
	const int upTypeCount = getUpTypeCount();
	for(int i = 0; i < upTypeCount; ++i) {
		if(! getUpType(i)->equal(other->getUpType(i))) {
			return false;
		}
	}
	return true;
}

int MetaType::compare(const MetaType * other) const
{
	if(getModule() == other->getModule()) {
		return internal_::compareTwoValues(getRawType(), other->getRawType());
	}
	const int upTypeCount = getUpTypeCount();
	int result = internal_::compareTwoValues(upTypeCount, other->getUpTypeCount());
	if(result != 0) {
		return result;
	}
	for(int i = 0; i < upTypeCount; ++i) {
		result = getUpType(i)->compare(other->getUpType(i));
		if(result != 0) {
			return result;
		}
	}
	return 0;
}

const MetaType * MetaType::getUpType() const
{
	return unifiedType->upTypeData.count == 0 ? nullptr : unifiedType->upTypeData.upTypeList[0];
}

const MetaType * MetaType::getUpType(const int i) const
{
	return unifiedType->upTypeData.upTypeList[i];
}

int MetaType::getUpTypeCount() const noexcept
{
	return unifiedType->upTypeData.count;
}

void * MetaType::construct() const
{
	return constructData(nullptr, nullptr);
}

void * MetaType::copyConstruct(const void * copyFrom) const
{
	return constructData(nullptr, copyFrom);
}

void * MetaType::constructData(VariantData * data, const void * copyFrom) const
{
	return unifiedType->constructData(data, copyFrom);
}

void MetaType::destroy(void * instance) const
{
	unifiedType->destroy(instance);
}

bool MetaType::cast(Variant * result, const Variant & value, const MetaType * toMetaType) const
{
	return unifiedType->cast(result, value, toMetaType);
}

bool MetaType::castFrom(Variant * result, const Variant & value, const MetaType * fromMetaType) const
{
	return unifiedType->castFrom(result, value, fromMetaType);
}

bool commonCast(
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

const MetaType * voidMetaType = getMetaType<void>();

} // namespace metapp
