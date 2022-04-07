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

#ifndef METAPP_METAREPO_H_969872685611
#define METAPP_METAREPO_H_969872685611

#include "metapp/metatypes/metatypes.h"
#include "metapp/implement/internal/metarepobase_i.h"
#include "metapp/implement/internal/inheritancerepo_i.h"
#include "metapp/registration/registeredrepo.h"

namespace metapp {

class MetaRepo : public internal_::MetaRepoBase, public internal_::InheritanceRepo
{
public:
	MetaRepo();
	~MetaRepo() = default;

	MetaRepo(const MetaRepo &) = delete;
	MetaRepo(MetaRepo &&) = delete;

	const RegisteredField & getField(const std::string & name) const;
	const RegisteredFieldList & getFieldList() const;

	const RegisteredMethod & getMethod(const std::string & name) const;
	RegisteredMethodList getMethodList(const std::string & name) const;
	const RegisteredMethodList & getMethodList() const;

	const RegisteredType & getType(const std::string & name) const;
	const RegisteredType & getType(const TypeKind kind) const;
	const RegisteredTypeList & getTypeList() const;

	RegisteredRepo & registerRepo(const std::string & name, MetaRepo * repo = nullptr);
	const RegisteredRepo & getRepo(const std::string & name) const;
	const RegisteredRepoList & getRepoList() const;

private:
	void registerBuiltinTypes();

private:
	RegisteredRepoList repoList;
	std::map<
		std::reference_wrapper<const std::string>,
		RegisteredRepo *,
		std::less<const std::string>
	> repoMap;
};

MetaRepo * getMetaRepo();

} // namespace metapp


#endif
