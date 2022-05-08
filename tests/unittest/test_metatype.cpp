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

#include <string>
#include <iostream>
#include <climits>

TEST_CASE("MetaType, CV and MetaType equivalence")
{
	REQUIRE(metapp::getMetaType<int>() == metapp::getMetaType<int>());
	REQUIRE(metapp::getMetaType<const int>() == metapp::getMetaType<const int>());
	REQUIRE(metapp::getMetaType<int *>() == metapp::getMetaType<int *>());
	REQUIRE(metapp::getMetaType<const int *>() == metapp::getMetaType<const int *>());

	REQUIRE(metapp::getMetaType<int>() != metapp::getMetaType<const int>());
	REQUIRE(metapp::getMetaType<volatile int>() != metapp::getMetaType<const int>());
	REQUIRE(metapp::getMetaType<const volatile int>() != metapp::getMetaType<const int>());
}

TEST_CASE("MetaType, equal")
{
	REQUIRE(metapp::getMetaType<int>()->equal(metapp::getMetaType<int>()));
	REQUIRE(metapp::getMetaType<int>()->equal(metapp::getMetaType<const int>()));
	REQUIRE(metapp::getMetaType<volatile int>()->equal(metapp::getMetaType<const int>()));
	REQUIRE(metapp::getMetaType<const volatile int>()->equal(metapp::getMetaType<const int>()));
}

TEST_CASE("MetaType, getUpType")
{
	SECTION("int") {
		const metapp::MetaType * metaType = metapp::getMetaType<int>();
		REQUIRE(metaType->getTypeKind() == metapp::tkInt);
		REQUIRE(metaType->getUpTypeCount() == 0);
		REQUIRE(metaType->getUpType() == nullptr);
	}

	SECTION("int *") {
		const metapp::MetaType * metaType = metapp::getMetaType<int *>();
		REQUIRE(metaType->getTypeKind() == metapp::tkPointer);
		REQUIRE(metaType->getUpTypeCount() == 1);
		REQUIRE(metaType->getUpType()->getTypeKind() == metapp::tkInt);
	}

	SECTION("std::vector<char>") {
		const metapp::MetaType * metaType = metapp::getMetaType<std::vector<char> >();
		REQUIRE(metaType->getTypeKind() == metapp::tkStdVector);
		REQUIRE(metaType->getUpTypeCount() == 1);
		REQUIRE(metaType->getUpType()->getTypeKind() == metapp::tkChar);
	}

	SECTION("std::map<int, long>") {
		const metapp::MetaType * metaType = metapp::getMetaType<std::map<int, long> >();
		REQUIRE(metaType->getTypeKind() == metapp::tkStdMap);
		REQUIRE(metaType->getUpTypeCount() == 2);
		REQUIRE(metaType->getUpType(0)->getTypeKind() == metapp::tkInt);
		REQUIRE(metaType->getUpType(1)->getTypeKind() == metapp::tkLong);
	}
}

TEST_CASE("MetaType, getTypeKind")
{
	REQUIRE(metapp::getMetaType<void>()->getTypeKind() == metapp::tkVoid);
	REQUIRE(metapp::getMetaType<bool>()->getTypeKind() == metapp::tkBool);
	REQUIRE(metapp::getMetaType<char>()->getTypeKind() == metapp::tkChar);
	REQUIRE(metapp::getMetaType<wchar_t>()->getTypeKind() == metapp::tkWideChar);
	REQUIRE(metapp::getMetaType<signed char>()->getTypeKind() == metapp::tkSignedChar);
	REQUIRE(metapp::getMetaType<unsigned char>()->getTypeKind() == metapp::tkUnsignedChar);
	REQUIRE(metapp::getMetaType<short>()->getTypeKind() == metapp::tkShort);
	REQUIRE(metapp::getMetaType<unsigned short>()->getTypeKind() == metapp::tkUnsignedShort);
	REQUIRE(metapp::getMetaType<int>()->getTypeKind() == metapp::tkInt);
	REQUIRE(metapp::getMetaType<unsigned int>()->getTypeKind() == metapp::tkUnsignedInt);
	REQUIRE(metapp::getMetaType<long>()->getTypeKind() == metapp::tkLong);
	REQUIRE(metapp::getMetaType<unsigned long>()->getTypeKind() == metapp::tkUnsignedLong);
	REQUIRE(metapp::getMetaType<long long>()->getTypeKind() == metapp::tkLongLong);
	REQUIRE(metapp::getMetaType<unsigned long long>()->getTypeKind() == metapp::tkUnsignedLongLong);

	REQUIRE(metapp::getMetaType<float>()->getTypeKind() == metapp::tkFloat);
	REQUIRE(metapp::getMetaType<double>()->getTypeKind() == metapp::tkDouble);
	REQUIRE(metapp::getMetaType<long double>()->getTypeKind() == metapp::tkLongDouble);
	
	struct MyClass {};
	REQUIRE(metapp::getMetaType<MyClass>()->getTypeKind() == metapp::tkObject);

	REQUIRE(metapp::getMetaType<int *>()->getTypeKind() == metapp::tkPointer);
	REQUIRE(metapp::getMetaType<MyClass *>()->getTypeKind() == metapp::tkPointer);
	REQUIRE(metapp::getMetaType<int &>()->getTypeKind() == metapp::tkReference);
	REQUIRE(metapp::getMetaType<MyClass &>()->getTypeKind() == metapp::tkReference);
	REQUIRE(metapp::getMetaType<int &&>()->getTypeKind() == metapp::tkReference);
	REQUIRE(metapp::getMetaType<MyClass &&>()->getTypeKind() == metapp::tkReference);
	REQUIRE(metapp::getMetaType<int (&)[]>()->getTypeKind() == metapp::tkReference);
	
	REQUIRE(metapp::getMetaType<void ()>()->getTypeKind() == metapp::tkFunction);
	REQUIRE(metapp::getMetaType<void (*)()>()->getTypeKind() == metapp::tkFunction);
	REQUIRE(metapp::getMetaType<void (MyClass::*)()>()->getTypeKind() == metapp::tkMemberFunction);
	REQUIRE(metapp::getMetaType<int MyClass::*>()->getTypeKind() == metapp::tkMemberPointer);
	REQUIRE(metapp::getMetaType<metapp::Constructor<MyClass()> >()->getTypeKind() == metapp::tkConstructor);
	REQUIRE(metapp::getMetaType<metapp::DefaultArgsFunction<void (int)> >()->getTypeKind() == metapp::tkDefaultArgsFunction);
	REQUIRE(metapp::getMetaType<metapp::VariadicFunction<void (int)> >()->getTypeKind() == metapp::tkVariadicFunction);

	REQUIRE(metapp::getMetaType<int []>()->getTypeKind() == metapp::tkArray);
	REQUIRE(metapp::getMetaType<std::string * [][3]>()->getTypeKind() == metapp::tkArray);
	REQUIRE(metapp::getMetaType<MyClass [5][3][10]>()->getTypeKind() == metapp::tkArray);
	
	enum EnumA {};
	enum class EnumB {};
	REQUIRE(metapp::getMetaType<EnumA>()->getTypeKind() == metapp::tkEnum);
	REQUIRE(metapp::getMetaType<EnumB>()->getTypeKind() == metapp::tkEnum);

	REQUIRE(metapp::getMetaType<accessorpp::Accessor<int> >()->getTypeKind() == metapp::tkAccessor);
	REQUIRE(metapp::getMetaType<metapp::Variant>()->getTypeKind() == metapp::tkVariant);
	
	REQUIRE(metapp::getMetaType<std::string>()->getTypeKind() == metapp::tkStdString);
	REQUIRE(metapp::getMetaType<std::wstring>()->getTypeKind() == metapp::tkStdWideString);
	REQUIRE(metapp::getMetaType<std::shared_ptr<int> >()->getTypeKind() == metapp::tkStdSharedPtr);
	REQUIRE(metapp::getMetaType<std::unique_ptr<int> >()->getTypeKind() == metapp::tkStdUniquePtr);
	REQUIRE(metapp::getMetaType<std::function<void ()> >()->getTypeKind() == metapp::tkStdFunction);
	
	REQUIRE(metapp::getMetaType<std::vector<int> >()->getTypeKind() == metapp::tkStdVector);
	REQUIRE(metapp::getMetaType<std::list<int> >()->getTypeKind() == metapp::tkStdList);
	REQUIRE(metapp::getMetaType<std::deque<int> >()->getTypeKind() == metapp::tkStdDeque);
	REQUIRE(metapp::getMetaType<std::array<int, 5> >()->getTypeKind() == metapp::tkStdArray);
	REQUIRE(metapp::getMetaType<std::forward_list<int> >()->getTypeKind() == metapp::tkStdForwardList);
	
	REQUIRE(metapp::getMetaType<std::stack<int> >()->getTypeKind() == metapp::tkStdStack);
	REQUIRE(metapp::getMetaType<std::stack<int, std::vector<int> > >()->getTypeKind() == metapp::tkStdStack);
	REQUIRE(metapp::getMetaType<std::queue<int> >()->getTypeKind() == metapp::tkStdQueue);
	REQUIRE(metapp::getMetaType<std::queue<int, std::vector<int> > >()->getTypeKind() == metapp::tkStdQueue);
	REQUIRE(metapp::getMetaType<std::priority_queue<int> >()->getTypeKind() == metapp::tkStdPriorityQueue);
	
	REQUIRE(metapp::getMetaType<std::map<int, std::string> >()->getTypeKind() == metapp::tkStdMap);
	REQUIRE(metapp::getMetaType<std::multimap<int, std::string> >()->getTypeKind() == metapp::tkStdMultimap);
	REQUIRE(metapp::getMetaType<std::set<std::string> >()->getTypeKind() == metapp::tkStdSet);
	REQUIRE(metapp::getMetaType<std::multiset<std::string> >()->getTypeKind() == metapp::tkStdMultiset);
	REQUIRE(metapp::getMetaType<std::unordered_map<int, std::string> >()->getTypeKind() == metapp::tkStdUnorderedMap);
	REQUIRE(metapp::getMetaType<std::unordered_multimap<int, std::string> >()->getTypeKind() == metapp::tkStdUnorderedMultimap);
	REQUIRE(metapp::getMetaType<std::unordered_set<std::string> >()->getTypeKind() == metapp::tkStdUnorderedSet);
	REQUIRE(metapp::getMetaType<std::unordered_multiset<std::string> >()->getTypeKind() == metapp::tkStdUnorderedMultiset);

	REQUIRE(metapp::getMetaType<std::pair<int, std::string> >()->getTypeKind() == metapp::tkStdPair);
	REQUIRE(metapp::getMetaType<std::tuple<int, std::string, long, double, char> >()->getTypeKind() == metapp::tkStdTuple);

#ifdef METAPP_SUPPORT_STANDARD_17
	REQUIRE(metapp::getMetaType<std::any>()->getTypeKind() == metapp::tkStdAny);
	REQUIRE(metapp::getMetaType<std::variant<int, char, double> >()->getTypeKind() == metapp::tkStdVariant);
#endif
}

TEST_CASE("MetaType, construct")
{
	SECTION("int") {
		std::unique_ptr<int> instance((int *)metapp::getMetaType<int>()->construct());
		REQUIRE(*instance == 0);
	}
	SECTION("MyClass") {
		struct MyClass
		{
			int value = 5;
		};
		std::unique_ptr<MyClass> instance((MyClass *)metapp::getMetaType<MyClass>()->construct());
		REQUIRE(instance->value == 5);
	}
}

TEST_CASE("MetaType, copyConstruct")
{
	SECTION("int") {
		int n = 38;
		std::unique_ptr<int> instance((int *)metapp::getMetaType<int>()->copyConstruct(&n));
		REQUIRE(*instance == 38);
	}
	SECTION("MyClass") {
		struct MyClass
		{
			int value;
		};
		MyClass copyFrom { 98 };
		std::unique_ptr<MyClass> instance((MyClass *)metapp::getMetaType<MyClass>()->copyConstruct(&copyFrom));
		REQUIRE(instance->value == 98);
	}
}

TEST_CASE("MetaType, destroy")
{
	struct MyClass {
		explicit MyClass(bool * freed) : freed(freed) { *freed = false; }
		~MyClass() { *freed = true; }

		bool * freed;
	};
	bool freed = true;
	REQUIRE(freed);
	void * p = new MyClass(&freed);
	REQUIRE(! freed);
	metapp::getMetaType<MyClass>()->destroy(p);
	REQUIRE(freed);
}

TEST_CASE("MetaType, addReference")
{
	REQUIRE(metapp::getMetaType<int>()->addReference() == metapp::getMetaType<int &>());
	REQUIRE(metapp::getMetaType<int &>()->addReference() == metapp::getMetaType<int &>());
	REQUIRE(metapp::getMetaType<int &&>()->addReference() == metapp::getMetaType<int &>());
	REQUIRE(metapp::getMetaType<const int>()->addReference() == metapp::getMetaType<const int &>());
	REQUIRE(metapp::getMetaType<int *>()->addReference() == metapp::getMetaType<int * &>());
}
