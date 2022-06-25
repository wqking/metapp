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
#include "metapp/allmetatypes.h"

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
	const Variant * fromVar,
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
		const MetaRepo * metaRepo = getMetaRepoList()->findMetaRepoForHierarchy(fromUpType);
		if(metaRepo != nullptr && metaRepo->isClassInHierarchy(toUpType)) {
			if(result != nullptr) {
				void * instance = nullptr;
				// Should not use getPointer() in utility
				if(fromMetaType->isPointer()) {
					instance = fromVar->get<void *>();
				}
				else {
					instance = fromVar->getAddress();
				}
				if(instance != nullptr) {
					instance = metaRepo->cast(instance, fromUpType, toUpType);
				}
				if(instance != nullptr) {
					if(toMetaType->isReference()) {
						const Variant temp = Variant::reference(*(int *)instance);
						*result = Variant::retype(toMetaType, temp);
					}
					else {
						const Variant temp = instance;
						*result = Variant::retype(toMetaType, temp);
					}
					return TristateBool::yes;
				}
			}
			else {
				if(metaRepo->getRelationship(fromUpType, toUpType) != MetaRepo::Relationship::none) {
					return TristateBool::yes;
				}
			}
			return TristateBool::no;
		}
	}
	return TristateBool::unknown;
}

TristateBool doCastPointerReference(
	Variant * result,
	const Variant * fromVar,
	const MetaType * fromMetaType,
	const MetaType * toMetaType
)
{
	if(fromMetaType->isReference() || toMetaType->isReference()) {
		if(getNonReferenceMetaType(fromMetaType)->cast(
			result, fromVar, getNonReferenceMetaType(toMetaType))) {
			return TristateBool::yes;
		}
	}

	return doCastObject(result, fromVar, fromMetaType, toMetaType);
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


VariantData DeclareMetaTypeVoidBase::constructVariantData(const void * /*value*/, const CopyStrategy /*copyStrategy*/)
{
	return VariantData();
}

void * DeclareMetaTypeVoidBase::constructData(const void * /*value*/, void * /*memory*/, const CopyStrategy /*copyStrategy*/)
{
	return nullptr;
}

void DeclareMetaTypeVoidBase::destroy(void * /*instance*/, const bool /*freeMemory*/)
{
}

void DeclareMetaTypeVoidBase::dtor(void * /*instance*/, const bool /*freeMemory*/)
{
}

bool DeclareMetaTypeVoidBase::cast(Variant * /*result*/, const Variant * /*fromVar*/, const MetaType * /*toMetaType*/)
{
	return false;
}

bool DeclareMetaTypeVoidBase::castFrom(Variant * /*result*/, const Variant * /*fromVar*/, const MetaType * /*fromMetaType*/)
{
	return false;
}


MetaType::MetaType(
		const internal_::MetaTable & metaTable,
		const internal_::UnifiedType * unifiedType,
		const TypeFlags typeFlags
	) noexcept
	:
		metaTable(metaTable),
		unifiedType(unifiedType),
		typeFlags(typeFlags)
{
}

bool MetaType::doCheckEqualCrossModules(const MetaType * other) const
{
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

bool commonCast(
	Variant * result,
	const Variant * fromVar,
	const MetaType * fromMetaType,
	const MetaType * toMetaType
)
{
	const internal_::TristateBool tristate = internal_::doCastPointerReference(result, fromVar, fromMetaType, toMetaType);
	if(tristate != internal_::TristateBool::unknown) {
		return tristate == internal_::TristateBool::yes;
	}

	if(toMetaType->castFrom(result, fromVar, fromMetaType)) {
		return true;
	}

	return false;
}

const MetaType * voidMetaType = getMetaType<void>();

} // namespace metapp
