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

TEST_CASE("metatypes, tkMemberPointer, MetaAccessible, get/set")
{
	struct TestClass
	{
		TestClass() : value(), constValue(5) {}
		TestClass(const int v, const int cv) : value(v), constValue(cv) {}

		int value;
		const int constValue;
	};

	SECTION("value") {
		metapp::Variant v(&TestClass::value);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkMemberPointer);

		TestClass obj;
		REQUIRE(metapp::accessibleGet(v, &obj).get<int>() == 0);
		obj.value = 5;
		REQUIRE(metapp::accessibleGet(v, &obj).get<int>() == 5);
		metapp::accessibleSet(v, &obj, 38);
		REQUIRE(metapp::accessibleGet(v, &obj).get<int>() == 38);
	}
	
	SECTION("constValue") {
		metapp::Variant v(&TestClass::constValue);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkMemberPointer);

		TestClass obj1;
		REQUIRE(metapp::accessibleGet(v, &obj1).get<int>() == 5);
		REQUIRE_THROWS(metapp::accessibleSet(v, &obj1, 38));

		TestClass obj2 { 6, 9 };
		REQUIRE(metapp::accessibleGet(v, &obj2).get<int>() == 9);
		REQUIRE_THROWS(metapp::accessibleSet(v, &obj2, 38));
	}
	
}

TEST_CASE("metatypes, tkMemberPointer, MetaAccessible, instance constness")
{
	struct TestClass
	{
		int value;
	};

	SECTION("no cv, object") {
		metapp::Variant v(&TestClass::value);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkMemberPointer);

		TestClass obj {};
		REQUIRE(! metapp::getNonReferenceMetaType(metapp::accessibleGet(v, obj))->isConst());
	}
	
	SECTION("const, object") {
		metapp::Variant v(&TestClass::value);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkMemberPointer);

		const TestClass obj { 5 };
		REQUIRE(metapp::getNonReferenceMetaType(metapp::accessibleGet(v, metapp::Variant::create<const TestClass>(obj)))->isConst());
	}
	
	SECTION("no cv, pointer") {
		metapp::Variant v(&TestClass::value);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkMemberPointer);

		TestClass obj {};
		TestClass * ptr = &obj;
		REQUIRE(! metapp::getNonReferenceMetaType(metapp::accessibleGet(v, ptr))->isConst());
	}
	
	SECTION("const, pointer") {
		metapp::Variant v(&TestClass::value);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkMemberPointer);

		TestClass obj { 5 };
		const TestClass * ptr = &obj;
		REQUIRE(metapp::getNonReferenceMetaType(metapp::accessibleGet(v, ptr))->isConst());
	}
	
	SECTION("no cv, reference") {
		metapp::Variant v(&TestClass::value);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkMemberPointer);

		TestClass obj {};
		REQUIRE(! metapp::getNonReferenceMetaType(metapp::accessibleGet(v, metapp::Variant::reference(obj)))->isConst());
	}

	SECTION("const, reference") {
		metapp::Variant v(&TestClass::value);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkMemberPointer);

		const TestClass obj { 5 };
		REQUIRE(metapp::getNonReferenceMetaType(metapp::accessibleGet(v, metapp::Variant::reference(obj)))->isConst());
	}

}

} // namespace