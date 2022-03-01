#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

namespace {

struct Base
{
	Base() = default;
	Base(const int n, int * ctorCounter) : myValue(n), ctorCounter(ctorCounter) {
		++*ctorCounter;
	}
	~Base() {
		myValue = 0;
		--*ctorCounter;
	}

	int myValue;
	int * ctorCounter;

};

TEST_CASE("metatypes, constructor")
{
	int ctorCounter = 0;
	metapp::Variant ctor = metapp::Constructor<Base (int, int *)>();
	metapp::Variant arguments[] = { 7, &ctorCounter };
	SECTION("free by Variant") {
		{
			metapp::Variant obj = metapp::Variant().makeObject(
				ctor.getMetaType()->getMetaCallable()->invoke(metapp::Variant(), nullptr, arguments, 2)
			);
			REQUIRE(ctorCounter == 1);
			Base & base = obj.get<Base &>();
			REQUIRE(obj.getMetaType() == metapp::getMetaType<Base>());
			REQUIRE(base.myValue == 7);
			REQUIRE(static_cast<const Base *>(obj.getAddress())->myValue == 7);
		}
		REQUIRE(ctorCounter == 0);
	}
	SECTION("free by delete") {
		Base * base = ctor.getMetaType()->getMetaCallable()->invoke(metapp::Variant(), nullptr, arguments, 2).get<Base *>();
		REQUIRE(base->myValue == 7);
		REQUIRE(ctorCounter == 1);
		delete base;
		REQUIRE(ctorCounter == 0);
	}
}


} // namespace
