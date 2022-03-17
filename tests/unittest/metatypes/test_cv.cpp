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

#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

TEST_CASE("metatypes, cv, top level")
{
	const metapp::MetaType * metaType;

	metaType = metapp::getMetaType<const int>();
	REQUIRE(metaType->isConst());
	
	metaType = metapp::getMetaType<volatile int>();
	REQUIRE(metaType->isVolatile());
	
	metaType = metapp::getMetaType<const volatile int>();
	REQUIRE(metaType->isConst());
	REQUIRE(metaType->isVolatile());
}

TEST_CASE("metatypes, cv, reference")
{
	const metapp::MetaType * metaType;
	
	metaType = metapp::getMetaType<const int &>();
	REQUIRE(! metaType->isConst());
	REQUIRE(metaType->getUpType()->isConst());
	
	metaType = metapp::getMetaType<volatile int &>();
	REQUIRE(! metaType->isVolatile());
	REQUIRE(metaType->getUpType()->isVolatile());
	
	metaType = metapp::getMetaType<const volatile int &>();
	REQUIRE(! metaType->isConst());
	REQUIRE(! metaType->isVolatile());
	REQUIRE(metaType->getUpType()->isConst());
	REQUIRE(metaType->getUpType()->isVolatile());
}

