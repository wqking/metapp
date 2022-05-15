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
#include "include/dataprovider.h"

#include "metapp/variant.h"
#include "metapp/allmetatypes.h"
#include "metapp/metarepo.h"

#include <string>
#include <cstring>

namespace {

TEMPLATE_LIST_TEST_CASE("Variant, cast T to T", "", TestTypes_All)
{
	auto dataProvider = TestDataProvider<TestType>();
	for(int dataIndex = 0; dataIndex < dataProvider.getDataCount(); ++dataIndex) {
		metapp::Variant v(dataProvider.getData(dataIndex));
		REQUIRE(metapp::getTypeKind(v) == dataProvider.getTypeKind());

		REQUIRE(v.canCast<TestType>());
		REQUIRE(v.cast<TestType>().template get<TestType>() == dataProvider.getData(dataIndex));
	}
}

TEMPLATE_LIST_TEST_CASE("Variant, cast T to T &", "", TestTypes_All)
{
	auto dataProvider = TestDataProvider<TestType>();
	for(int dataIndex = 0; dataIndex < dataProvider.getDataCount(); ++dataIndex) {
		metapp::Variant v(dataProvider.getData(dataIndex));
		REQUIRE(metapp::getTypeKind(v) == dataProvider.getTypeKind());

		REQUIRE(v.canCast<TestType &>());
		REQUIRE(v.cast<TestType &>().template get<TestType &>() == dataProvider.getData(dataIndex));
	}
}

TEMPLATE_LIST_TEST_CASE("Variant, cast T to U", "", TestTypes_Pairs_Arithmetic)
{
	using First = typename metapp::TypeListGetAt<TestType, 0>::Type;
	using Second = typename metapp::TypeListGetAt<TestType, 1>::Type;
	//printf("%d %d\n", metapp::getMetaType<First>()->getTypeKind(), metapp::getMetaType<Second>()->getTypeKind());

	auto dataProvider = TestDataProvider<First>();
	for(int dataIndex = 0; dataIndex < dataProvider.getDataCount(); ++dataIndex) {
		metapp::Variant v(dataProvider.getData(dataIndex));
		REQUIRE(metapp::getTypeKind(v) == dataProvider.getTypeKind());
		REQUIRE(v.canCast<Second>());
		REQUIRE(v.cast<Second>().template get<Second>() == (Second)dataProvider.getData(dataIndex));
	}
}

TEMPLATE_LIST_TEST_CASE("Variant, cast const T & to U", "", TestTypes_Pairs_Arithmetic)
{
	using First = typename metapp::TypeListGetAt<TestType, 0>::Type;
	using Second = typename metapp::TypeListGetAt<TestType, 1>::Type;

	auto dataProvider = TestDataProvider<First>();
	for(int dataIndex = 0; dataIndex < dataProvider.getDataCount(); ++dataIndex) {
		First n = dataProvider.getData(dataIndex);
		metapp::Variant v(metapp::Variant::create<const First &>(n));
		REQUIRE(v.getMetaType()->getTypeKind() == metapp::tkReference);
		REQUIRE(v.getMetaType()->getUpType()->getTypeKind() == dataProvider.getTypeKind());
		
		REQUIRE(v.canCast<Second>());
		auto casted = v.cast<Second>();
		auto castedDataProvider = TestDataProvider<Second>();
		REQUIRE(casted.getMetaType()->getTypeKind() == castedDataProvider.getTypeKind());
		REQUIRE(casted.template get<Second>() == (Second)dataProvider.getData(dataIndex));
	}
}

TEST_CASE("Variant, cast int ** to int ** and int ** &")
{
	int n = 5;
	int * pn = &n;
	metapp::Variant v(&pn);
	REQUIRE(! v.canCast<int>());
	REQUIRE(! v.canCast<int *>());
	REQUIRE(v.canCast<int **>());
	REQUIRE(v.canCast<int ** &>());
	REQUIRE(v.canCast<int volatile * const *>());
	REQUIRE(v.canCast<int volatile * const * &>());
	REQUIRE(v.cast<int **>().get<int **>() == &pn);
	REQUIRE(v.cast<int ** &>().get<int ** &>() == &pn);
	REQUIRE(v.cast<int volatile * const *>().get<int **>() == &pn);
	REQUIRE(v.cast<int volatile * const * &>().get<int **>() == &pn);
}

TEST_CASE("Variant, can't cast int * to int or int &")
{
	int n = 5;
	metapp::Variant v(&n);
	REQUIRE(! v.canCast<int>());
	REQUIRE(! v.canCast<int &>());
	REQUIRE_THROWS(v.cast<int>());
	REQUIRE_THROWS(v.cast<int &>());
	
	REQUIRE(v.castSilently<int>().isEmpty());
	REQUIRE(v.castSilently<int &>().isEmpty());
}

TEST_CASE("Variant, can't cast int & to int *")
{
	int n = 5;
	metapp::Variant v(metapp::Variant::create<int &>(n));
	REQUIRE(! v.canCast<int *>());
	REQUIRE_THROWS(v.cast<int *>());
	REQUIRE(v.castSilently<int *>().isEmpty());
}

TEST_CASE("Variant, can't cast int to int *")
{
	metapp::Variant v(5);
	REQUIRE(! v.canCast<int *>());
	REQUIRE_THROWS(v.cast<int *>());
}

TEST_CASE("Variant, cast int * to int * &")
{
	int n = 5;
	metapp::Variant v(&n);
	REQUIRE(v.canCast<int * &>());
	REQUIRE(v.canCast<int const * &>());
	REQUIRE(v.cast<int * &>().get<int * &>() == &n);
	REQUIRE(v.cast<int const * &>().get<int * &>() == &n);
	REQUIRE(v.castSilently<int const * &>().get<int * &>() == &n);
}

TEST_CASE("Variant, cast int * & to int *")
{
	int n = 5;
	int * pn = &n;
	metapp::Variant v(metapp::Variant::create<int * &>(pn));
	REQUIRE(v.canCast<int *>());
	REQUIRE(v.cast<int *>().get<int *>() == &n);
	REQUIRE(v.cast<int * &>().get<int * &>() == &n);
	REQUIRE(v.cast<int * &>().get<int *>() == &n);
	REQUIRE(v.cast<int *>().get<int * &>() == &n);
	REQUIRE(v.castSilently<int *>().get<int * &>() == &n);
}

TEST_CASE("Variant, can't cast int * to long *")
{
	int n = 5;
	metapp::Variant v(&n);
	REQUIRE(! v.canCast<long *>());
	REQUIRE_THROWS(v.cast<long *>());
}

TEST_CASE("Variant, cast std::string & to std::string")
{
	std::string s = "hello";
	metapp::Variant v(metapp::Variant::create<std::string &>(s));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);

	REQUIRE(v.canCast<std::string>());
	REQUIRE(v.cast<std::string>().get<std::string>() == "hello");
}

TEST_CASE("Variant, cast std::string to std::string &")
{
	std::string s = "hello";
	metapp::Variant v(metapp::Variant::create<std::string>(s));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdString);

	REQUIRE(v.canCast<std::string &>());
	REQUIRE(v.cast<std::string &>().get<std::string &>() == "hello");
}

TEST_CASE("Variant, cast std::string to const std::string &")
{
	std::string s = "hello";
	metapp::Variant v(metapp::Variant::create<std::string>(s));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdString);
	REQUIRE(! v.getMetaType()->isConst());

	REQUIRE(v.canCast<const std::string &>());
	auto casted = v.cast<const std::string &>();
	REQUIRE(casted.get<const std::string &>() == "hello");
	REQUIRE(casted.getMetaType()->getTypeKind() == metapp::tkStdString);
	REQUIRE(casted.getMetaType()->isConst());
}

TEST_CASE("Variant, cast char * to std::string")
{
	const char * s = "hello";
	metapp::Variant v(s);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);

	REQUIRE(v.get<const char *>() == std::string("hello"));
	REQUIRE(v.get<char *>() == std::string("hello"));
	REQUIRE((const char *)v.get<const volatile char *>() == std::string("hello"));

	REQUIRE(v.cast<std::string>().get<const std::string &>() == "hello");
	REQUIRE(v.cast<const std::string &>().get<const std::string &>() == "hello");
	REQUIRE(v.cast<std::string &>().get<const std::string &>() == "hello");
}

TEST_CASE("Variant, cast char[6] to std::string")
{
	char s[6] = "hello";
	metapp::Variant v(metapp::Variant::create<char[6]>(s));
	REQUIRE(metapp::getTypeKind(v) == metapp::tkArray);

	REQUIRE(v.get<char []>() == std::string("hello"));
	REQUIRE(v.get<char [6]>() == std::string("hello"));
	REQUIRE(v.get<const char []>() == std::string("hello"));

	REQUIRE(v.canCast<std::string>());
	REQUIRE(v.cast<std::string>().get<const std::string &>() == "hello");
}

TEST_CASE("Variant, cast wchar_t * to std::wstring")
{
	const wchar_t * s = L"hello";
	metapp::Variant v(s);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);

	REQUIRE(v.get<const wchar_t *>() == std::wstring(L"hello"));
	REQUIRE(v.get<wchar_t *>() == std::wstring(L"hello"));
	REQUIRE((const wchar_t *)v.get<const volatile wchar_t *>() == std::wstring(L"hello"));

	REQUIRE(v.cast<std::wstring>().get<const std::wstring &>() == L"hello");
	REQUIRE(v.cast<const std::wstring &>().get<const std::wstring &>() == L"hello");
	REQUIRE(v.cast<std::wstring &>().get<const std::wstring &>() == L"hello");
}

struct MyClass
{
	int value;

	MyClass() : value(0) {}
	explicit MyClass(const int n) : value(n) {}
	virtual ~MyClass() {}

	virtual std::string getName() const {
		return "MyClass";
	}
};

struct Derived : MyClass
{
	using MyClass::MyClass;

	virtual std::string getName() const override {
		return "Derived";
	}
};

TEST_CASE("Variant, cast object")
{
	MyClass obj{ 38 };
	
	metapp::Variant v(obj);
	REQUIRE(v.canCast<MyClass>());
	REQUIRE(v.canCast<MyClass &>());
	REQUIRE(v.canCast<const MyClass &>());
	REQUIRE(v.canCast<const volatile MyClass &>());
	v.cast<MyClass>().get<MyClass>();
	REQUIRE(v.cast<MyClass>().get<MyClass>().value == 38);
}

TEST_CASE("Variant, cast Derived * to MyClass *")
{
	metapp::getMetaRepo()->clear();
	metapp::getMetaRepo()->registerBase<Derived, MyClass>();

	Derived derived;
	metapp::Variant v(&derived);
	REQUIRE(v.canCast<MyClass *>());
	REQUIRE(! v.canCast<MyClass &>());
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	REQUIRE(v.getMetaType()->getUpType() == metapp::getMetaType<Derived>());

	metapp::Variant casted = v.cast<MyClass *>();
	REQUIRE(metapp::getTypeKind(casted) == metapp::tkPointer);
	REQUIRE(casted.getMetaType()->getUpType() == metapp::getMetaType<MyClass>());
}

TEST_CASE("Variant, cast MyClass * to Derived *")
{
	metapp::getMetaRepo()->clear();
	metapp::getMetaRepo()->registerBase<Derived, MyClass>();

	Derived derived;
	MyClass * myClass = &derived;
	metapp::Variant v(myClass);
	REQUIRE(v.canCast<MyClass *>());
	REQUIRE(v.canCast<Derived *>());
	REQUIRE(! v.canCast<MyClass &>());
	REQUIRE(metapp::getTypeKind(v) == metapp::tkPointer);
	REQUIRE(v.getMetaType()->getUpType() == metapp::getMetaType<MyClass>());

	metapp::Variant casted = v.cast<Derived *>();
	REQUIRE(metapp::getTypeKind(casted) == metapp::tkPointer);
	REQUIRE(casted.getMetaType()->getUpType() == metapp::getMetaType<Derived>());
}

TEST_CASE("Variant, cast Derived & to MyClass &")
{
	metapp::getMetaRepo()->clear();
	metapp::getMetaRepo()->registerBase<Derived, MyClass>();

	Derived derived {};
	metapp::Variant v(metapp::Variant::create<Derived &>(derived));
	REQUIRE(! v.canCast<MyClass *>());
	REQUIRE(v.canCast<MyClass &>());
	REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
	REQUIRE(v.getMetaType()->getUpType() == metapp::getMetaType<Derived>());

	metapp::Variant casted = v.cast<MyClass &>();
	REQUIRE(metapp::getTypeKind(casted) == metapp::tkReference);
	REQUIRE(casted.getMetaType()->getUpType() == metapp::getMetaType<MyClass>());
}

TEST_CASE("Variant, cast Derived to MyClass &")
{
	metapp::getMetaRepo()->clear();
	metapp::getMetaRepo()->registerBase<Derived, MyClass>();

	metapp::Variant v(metapp::Variant::create<Derived>(Derived {}));
	REQUIRE(! v.canCast<MyClass *>());
	REQUIRE(v.canCast<MyClass &>());
	REQUIRE(v.getMetaType() == metapp::getMetaType<Derived>());

	metapp::Variant casted = v.cast<MyClass &>();
	REQUIRE(metapp::getTypeKind(casted) == metapp::tkReference);
	REQUIRE(casted.getMetaType()->getUpType() == metapp::getMetaType<MyClass>());
	REQUIRE(casted.get<MyClass &>().getName() == "Derived");
}

struct ImplictConstruct
{
	ImplictConstruct(const std::string & text) : text(text) {}
	ImplictConstruct(const MyClass & obj) : text(std::to_string(obj.value)) {}

	std::string text;
};

}

template <>
struct metapp::DeclareMetaType <ImplictConstruct>
	: metapp::DeclareMetaTypeBase <ImplictConstruct>,
		metapp::CastFromTypes <ImplictConstruct, metapp::TypeList<std::string, MyClass> >
{
};

namespace {

TEST_CASE("Variant, implicitly cast std::string to ImplictConstruct")
{
	metapp::Variant v(std::string("abc"));
	REQUIRE(v.canCast<ImplictConstruct>());
	REQUIRE(v.cast<ImplictConstruct>().get<ImplictConstruct &>().text == "abc");
	REQUIRE(v.canCast<const ImplictConstruct &>());
	REQUIRE(v.cast<const ImplictConstruct &>().get<const ImplictConstruct &>().text == "abc");
}

TEST_CASE("Variant, implicitly cast MyClass to ImplictConstruct")
{
	metapp::Variant v(MyClass{6});
	REQUIRE(v.canCast<ImplictConstruct>());
	REQUIRE(v.cast<ImplictConstruct>().get<ImplictConstruct &>().text == "6");
	REQUIRE(v.canCast<const ImplictConstruct &>());
	REQUIRE(v.cast<const ImplictConstruct &>().get<const ImplictConstruct &>().text == "6");
}

TEST_CASE("Variant, implicitly cast const MyClass & to ImplictConstruct")
{
	MyClass obj{6};
	metapp::Variant v = metapp::Variant::create<const MyClass &>(obj);
	REQUIRE(v.canCast<ImplictConstruct>());
	REQUIRE(v.cast<ImplictConstruct>().get<ImplictConstruct &>().text == "6");
	REQUIRE(v.canCast<const ImplictConstruct &>());
	REQUIRE(v.cast<const ImplictConstruct &>().get<const ImplictConstruct &>().text == "6");
}

struct ImplicitTypeCast
{
	int value;

	operator std::string () const {
		return std::to_string(value);
	}
	
	operator MyClass () const {
		return MyClass{value};
	}
};

}

template <>
struct metapp::DeclareMetaType <ImplicitTypeCast>
	: metapp::DeclareMetaTypeBase <ImplicitTypeCast>,
		metapp::CastToTypes <ImplicitTypeCast, metapp::TypeList<std::string, MyClass> >
{
};

namespace {

TEST_CASE("Variant, implicitly cast ImplicitTypeCast to std::string")
{
	ImplicitTypeCast xxx {5};
	MyClass a = static_cast<MyClass>(xxx);
	std::string n = xxx;
	REQUIRE(n == "5");
	metapp::Variant v(ImplicitTypeCast {38});
	v.canCast<std::string>();
	REQUIRE(v.canCast<std::string>());
	REQUIRE(v.cast<std::string>().get<std::string>() == "38");
}

TEST_CASE("Variant, implicitly cast ImplicitTypeCast to MyClass")
{
	ImplicitTypeCast xxx {5};
	MyClass n = xxx;
	REQUIRE(n.value == 5);
	metapp::Variant v(ImplicitTypeCast {38});
	REQUIRE(v.canCast<MyClass>());
	REQUIRE(v.cast<MyClass>().get<MyClass>().value == 38);
	REQUIRE(v.canCast<const MyClass &>());
	REQUIRE(v.cast<const MyClass &>().get<const MyClass &>().value == 38);
}

TEST_CASE("Variant, implicitly cast const ImplicitTypeCast & to MyClass")
{
	ImplicitTypeCast obj{38};
	metapp::Variant v = metapp::Variant::create<const ImplicitTypeCast &>(obj);
	REQUIRE(v.canCast<MyClass>());
	REQUIRE(v.cast<MyClass>().get<MyClass>().value == 38);
	REQUIRE(v.canCast<const MyClass &>());
	REQUIRE(v.cast<const MyClass &>().get<const MyClass &>().value == 38);
}


} // namespace
