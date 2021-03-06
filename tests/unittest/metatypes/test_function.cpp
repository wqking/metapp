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

void firstFunc() {}

TEST_CASE("metatypes, tkFunction, construct")
{
	using FT = void ();

	SECTION("Variant, FT") {
		metapp::Variant v(metapp::getMetaType<FT>(), nullptr);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkFunction);
		REQUIRE(v.canGet<FT>());
		REQUIRE(v.get<FT>() == nullptr);
		// function type is always decayed to function pointer, so FT is not much different with FT *
		REQUIRE(v.canGet<FT *>());
		REQUIRE(v.get<FT *>() == nullptr);
	}

	SECTION("Variant, FT *") {
		metapp::Variant v(metapp::getMetaType<FT *>(), nullptr);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkFunction);
		REQUIRE(v.canGet<FT>());
		REQUIRE(v.get<FT>() == nullptr);
		REQUIRE(v.canGet<FT *>());
		REQUIRE(v.get<FT *>() == nullptr);
	}

	SECTION("Variant, FT, copy from firstFunc") {
		// Note here is address of firstFunc, not address of address of firstFunc.
		// This is because we are creating Variant of type `FT`, so the second argument should
		// be pointer to FT.
		// The equivalent pseudo C++ code is `FT v = firstFunc;`, assume firstFunc is not decayed to function pointer,
		// note the pseudo code is not valid C++.
		metapp::Variant v(metapp::getMetaType<FT>(), (const void *)&firstFunc);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkFunction);
		REQUIRE(v.canGet<FT>());
		REQUIRE(v.get<FT>() == &firstFunc);
		REQUIRE(v.canGet<FT *>());
		REQUIRE(v.get<FT *>() == &firstFunc);
	}

	SECTION("Variant, FT *, copy from firstFunc") {
		FT * funcPtr = &firstFunc;
		// We are creating Variant of type `FT *`, so the second argument should
		// be pointer to address of FT.
		// The equivalent pseudo C++ code is `FT * v = &firstFunc;`.
		metapp::Variant v(metapp::getMetaType<FT *>(), &funcPtr);
		REQUIRE(metapp::getTypeKind(v) == metapp::tkFunction);
		REQUIRE(v.canGet<FT>());
		REQUIRE(v.get<FT>() == funcPtr);
		REQUIRE(v.canGet<FT *>());
		REQUIRE(v.get<FT *>() == funcPtr);
	}

	SECTION("Heap, FT") {
		auto metaType = metapp::getMetaType<FT>();
		void * ft = metaType->construct();
		REQUIRE(*static_cast<FT **>(ft) == nullptr);
		metaType->destroy(ft);
	}

	SECTION("Heap, FT *") {
		auto metaType = metapp::getMetaType<FT *>();
		void * ft = metaType->construct();
		REQUIRE(*static_cast<FT **>(ft) == nullptr);
		metaType->destroy(ft);
	}

	SECTION("Heap, FT, copy from firstFunc") {
		auto metaType = metapp::getMetaType<FT>();
		void * ft = metaType->copyConstruct((void *)&firstFunc);
		REQUIRE(*static_cast<FT **>(ft) == &firstFunc);
		metaType->destroy(ft);
	}

	SECTION("Heap, FT *, copy from firstFunc") {
		FT * funcPtr = &firstFunc;
		auto metaType = metapp::getMetaType<FT *>();
		void * ft = metaType->copyConstruct(&funcPtr);
		REQUIRE(*static_cast<FT **>(ft) == &firstFunc);
		metaType->destroy(ft);
	}

	SECTION("Heap, FT, placement construct") {
		FT * memory = &firstFunc;
		REQUIRE(memory != nullptr);
		auto metaType = metapp::getMetaType<FT>();
		void * ft = metaType->placementConstruct(&memory);
		REQUIRE(memory == nullptr);
		metaType->dtor(ft);
	}

	SECTION("Heap, FT *, placement construct") {
		FT * memory = &firstFunc;
		REQUIRE(memory != nullptr);
		auto metaType = metapp::getMetaType<FT *>();
		void * ft = metaType->placementConstruct(&memory);
		REQUIRE(memory == nullptr);
		metaType->dtor(ft);
	}

	SECTION("Heap, FT, placement copy construct") {
		FT * memory = nullptr;
		REQUIRE(memory == nullptr);
		auto metaType = metapp::getMetaType<FT>();
		void * ft = metaType->placementCopyConstruct(&memory, (void *)&firstFunc);
		REQUIRE(memory == &firstFunc);
		metaType->dtor(ft);
	}

	SECTION("Heap, FT *, placement copy construct") {
		FT * funcPtr = &firstFunc;
		FT * memory = nullptr;
		REQUIRE(memory == nullptr);
		auto metaType = metapp::getMetaType<FT *>();
		void * ft = metaType->placementCopyConstruct(&memory, &funcPtr);
		REQUIRE(memory == &firstFunc);
		metaType->dtor(ft);
	}

}

const void * func1(int, const std::vector<int> &) { return nullptr; }

TEST_CASE("metatypes, tkFunction, free function, types")
{
	REQUIRE(metapp::getMetaType<void ()>()->getTypeKind() == metapp::tkFunction);

	metapp::Variant v(&func1);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkFunction);
	REQUIRE(v.get<void *>() == (void *)&func1);
	REQUIRE(v.canGet<const void *(int, const std::vector<int> &)>());
	REQUIRE(v.get<const void *(int, const std::vector<int> &)>() == &func1);

	using namespace metapp;
	auto metaType = v.getMetaType();
	REQUIRE(metaType->getMetaCallable()->getParameterCountInfo(v).getMinParameterCount() == 2);
	REQUIRE(metaType->getMetaCallable()->getReturnType(v)->equal(metapp::getMetaType<void *>()));
	REQUIRE(metaType->getMetaCallable()->getParameterType(v, 0)->equal(metapp::getMetaType<int>()));
	REQUIRE(metaType->getMetaCallable()->getParameterType(v, 1)->equal(metapp::getMetaType<std::vector<int> &>()));
}

void myFunc(int & a, std::string & b)
{
	a = 15;
	b = "world";
}

std::string myFunc2(const int a, const metapp::Variant & b)
{
	return b.cast<const std::string &>().get<const std::string &>()
		+ std::to_string(a)
		+ (b.getMetaType()->isReference() ? "R" : "NR")
	;
}

int myFunc3()
{
	return 38;
}

TEST_CASE("metatypes, tkFunction, free function, invoke")
{
	{
		metapp::Variant v(&myFunc);
		int a = 0;
		std::string b;
		metapp::Variant arguments[] = { metapp::Variant::reference(a), metapp::Variant::reference(b) };
		REQUIRE(v.getMetaType()->getMetaCallable()->canInvoke(v, nullptr, { arguments, 2 }));
		REQUIRE(! v.getMetaType()->getMetaCallable()->canInvoke(v, nullptr, { arguments, 1 }));
		v.getMetaType()->getMetaCallable()->invoke(v, nullptr, { arguments, 2 });
		REQUIRE(a == 15);
		REQUIRE(b == "world");
	}
	{
		metapp::Variant v(&myFunc2);
		metapp::Variant arguments[] = { 5, "hello" };
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(v, nullptr, { arguments, 2 }).get<std::string>() == "hello5NR");
		// metapp::callableInvoke passes Variant of reference to any argument of type Variant
		REQUIRE(metapp::callableInvoke(v, nullptr, 6, "world").get<std::string>() == "world6R");
	}
	{
		metapp::Variant v(&myFunc3);
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(v, nullptr, {}).get<int>() == 38);
		REQUIRE(metapp::callableInvoke(v, nullptr).get<int>() == 38);
		REQUIRE_THROWS(metapp::callableInvoke(v, nullptr, 1));
	}
}


} // namespace
