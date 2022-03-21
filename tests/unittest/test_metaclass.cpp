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

#include "test.h"

#include "metapp/variant.h"
#include "metapp/interfaces/metaclass.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <iostream>
#include <climits>

struct BaseFirst
{
	BaseFirst() : first(0) {
	}

	explicit BaseFirst(const int value) : first(value) {
	}

	int first;
};

struct BaseSecond
{
	BaseSecond() : second(0) {
	}

	explicit BaseSecond(const int value) : second(value) {
	}

	int second;
};

struct SonFirst : BaseFirst
{
};

struct SonFirstSecond : BaseFirst, BaseSecond
{
};


constexpr metapp::TypeKind tkBaseFirst = metapp::tkUser;
constexpr metapp::TypeKind tkBaseSecond = metapp::tkUser + 1;
constexpr metapp::TypeKind tkSonFirst = metapp::tkUser + 10;
constexpr metapp::TypeKind tkSonFirstSecond = metapp::tkUser + 11;

template <>
struct metapp::DeclareMetaType <BaseFirst> : metapp::DeclareMetaTypeBase <BaseFirst>
{
	static constexpr metapp::TypeKind typeKind = tkBaseFirst;

	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<BaseFirst>(),
			[](metapp::MetaClass & mc) {
				mc.addConstructor(metapp::Constructor<BaseFirst()>());
				mc.addConstructor(metapp::Constructor<BaseFirst(int)>());
				mc.addField("first", &BaseFirst::first);
			}
		);
		return &metaClass;
	}

};

template <>
struct metapp::DeclareMetaType <SonFirst> : metapp::DeclareMetaTypeBase <SonFirst>
{
	static constexpr metapp::TypeKind typeKind = tkSonFirst;

	static const metapp::MetaClass * getMetaClass() {
		static metapp::MetaClass metaClass(
			metapp::getMetaType<SonFirst>(),
			[](metapp::MetaClass & mc) {
			}
		);
		return &metaClass;
	}

};

template <>
struct metapp::DeclareMetaType <SonFirstSecond> : metapp::DeclareMetaTypeBase <SonFirstSecond>
{
	static constexpr metapp::TypeKind typeKind = tkSonFirstSecond;

	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<SonFirstSecond>(),
			[](metapp::MetaClass & mc) {
			}
		);
		return &metaClass;
	}

};


TEST_CASE("MetaClass, cast")
{
}

