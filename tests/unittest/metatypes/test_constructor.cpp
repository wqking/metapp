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
#include "metapp/utilities/utility.h"

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
	REQUIRE(metapp::getTypeKind(ctor) == metapp::tkConstructor);

	SECTION("free by Variant") {
		{
			metapp::Variant arguments[] = { 7, &ctorCounter };
			metapp::Variant obj = metapp::Variant::takeFrom(
				ctor.getMetaType()->getMetaCallable()->invoke(ctor, nullptr, arguments, 2)
			);
			REQUIRE(ctorCounter == 1);
			Base & base = obj.get<Base &>();
			REQUIRE(obj.getMetaType() == metapp::getMetaType<Base>());
			REQUIRE(base.myValue == 7);
			REQUIRE(static_cast<const Base *>(obj.getAddress())->myValue == 7);
			REQUIRE(obj.get<const Base &>().myValue == 7);
		}
		REQUIRE(ctorCounter == 0);
	}
	SECTION("free by delete, using metapp::callableInvoke") {
		Base * base = metapp::callableInvoke(ctor, nullptr, 7, &ctorCounter).get<Base *>();
		REQUIRE(base->myValue == 7);
		REQUIRE(ctorCounter == 1);
		delete base;
		REQUIRE(ctorCounter == 0);
	}
}


} // namespace
