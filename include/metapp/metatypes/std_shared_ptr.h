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

#ifndef METAPP_STD_SHARED_PTR_H_969872685611
#define METAPP_STD_SHARED_PTR_H_969872685611

#include "metapp/metatype.h"

#include <memory>

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <std::shared_ptr<T> >
{
	using SharedPtr = std::shared_ptr<T>;

	using UpType = T;
	static constexpr TypeKind typeKind = tkStdSharedPtr;

	static void * constructData(MetaTypeData * data, const void * copyFrom) {
		if(data != nullptr) {
			if(copyFrom == nullptr) {
				data->constructSharedPtr(std::static_pointer_cast<void>(SharedPtr()));
			}
			else {
				data->constructSharedPtr(std::static_pointer_cast<void>(*(SharedPtr *)copyFrom));
			}
			return nullptr;
		}
		else {
			if(copyFrom == nullptr) {
				return new SharedPtr();
			}
			else {
				return new SharedPtr(*(SharedPtr *)copyFrom);
			}
		}
	}

	static bool cast(Variant * result, const Variant & value, const MetaType * toMetaType) {
		if(toMetaType->isPointer()) {
			if(result != nullptr) {
				*result = value.get<SharedPtr &>().get();
			}
			return true;
		}
		if(toMetaType->getTypeKind() == tkStdWeakPtr && getMetaType<SharedPtr>()->getUpType()->equal(toMetaType->getUpType())) {
			if(result != nullptr) {
				*result = std::weak_ptr<T>(value.get<SharedPtr &>());
			}
			return true;
		}

		return commonCast(result, value, getMetaType<SharedPtr>(), toMetaType);
	}

};


} // namespace metapp


#endif

