#include "test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"
#include "metapp/metarepository.h"
#include "metapp/utils/metatypedumper.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

TEST_CASE("play camp")
{
	//metapp::MetaTypeDumper().dump(std::cout, metapp::getMetaType<std::vector<const char * volatile * (*)(int)> >());
}

// This test case is used for writting the sample code in the documentation
TEST_CASE("For writting document")
{
	SECTION("Variant::canGet") {
		int n = 5;
		metapp::Variant v1(metapp::Variant::create<int &>(n)); // reference
		assert(v1.canGet<int>()); // rule 3
		assert(v1.canGet<int &>()); // rule 1
		assert(v1.canGet<const int &>()); // rule 1
		assert(v1.get<int>() == 5);
		assert(v1.get<int &>() == 5);
		assert(v1.get<const int &>() == 5);
		assert(v1.canGet<long &>()); // rule 1
		v1.get<long &>(); // this may return wrong value, because long & is not int &

		metapp::Variant v2(38);
		assert(v2.canGet<int>()); // rule 4
		assert(v2.canGet<int &>()); // rule 3
		assert(! v2.canGet<long>()); // rule 4

		int m = 9;
		metapp::Variant v3(&m); // pointer
		assert(v3.canGet<int *>()); // rule 2
		assert(v3.canGet<const int *>()); // rule 2
		assert(! v3.canGet<int>()); // rule 2
	}

	SECTION("Variant::getAddress") {
		// value, equvalent native C++: int v1 = 5;
		metapp::Variant v1(5);
		assert(v1.get<int>() == 5);

		// equvalent native C++: *&v1 = 38;
		*(int *)v1.getAddress() = 38;
		assert(v1.get<int>() == 38);
		int n1 = 8;
		int n2 = 9;
		// pointer, equvalent native C++: int * v2 = &n1;
		metapp::Variant v2(&n1);
		assert(* v2.get<int *>() == 8);
		// equvalent native C++: *&v2 = &n2;
		*(int **)v2.getAddress() = &n2;
		assert(* v2.get<int *>() == 9);

		int m = 10;
		// reference, equvalent native C++: int & v3 = m;
		metapp::Variant v3(metapp::Variant::create<int &>(m));
		assert(m == 10);
		// equvalent native C++: *&v3 = 15;
		*(int *)v3.getAddress() = 15;
		assert(m == 15);
	}

	SECTION("Variant::toReference") {
		metapp::Variant v1(5); // value
		assert(v1.get<int>() == 5);
		metapp::Variant r1(v1.toReference());
		assert(r1.get<int &>() == 5);
		r1.get<int &>() = 38;
		assert(v1.get<int>() == 38);
		assert(r1.get<int &>() == 38);

		int n = 9;
		// pointer, points to n;
		metapp::Variant v2(&n);
		assert(n == 9);
		assert(*v2.get<int *>() == 9);
		// r2 refers to n
		metapp::Variant r2(v2.toReference()); 
		assert(r2.get<int &>() == 9);
		r2.get<int &>() = 10;
		assert(n == 10);
		assert(*v2.get<int *>() == 10);

		int m = 10;
		// reference, refers to m;
		metapp::Variant v3(metapp::Variant::create<int &>(m));
		assert(m == 10);
		assert(v3.get<int &>() == 10);
		// r3 refers to m
		metapp::Variant r3(v3.toReference()); 
		assert(r3.get<int &>() == 10);
		r3.get<int &>() = 15;
		assert(m == 15);
		assert(v3.get<int &>() == 15);
		assert(r3.get<int &>() == 15);
	}
}

} // namespace
