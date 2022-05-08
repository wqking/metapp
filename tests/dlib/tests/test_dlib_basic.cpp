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

#include "dlibtest.h"

#include "metapp/variant.h"
#include "metapp/allmetatypes.h"

#include <string>
#include <iostream>

TEST_CASE("dlib")
{
	DlibLoaderWrapper dlibLoaderWrapper;
	const LibData * libData = dlibLoaderWrapper.getLibData();
	REQUIRE(libData->strHello == "hello");
	REQUIRE(libData->var5.getMetaType()->getTypeKind() == metapp::tkInt);
	libData->var5.get<int>();
	REQUIRE(libData->var5.get<int>() == 5);
	REQUIRE(libData->var5.cast<unsigned int>().get<unsigned int>() == 5u);
}

