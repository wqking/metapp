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

#ifndef METAPP_METAREPOSITORY_H_969872685611
#define METAPP_METAREPOSITORY_H_969872685611

#include "metapp/metatypes/metatypes.h"
#include "metapp/implement/internal/metarepobase_i.h"

namespace metapp {

class MetaRepository : public internal_::MetaRepoBase
{
public:
	MetaRepository();
	~MetaRepository() = default;

	MetaRepository(const MetaRepository &) = delete;
	MetaRepository(MetaRepository &&) = delete;

	MetaRepository * addRepository(const std::string & name);

	std::vector<std::string> getRepositionNameList() const;

private:
	void registerBuiltinTypes();

private:
	std::map<std::string, std::unique_ptr<MetaRepository> > repositoryMap;
};

MetaRepository * getMetaRepository();

} // namespace metapp

#include "metapp/implement/metarepository_impl.h"

#endif
