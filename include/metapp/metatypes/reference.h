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

#ifndef METAPP_REFERENCE_H_969872685611
#define METAPP_REFERENCE_H_969872685611

#include "metapp/metatype.h"

#include <functional>

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <T &>
{
	using Common = CommonDeclareMetaType<T &>;
	using UpType = T;
	static constexpr TypeKind typeKind = tkReference;

	static void * constructData(MetaTypeData * data, const void * copyFrom) {
		if(data != nullptr) {
			data->constructReference(copyFrom);
		}
		return nullptr;
	}

};

template <typename T>
struct DeclareMetaTypeBase <T &&> : public DeclareMetaTypeBase<T &>
{
};

template <typename T>
struct DeclareMetaTypeBase <std::reference_wrapper<T> >
	: public DeclareMetaTypeBase<T &>
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkReference;
	using Common = CommonDeclareMetaType<T &>;
	static constexpr TypeFlags typeFlags = tfReference | Common::typeFlags;

	using WrapperType = std::reference_wrapper<T>;

	static void * constructData(MetaTypeData * data, const void * copyFrom) {
		if(data != nullptr) {
			data->constructReference(&(T &)*(WrapperType *)copyFrom);
		}
		return nullptr;
	}

};


} // namespace metapp


#endif

