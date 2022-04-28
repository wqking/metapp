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
	REQUIRE(metaType->getMetaMember() != nullptr);
	REQUIRE(metaType->getMetaMember()->getClassType(v)->getTypeKind() == 2000);
	REQUIRE(metaType->getMetaCallable()->getParameterCount(v) == 2);
	REQUIRE(matchUpTypeKinds(metaType->getMetaCallable()->getReturnType(v), { tkPointer, tkVoid }));
	REQUIRE(matchUpTypeKinds(metaType->getMetaCallable()->getParameterType(v, 0), { tkInt }));
	REQUIRE(matchUpTypeKinds(metaType->getMetaCallable()->getParameterType(v, 1), { tkReference, tkStdVector, tkInt }));
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
		metapp::Variant arguments[] = { metapp::Variant::create<int &>(a), metapp::Variant::create<std::string &>(b) };
		Base obj { 5 };
		v.getMetaType()->getMetaCallable()->invoke(v, &obj, arguments, 2);
		REQUIRE(a == 8);
		REQUIRE(b == "Good");
	}

	SECTION("virtual") {
		metapp::Variant v(&Base::add);
		metapp::Variant arguments[] = { 7 };
		Derived obj { 5 };
		metapp::Variant result = v.getMetaType()->getMetaCallable()->invoke(v, &obj, arguments, 1);
		REQUIRE(result.get<int>() == 12);
	}

}


} // namespace