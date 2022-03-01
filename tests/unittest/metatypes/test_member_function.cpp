#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"
#include "metapp/utilities/inspectors.h"

namespace {

struct Class1
{
	const void * func(int, const std::vector<int> &) { return nullptr; }
	const std::array<int, 5> * data;
};

} // namespace

namespace metapp {
template <>
struct DeclareMetaType <Class1> : public DeclareMetaTypeBase<Class1>
{
	static constexpr TypeKind typeKind = 2000;
};
} // namespace metapp

namespace {

TEST_CASE("metatypes, member function, types")
{
	metapp::Variant v(&Class1::func);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkMemberFunction);
	
	using namespace metapp;
	auto metaType = v.getMetaType();
	metapp::CallableInspector inspector(metaType);
	REQUIRE(inspector.getClassType() != nullptr);
	REQUIRE(inspector.getClassType()->getTypeKind() == 2000);
	REQUIRE(inspector.getParamCount() == 2);
	REQUIRE(matchUpTypeKinds(inspector.getReturnType(), { tkPointer, tkVoid }));
	REQUIRE(matchUpTypeKinds(inspector.getParamType(0), { tkInt }));
	REQUIRE(matchUpTypeKinds(inspector.getParamType(1), { tkReference, tkStdVector, tkInt }));
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

TEST_CASE("metatypes, member function, invoke")
{
	SECTION("Non-virtual") {
		metapp::Variant v(&Base::func1);
		int a = 0;
		std::string b;
		metapp::Variant arguments[] = { metapp::Variant().set<int &>(a), metapp::Variant().set<std::string &>(b) };
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