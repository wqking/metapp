#include "test.h"
#include "include/dataprovider.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

#include <string>
#include <cstring>

namespace {

TEMPLATE_LIST_TEST_CASE("Variant, cast T to T", "", TestTypes_All)
{
	auto dataProvider = TestDataProvider<TestType>();
	for(size_t dataIndex = 0; dataIndex < dataProvider.getDataCount(); ++dataIndex) {
		metapp::Variant v(dataProvider.getData(dataIndex));
		REQUIRE(metapp::getTypeKind(v) == dataProvider.getTypeKind());

		REQUIRE(v.canCast<TestType>());
		REQUIRE(v.cast<TestType>().template get<TestType>() == dataProvider.getData(dataIndex));
	}
}

TEMPLATE_LIST_TEST_CASE("Variant, cast T to T &", "", TestTypes_All)
{
	auto dataProvider = TestDataProvider<TestType>();
	for(size_t dataIndex = 0; dataIndex < dataProvider.getDataCount(); ++dataIndex) {
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
	for(size_t dataIndex = 0; dataIndex < dataProvider.getDataCount(); ++dataIndex) {
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
	for(size_t dataIndex = 0; dataIndex < dataProvider.getDataCount(); ++dataIndex) {
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

TEMPLATE_LIST_TEST_CASE("Variant, cast const T & to const U &", "", TestTypes_Pairs_Arithmetic)
{
	using First = typename metapp::TypeListGetAt<TestType, 0>::Type;
	using Second = typename metapp::TypeListGetAt<TestType, 1>::Type;

	auto dataProvider = TestDataProvider<First>();
	for(size_t dataIndex = 0; dataIndex < dataProvider.getDataCount(); ++dataIndex) {
		First n = dataProvider.getData(dataIndex);
		metapp::Variant v(metapp::Variant::create<const First &>(n));
		REQUIRE(v.getMetaType()->getTypeKind() == metapp::tkReference);
		REQUIRE(v.getMetaType()->getUpType()->getTypeKind() == dataProvider.getTypeKind());
		
		REQUIRE(v.canCast<const Second &>());
		auto casted = v.cast<const Second &>();
		auto castedDataProvider = TestDataProvider<Second>();
		REQUIRE(casted.getMetaType()->getTypeKind() == metapp::tkReference);
		REQUIRE(casted.getMetaType()->getUpType()->getTypeKind() == castedDataProvider.getTypeKind());
		// This doesn't work because the cast is similar as (double &)n, which n is int &
		//REQUIRE(casted.get<Second>() == (Second)dataProvider.getData(dataIndex));
	}
}

TEMPLATE_LIST_TEST_CASE("Variant, cast const T * to const U *", "", TestTypes_Pairs_Arithmetic)
{
	using First = typename metapp::TypeListGetAt<TestType, 0>::Type;
	using Second = typename metapp::TypeListGetAt<TestType, 1>::Type;

	auto dataProvider = TestDataProvider<First>();
	for(size_t dataIndex = 0; dataIndex < dataProvider.getDataCount(); ++dataIndex) {
		First n = dataProvider.getData(dataIndex);
		metapp::Variant v(&n);
		REQUIRE((v.getMetaType()->getTypeKind() == metapp::tkPointer
			|| v.getMetaType()->getTypeKind() == metapp::tkCharPtr
			|| v.getMetaType()->getTypeKind() == metapp::tkWideCharPtr
			));
		REQUIRE(v.getMetaType()->getUpType()->getTypeKind() == dataProvider.getTypeKind());
		
		REQUIRE(v.canCast<const Second *>());
		auto casted = v.cast<const Second *>();
		auto castedDataProvider = TestDataProvider<Second>();
		REQUIRE((casted.getMetaType()->getTypeKind() == metapp::tkPointer
			|| casted.getMetaType()->getTypeKind() == metapp::tkCharPtr
			|| casted.getMetaType()->getTypeKind() == metapp::tkWideCharPtr
		));
		REQUIRE(casted.getMetaType()->getUpType()->getTypeKind() == castedDataProvider.getTypeKind());
	}
}

TEST_CASE("Variant, can't cast int * to int or int &")
{
	int n = 5;
	metapp::Variant v(&n);
	REQUIRE(! v.canCast<int>());
	REQUIRE(! v.canCast<int &>());
	REQUIRE_THROWS(v.cast<int>());
	REQUIRE_THROWS(v.cast<int &>());
}

TEST_CASE("Variant, can't cast int & to int *")
{
	int n = 5;
	metapp::Variant v(metapp::Variant::create<int &>(n));
	REQUIRE(! v.canCast<int *>());
	REQUIRE_THROWS(v.cast<int *>());
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
	REQUIRE(v.cast<int * &>().get<int * &>() == &n);
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
}

struct MyClass
{
	int value;
};

struct Derived : public MyClass
{
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

TEST_CASE("Variant, cast std::string to char *")
{
	std::string s("abcdef");
	metapp::Variant v(s);
	REQUIRE(v.canCast<const char *>());
	REQUIRE(v.canCast<char *>());
	REQUIRE(strcmp(v.cast<const char *>().get<const char *>(), s.c_str()) == 0);
	REQUIRE(strcmp(v.cast<char *>().get<char *>(), s.c_str()) == 0);
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

TEST_CASE("Variant, cast std::wstring to wchar_t *")
{
	std::wstring ws(L"abcdef");
	metapp::Variant v(ws);
	REQUIRE(! v.canCast<const char *>());
	REQUIRE(v.canCast<const wchar_t *>());
	REQUIRE(v.canCast<wchar_t *>());
	REQUIRE(wcscmp(v.cast<const wchar_t *>().get<const wchar_t *>(), ws.c_str()) == 0);
	REQUIRE(wcscmp(v.cast<wchar_t *>().get<wchar_t *>(), ws.c_str()) == 0);
}

TEST_CASE("Variant, cast Derived * to MyClass *")
{
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

TEST_CASE("Variant, cast Derived & to MyClass &")
{
	Derived derived;
	metapp::Variant v(metapp::Variant::create<Derived &>(derived));
	REQUIRE(! v.canCast<MyClass *>());
	REQUIRE(v.canCast<MyClass &>());
	REQUIRE(metapp::getTypeKind(v) == metapp::tkReference);
	REQUIRE(v.getMetaType()->getUpType() == metapp::getMetaType<Derived>());

	metapp::Variant casted = v.cast<MyClass &>();
	REQUIRE(metapp::getTypeKind(casted) == metapp::tkReference);
	REQUIRE(casted.getMetaType()->getUpType() == metapp::getMetaType<MyClass>());
}

struct ImplictConstruct
{
	ImplictConstruct(const std::string & text) : text(text) {}

	std::string text;
};

TEST_CASE("Variant, implicitly cast std::string to ImplictConstruct")
{
	metapp::Variant v(std::string("abc"));
	REQUIRE(v.canCast<ImplictConstruct>());
	REQUIRE(v.cast<ImplictConstruct>().get<ImplictConstruct &>().text == "abc");
}

struct ImplicitTypeCast
{
	int value;

	operator int () const {
		return value;
	}
};

TEST_CASE("Variant, implicitly cast ImplicitTypeCast to int")
{
	ImplicitTypeCast xxx {5};
	int n = xxx;
	REQUIRE(n == 5);
	metapp::Variant v(ImplicitTypeCast {38});
	v.canCast<int>();
	REQUIRE(v.canCast<int>());
	REQUIRE(v.cast<int>().get<int>() == 38);
}


} // namespace
