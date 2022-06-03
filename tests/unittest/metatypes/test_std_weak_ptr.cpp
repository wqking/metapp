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

TEST_CASE("metatypes, std::weak_ptr<int>")
{
	std::shared_ptr<int> sp = std::make_shared<int>(38);
	std::weak_ptr<int> wp(sp);
	metapp::Variant v(wp);
	REQUIRE(metapp::getTypeKind(v) == metapp::tkStdWeakPtr);
	REQUIRE(v.canGet<std::weak_ptr<int> >());
	REQUIRE(*(v.get<std::weak_ptr<int> >().lock()) == 38);
	REQUIRE(v.getMetaType()->equal(metapp::getMetaType<std::weak_ptr<int> >()));
}

TEST_CASE("metatypes, std::weak_ptr<int>, cast to std::shared_ptr")
{
	std::shared_ptr<int> sp = std::make_shared<int>(9);
	std::weak_ptr<int> wp(sp);
	metapp::Variant v(wp);
	REQUIRE(*sp == 9);
	std::shared_ptr<int> wsp = v.cast<std::shared_ptr<int> &>().get<std::shared_ptr<int> &>();
	REQUIRE(*wsp == 9);
}

