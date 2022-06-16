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

TEST_CASE("MetaRepoList, for loop")
{
	const metapp::MetaRepoList * metaRepoList = metapp::getMetaRepoList();
	const std::size_t existedRepoCount = std::distance(metaRepoList->begin(), metaRepoList->end());

	auto getMetaRepoListSize = [metaRepoList]() -> std::size_t {
		std::size_t count = 0;
		for(auto repo : *metaRepoList) {
			(void)repo;
			++count;
		}
		return count;
	};

	std::vector<const metapp::MetaRepo *> repoList;
	
	SECTION("number of existing repos matches") {
		for(auto repo : *metaRepoList) {
			repoList.push_back(repo);
		}
		REQUIRE(repoList.size() == existedRepoCount);
		REQUIRE(repoList.size() == getMetaRepoListSize());
	}

	SECTION("add 3 more meta repos") {
		metapp::MetaRepo metaRepo1;
		metapp::MetaRepo metaRepo2;
		metapp::MetaRepo metaRepo3;

		for(auto repo : *metaRepoList) {
			repoList.push_back(repo);
		}
		REQUIRE(repoList.size() == existedRepoCount + 3);
		REQUIRE(repoList[existedRepoCount] == &metaRepo1);
		REQUIRE(repoList[existedRepoCount + 1] == &metaRepo2);
		REQUIRE(repoList[existedRepoCount + 2] == &metaRepo3);
	}

	SECTION("add 3 more meta repos, then free middle one") {
		metapp::MetaRepo metaRepo1;
		std::unique_ptr<metapp::MetaRepo> metaRepo2(new metapp::MetaRepo());
		metapp::MetaRepo metaRepo3;

		for(auto repo : *metaRepoList) {
			repoList.push_back(repo);
		}
		REQUIRE(repoList.size() == existedRepoCount + 3);
		REQUIRE(repoList[existedRepoCount] == &metaRepo1);
		REQUIRE(repoList[existedRepoCount + 1] == metaRepo2.get());
		REQUIRE(repoList[existedRepoCount + 2] == &metaRepo3);

		repoList.clear();
		metaRepo2.reset();

		for(auto repo : *metaRepoList) {
			repoList.push_back(repo);
		}
		REQUIRE(repoList.size() == existedRepoCount + 2);
		REQUIRE(repoList[existedRepoCount] == &metaRepo1);
		REQUIRE(repoList[existedRepoCount + 1] == &metaRepo3);
	}

	SECTION("add 3 more meta repos, then free all") {
		{
			metapp::MetaRepo metaRepo1;
			metapp::MetaRepo metaRepo2;
			metapp::MetaRepo metaRepo3;

			for(auto repo : *metaRepoList) {
				repoList.push_back(repo);
			}
			REQUIRE(repoList.size() == existedRepoCount + 3);
			REQUIRE(repoList[existedRepoCount] == &metaRepo1);
			REQUIRE(repoList[existedRepoCount + 1] == &metaRepo2);
			REQUIRE(repoList[existedRepoCount + 2] == &metaRepo3);
		}
		REQUIRE(getMetaRepoListSize() == existedRepoCount);
	}
}

TEST_CASE("MetaRepoList, findMetaRepoForHierarchy")
{
	const metapp::MetaRepoList * metaRepoList = metapp::getMetaRepoList();

	struct A {};
	struct B : A {};

	REQUIRE(metaRepoList->findMetaRepoForHierarchy(metapp::getMetaType<B>()) == nullptr);
	REQUIRE(metaRepoList->findMetaRepoForHierarchy(metapp::getMetaType<A>()) == nullptr);

	{
		metapp::MetaRepo metaRepo;
		metaRepo.registerBase<B, A>();
		REQUIRE(metaRepoList->findMetaRepoForHierarchy(metapp::getMetaType<B>()) == &metaRepo);
		REQUIRE(metaRepoList->findMetaRepoForHierarchy(metapp::getMetaType<A>()) == &metaRepo);
	}

	REQUIRE(metaRepoList->findMetaRepoForHierarchy(metapp::getMetaType<B>()) == nullptr);
	REQUIRE(metaRepoList->findMetaRepoForHierarchy(metapp::getMetaType<A>()) == nullptr);
}

TEST_CASE("MetaRepoList, traverseBases")
{
	const metapp::MetaRepoList * metaRepoList = metapp::getMetaRepoList();

	std::vector<const metapp::MetaType *> metaTypeList;
	
	auto callback = [&metaTypeList](const metapp::MetaType * metaType) -> bool {
		metaTypeList.push_back(metaType);
		return true;
	};

	SECTION("Unregistered class type, callback should be called once") {
		struct A {};
		metaRepoList->traverseBases(metapp::getMetaType<A>(), callback);
		REQUIRE(metaTypeList.size() == 1);
		REQUIRE(metaTypeList[0] == metapp::getMetaType<A>());
	}

	SECTION("Registered class type") {
		struct A {};
		struct B : A {};
		struct C : A {};
		struct D : B, C {};
		metapp::MetaRepo metaRepo;
		metaRepo.registerBase<B, A>();
		metaRepo.registerBase<C, A>();
		metaRepo.registerBase<D, B, C>();

		SECTION("traverse from A") {
			metaRepoList->traverseBases(metapp::getMetaType<A>(), callback);
			REQUIRE(metaTypeList.size() == 1);
			REQUIRE(metaTypeList[0] == metapp::getMetaType<A>());
		}

		SECTION("traverse from B") {
			metaRepoList->traverseBases(metapp::getMetaType<B>(), callback);
			REQUIRE(metaTypeList.size() == 2);
			REQUIRE(metaTypeList[0] == metapp::getMetaType<B>());
			REQUIRE(metaTypeList[1] == metapp::getMetaType<A>());
		}

		SECTION("traverse from D") {
			metaRepoList->traverseBases(metapp::getMetaType<D>(), callback);
			REQUIRE(metaTypeList.size() == 4);
			REQUIRE(metaTypeList[0] == metapp::getMetaType<D>());
			REQUIRE(metaTypeList[1] == metapp::getMetaType<B>());
			REQUIRE(metaTypeList[2] == metapp::getMetaType<A>());
			REQUIRE(metaTypeList[3] == metapp::getMetaType<C>());
		}

		SECTION("traverse from D, stop at A") {
			metaRepoList->traverseBases(metapp::getMetaType<D>(), [&metaTypeList](const metapp::MetaType * metaType) -> bool {
				metaTypeList.push_back(metaType);
				return ! metaType->equal(metapp::getMetaType<A>());
			});
			REQUIRE(metaTypeList.size() == 3);
			REQUIRE(metaTypeList[0] == metapp::getMetaType<D>());
			REQUIRE(metaTypeList[1] == metapp::getMetaType<B>());
			REQUIRE(metaTypeList[2] == metapp::getMetaType<A>());
		}
	}
}
