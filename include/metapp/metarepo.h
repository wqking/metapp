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

#include "metapp/implement/internal/metarepobase_i.h"
#include "metapp/implement/internal/inheritancerepo_i.h"

namespace metapp {

class MetaRepo : public internal_::MetaRepoBase, public internal_::InheritanceRepo
{
public:
	MetaRepo();
	~MetaRepo() = default;

	MetaRepo(const MetaRepo &) = delete;
	MetaRepo(MetaRepo &&) = delete;

	const MetaItem & getAccessible(const std::string & name) const;
	MetaItemView getAccessibleView() const;

	const MetaItem & getCallable(const std::string & name) const;
	MetaItemView getCallableView() const;

	const MetaItem & getType(const std::string & name) const;
	const MetaItem & getType(const TypeKind kind) const;
	const MetaItem & getType(const MetaType * metaType) const;
	MetaItemView getTypeView() const;

	MetaItem & registerRepo(const std::string & name, MetaRepo * repo = nullptr);
	const MetaItem & getRepo(const std::string & name) const;
	MetaItemView getRepoView() const;

private:
	void registerBuiltinTypes();

private:
	std::shared_ptr<ItemData> repoData;
};

MetaRepo * getMetaRepo();

} // namespace metapp


#endif
