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

TEST_CASE("metatypes, std::forward_list<std::string>")
{
	using Type = std::forward_list<std::string>;
	Type container { "perfect" };
	metapp::Variant v(container);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdForwardList);
	REQUIRE(v.get<Type>().front() == "perfect");
	v.get<Type &>().push_front("good");
	REQUIRE(v.get<Type &>().front() == "good");
	REQUIRE(v.getMetaType()->equal(metapp::getMetaType<std::forward_list<std::string> >()));
}

