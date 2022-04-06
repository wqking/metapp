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

#ifndef METAPP_REGISTEREDREPO_H_969872685611
#define METAPP_REGISTEREDREPO_H_969872685611

#include "metapp/registration/registeredannotation.h"

#include <string>
#include <deque>
#include <memory>

namespace metapp {

class MetaRepo;

class RegisteredRepo : public RegisteredAnnotation
{
private:
	struct Data
	{
		Data(const std::string & name, MetaRepo * repo)
			: name(name), repo(repo)
		{
		}

		std::string name;
		std::unique_ptr<MetaRepo> repo;
	};

public:
	static const RegisteredRepo & getEmpty()
	{
		static RegisteredRepo emptyRegisteredRepo{};
		return emptyRegisteredRepo;
	}

public:
	RegisteredRepo()
		: data()
	{
	}

	RegisteredRepo(const std::string & name, MetaRepo * repo)
		: data(std::make_shared<Data>(name, repo))
	{
	}

	const std::string & getName() const {
		return data->name;
	}

	MetaRepo * getRepo() const {
		return data->repo.get();
	}

	bool isEmpty() const {
		return ! data;
	}

private:
	std::shared_ptr<Data> data;
};

using RegisteredRepoList = std::deque<RegisteredRepo>;


} // namespace metapp


#endif
