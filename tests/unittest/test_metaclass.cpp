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

namespace metapp {

constexpr TypeKind tkBaseFirst = tkUser;
constexpr TypeKind tkBaseSecond = tkUser + 1;
constexpr TypeKind tkSonFirst = tkUser + 10;
constexpr TypeKind tkSonFirstSecond = tkUser + 11;

template <>
struct DeclareMetaType <BaseFirst> : public DeclareMetaTypeBase <BaseFirst>
{
	static constexpr TypeKind typeKind = tkBaseFirst;

	static const MetaClass * getMetaClass() {
		static const MetaClass metaClass(
			[](MetaClass & mc) {
				mc.addConstructor(metapp::Constructor<BaseFirst()>());
				mc.addConstructor(metapp::Constructor<BaseFirst(int)>());
				mc.addField("first", &BaseFirst::first);
			}
		);
		return &metaClass;
	}

};

template <>
struct DeclareMetaType <SonFirst> : public DeclareMetaTypeBase <SonFirst>
{
	static constexpr TypeKind typeKind = tkSonFirst;

	static const MetaClass * getMetaClass() {
		static MetaClass metaClass(
			[](MetaClass & mc) {
			}
		);
		return &metaClass;
	}

};

template <>
struct DeclareMetaType <SonFirstSecond> : public DeclareMetaTypeBase <SonFirstSecond>
{
	static constexpr TypeKind typeKind = tkSonFirstSecond;

	static const MetaClass * getMetaClass() {
		static const MetaClass metaClass(
			[](MetaClass & mc) {
			}
		);
		return &metaClass;
	}

};


} // namespace metapp

TEST_CASE("MetaClass, cast")
{
}

