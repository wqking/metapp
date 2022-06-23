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
#include "metapp/allmetatypes.h"

namespace {

struct Class1
{
	const void * func(int, const std::vector<int> &) const { return nullptr; }
	const std::array<int, 5> * data;
};

} // namespace

template <>
struct metapp::DeclareMetaType <Class1> : metapp::DeclareMetaTypeBase<Class1>
{
	static constexpr metapp::TypeKind typeKind = 2000;
};

namespace {

TEST_CASE("metatypes, tkMemberFunction, types")
{
	metapp::Variant v(&Class1::func);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkMemberFunction);
	
	using namespace metapp;
	auto metaType = v.getMetaType();
	REQUIRE(metaType->isConst());
	REQUIRE(metaType->getMetaCallable()->getParameterCountInfo(v).getMinParameterCount() == 2);
	REQUIRE(metaType->getMetaCallable()->getReturnType(v)->equal(metapp::getMetaType<void *>()));
	REQUIRE(metaType->getMetaCallable()->getParameterType(v, 0)->equal(metapp::getMetaType<int>()));
	REQUIRE(metaType->getMetaCallable()->getParameterType(v, 1)->equal(metapp::getMetaType<std::vector<int> &>()));
	REQUIRE(metaType->getMetaCallable()->getClassType(v)->equal(metapp::getMetaType<Class1>()));
}

struct Base
{
	Base(const int n = 0) : myValue(n) {
		//printf("===%d\n", n);
	}

	int myValue;

	void func1(int & a, std::string & b)
	{
		a = 3 + myValue;
		b = "Good";
	}

	virtual int add(const int ) const {
		return 0;
	}

	void method() {
	}

	void constMethod() const {
	}

};

struct Derived : Base
{
	using Base::Base;

	virtual int add(const int a) const override {
		return a + myValue;
	}
};

TEST_CASE("metatypes, tkMemberFunction, invoke")
{
	SECTION("Non-virtual") {
		metapp::Variant v(&Base::func1);
		int a = 0;
		std::string b;
		metapp::Variant arguments[] = { metapp::Variant::reference(a), metapp::Variant::reference(b) };
		Base obj { 5 };
		v.getMetaType()->getMetaCallable()->invoke(v, &obj, arguments);
		REQUIRE(a == 8);
		REQUIRE(b == "Good");
	}

	SECTION("virtual") {
		metapp::Variant v(&Base::add);
		Derived obj { 5 };
		metapp::Variant result = metapp::callableInvoke(v, &obj, 7);
		REQUIRE(result.get<int>() == 12);
	}

}

TEST_CASE("metatypes, tkMemberFunction, canInvoke, object instance CV")
{
	REQUIRE(metapp::callableCanInvoke(&Base::method, (Base *)0));
	REQUIRE(! metapp::callableCanInvoke(&Base::method, (const Base *)0));
	REQUIRE(metapp::callableCanInvoke(&Base::constMethod, (Base *)0));
	REQUIRE(metapp::callableCanInvoke(&Base::constMethod, (const Base *)0));

}


} // namespace
