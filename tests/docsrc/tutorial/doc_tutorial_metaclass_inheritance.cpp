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

#include "tutorial.h"

/*desc
# Tutorial for using class inheritance
desc*/

#include "metapp/allmetatypes.h"
#include "metapp/interfaces/metaclass.h"
#include "metapp/metarepo.h"

#include <iostream>

/* desc
## Define the C++ classes to reflect for

Here is the class hierarchy we are going to reflect for.
desc */

//code
struct Parent1
{
	// make some data to make derived class has `this` pointer at different address
	char p1[4];
};

struct Parent2
{
	char p2[16];
};

struct Son1 : Parent1
{
	char s1[5];
};

struct Son2 : Parent1, Parent2
{
	char s2[2];
};

struct Grandson1 : Son1
{
	char g1[35];
};

struct Grandson2 : Son1, Parent2
{
	char g2[50];
};
//code

//code
/*desc
If we DeclareMetaType for any classes, we can register the base classes in `setup` function.  
`setup` is a static function in DeclareMetaType. It will be called when the meta type is used,
and it's guaranteed to be called only once.  
`setup` is a good place to register the base classes.
desc*/
template <>
struct metapp::DeclareMetaType <Son1> : metapp::DeclareMetaTypeBase <Son1>
{
	static void setup() {
		metapp::getMetaRepo()->registerBase<Son1, Parent1>();
	}
};
//code

TEST_CASE("tutorialMetaClass_inheritance")
{
	//code
	//desc If we don't DeclareMetaType any classes, or we don't want to use `setup`,
	//desc we can register the base classes anywhere in the code.
	metapp::getMetaRepo()->registerBase<Son2, Parent1, Parent2>();
	metapp::getMetaRepo()->registerBase<Grandson1, Son1>();
	metapp::getMetaRepo()->registerBase<Grandson2, Son1, Parent2>();

	auto baseTypesViewSon1 = metapp::getMetaRepo()->getBases(metapp::getMetaType<Son1>());
	ASSERT(baseTypesViewSon1.getCount() == 1);
	ASSERT(baseTypesViewSon1.get(0) == metapp::getMetaType<Parent1>());

	auto baseTypesViewGrandson2 = metapp::getMetaRepo()->getBases<Grandson2>();
	ASSERT(baseTypesViewGrandson2.getCount() == 2);
	ASSERT(baseTypesViewGrandson2.get(0) == metapp::getMetaType<Son1>());
	ASSERT(baseTypesViewGrandson2.get(1) == metapp::getMetaType<Parent2>());

	Son2 son2;
	ASSERT(metapp::getMetaRepo()->castToBase<Son2>(&son2, 0) == static_cast<Parent1 *>(&son2));
	ASSERT(metapp::getMetaRepo()->castToBase<Son2>(&son2, 1) == static_cast<Parent2 *>(&son2));

	Son1 son1;
	ASSERT(metapp::getMetaRepo()->castToDerived<Son1>(&son1, 0) == static_cast<Grandson1 *>(&son1));
	//code
}

