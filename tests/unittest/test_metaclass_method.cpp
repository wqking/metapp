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
#include "metapp/metarepo.h"
#include "metapp/utils/utility.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

struct A
{
	std::string methodA() const noexcept {
		return "hello";
	}

	virtual int virtualMethod() const {
		return value;
	}

	int value;
};

struct B : A
{
	std::string methodB(const std::string & s) const {
		return "good" + s;
	}

	int virtualMethod() const override {
		return value + 9;
	}
};

struct B2
{
	int methodB2() {
		return 8;
	}
};

struct C : B, B2
{
	int methodC() {
		return 38;
	}
	
	int virtualMethod() const override {
		return value + 1;
	}
	
	void notReflected() {
	}
};

} // namespace

template <>
struct metapp::DeclareMetaType <A> : metapp::DeclareMetaTypeBase <A>
{
	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<A>(),
			[](metapp::MetaClass & mc) {
				mc.registerMethod("methodA", &A::methodA);
				mc.registerMethod("virtualMethod", &A::virtualMethod);
			}
		);
		return &metaClass;
	}

};

template <>
struct metapp::DeclareMetaType <B> : metapp::DeclareMetaTypeBase <B>
{
	static void setup()
	{
		metapp::getMetaRepo()->registerBase<B, A>();
	}

	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<B>(),
			[](metapp::MetaClass & mc) {
				auto & item = mc.registerMethod("methodB", &B::methodB);
				item.addAnnotation("hello", 5);
			}
		);
		return &metaClass;
	}

};

template <>
struct metapp::DeclareMetaType <B2> : metapp::DeclareMetaTypeBase <B2>
{
	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<B2>(),
			[](metapp::MetaClass & mc) {
				mc.registerMethod("methodB2", &B2::methodB2);
			}
		);
		return &metaClass;
	}

};

template <>
struct metapp::DeclareMetaType <C> : metapp::DeclareMetaTypeBase <C>
{
	static void setup()
	{
		metapp::getMetaRepo()->registerBase<C, B, B2>();
	}

	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<C>(),
			[](metapp::MetaClass & mc) {
				mc.registerMethod("methodC", &C::methodC);
			}
		);
		return &metaClass;
	}

};


TEST_CASE("MetaClass, method, struct B")
{
	auto metaTypeB = metapp::getMetaType<B>();
	auto metaClassB = metaTypeB->getMetaClass();
	B b;
	b.value = 2;

	REQUIRE(metaClassB->getMethod("notExist").isEmpty());
	
	const auto & methodB = metaClassB->getMethod("methodB");
	REQUIRE(metapp::invokeCallable(methodB.getMethod(), &b, "great").get<const std::string &>() == "goodgreat");
	REQUIRE(methodB.getAnnotation("hello").get<int>() == 5);

	const auto & virtualMethod = metaClassB->getMethod("virtualMethod");
	REQUIRE(metapp::invokeCallable(virtualMethod.getMethod(), &b).get<int>() == 11);
}

