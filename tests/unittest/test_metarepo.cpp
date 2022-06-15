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

#include "metapp/metarepo.h"
#include "metapp/allmetatypes.h"

#include <string>
#include <iostream>
#include <climits>

namespace {

} // namespace

TEST_CASE("MetaRepo, nested repo")
{
	int value {5};

	SECTION("as value") {
		metapp::MetaRepo metaRepo;

		metapp::MetaRepo nestedMetaRepo;
		nestedMetaRepo.registerAccessible("value", &value);
		REQUIRE(! nestedMetaRepo.getAccessible("value").isEmpty());

		metaRepo.registerRepo("nested", std::move(nestedMetaRepo));
		REQUIRE(nestedMetaRepo.getAccessible("value").isEmpty());

		const metapp::MetaRepo * repo = nullptr;
		SECTION("getRepo") {
			const auto & nestedItem = metaRepo.getRepo("nested");
			REQUIRE(! nestedItem.isEmpty());
			repo = nestedItem.asMetaRepo();
		}
		SECTION("getRepoView") {
			const auto repoView = metaRepo.getRepoView();
			REQUIRE(repoView.size() == 1);
			const auto & nestedItem = *repoView.begin();
			REQUIRE(! nestedItem.isEmpty());
			repo = nestedItem.asMetaRepo();
		}
		const auto & accessibleItem = repo->getAccessible("value");
		REQUIRE(metapp::accessibleGet(accessibleItem, nullptr).get<int>() == 5);
		metapp::accessibleSet(accessibleItem, nullptr, 38.2);
		REQUIRE(metapp::accessibleGet(accessibleItem, nullptr).get<int>() == 38);
	}

	SECTION("as pointer") {
		metapp::MetaRepo metaRepo;

		metapp::MetaRepo nestedMetaRepo;
		nestedMetaRepo.registerAccessible("value", &value);

		metaRepo.registerRepo("nested", &nestedMetaRepo);

		const auto & nestedItem = metaRepo.getRepo("nested");
		REQUIRE(! nestedItem.isEmpty());
		const metapp::MetaRepo * repo = nestedItem.asMetaRepo();
		const auto & accessibleItem = repo->getAccessible("value");
		REQUIRE(metapp::accessibleGet(accessibleItem, nullptr).get<int>() == 5);
		metapp::accessibleSet(accessibleItem, nullptr, 38.2);
		REQUIRE(metapp::accessibleGet(accessibleItem, nullptr).get<int>() == 38);
	}

	SECTION("as std::shared_ptr") {
		metapp::MetaRepo metaRepo;

		std::shared_ptr<metapp::MetaRepo> nestedMetaRepo(std::make_shared<metapp::MetaRepo>());
		nestedMetaRepo->registerAccessible("value", &value);

		metaRepo.registerRepo("nested", nestedMetaRepo);

		const auto & nestedItem = metaRepo.getRepo("nested");
		REQUIRE(! nestedItem.isEmpty());
		const metapp::MetaRepo * repo = nestedItem.asMetaRepo();
		const auto & accessibleItem = repo->getAccessible("value");
		REQUIRE(metapp::accessibleGet(accessibleItem, nullptr).get<int>() == 5);
		metapp::accessibleSet(accessibleItem, nullptr, 38.2);
		REQUIRE(metapp::accessibleGet(accessibleItem, nullptr).get<int>() == 38);
	}
}


