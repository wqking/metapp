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

#ifndef METAPP_CONSTRUCTOR_H_969872685611
#define METAPP_CONSTRUCTOR_H_969872685611

#include "metapp/metatype.h"
#include "metapp/variant.h"
#include "metapp/interfaces/bases/metacallablebase.h"
#include "metapp/interfaces/bases/metamemberbase.h"
#include "metapp/metatypes/utils/declareutil.h"

namespace metapp {

template <typename T>
struct Constructor;

template <typename Class, typename ...Args>
struct Constructor<Class (Args...)>
{
	Class * operator() (Args ... args) {
		return new Class(args...);
	}
};

template <typename Class, typename ...Args>
struct DeclareMetaTypeBase <Constructor<Class (Args...)> >
	: public MetaCallableBase<Constructor<Class (Args...)>, void, Class *, Args...>,
		public MetaMemberBase<Class>
{
public:
	using UpType = TypeList<Class, Args...>;
	static constexpr TypeKind typeKind = tkConstructor;

};


} // namespace metapp


#endif

