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

#ifndef METAPP_METATYPEDUMPER_H_969872685611
#define METAPP_METATYPEDUMPER_H_969872685611

#include "metapp/metatype.h"
#include "metapp/metarepo.h"

#include <ostream>

namespace metapp {

class MetaTypeDumper
{
public:
	explicit MetaTypeDumper(const MetaRepo * metaRepository_ = nullptr)
		: metaRepo(metaRepository_) {
		if(metaRepo == nullptr) {
			metaRepo = getMetaRepo();
		}
	}

	void dump(std::ostream & stream, const metapp::MetaType * metaType) {
		doDump(stream, metaType, 0);
	}

private:
	void doDump(std::ostream & stream, const metapp::MetaType * metaType, const int level) {
		if(metaType == nullptr) {
			return;
		}
		doDumpIndent(stream, level);
		stream << "Type: " << metaType->getTypeKind();
		std::string name = metaRepo->getType(metaType).getName();
		if(name.empty()) {
			name = metapp::getNameByTypeKind(metaType->getTypeKind());
		}
		if(! name.empty()) {
			stream << "(" << name << ")";
		}
		if(metaType->isConst() || metaType->isVolatile()) {
			stream << ", CV:";
			if(metaType->isConst()) {
				stream << " const";
			}
			if(metaType->isVolatile()) {
				stream << " volatile";
			}
		}
		doDumpUpType(stream, metaType, level);
	};

	void doDumpUpType(std::ostream & stream, const metapp::MetaType * metaType, const int level) {
		const size_t upCount = metaType->getUpTypeCount();
		stream << ", UpCount: " << upCount;
		stream << std::endl;
		for(size_t i = 0; i < upCount; ++i) {
			doDump(stream, metaType->getUpType(i), level + 1);
		}
	}

	void doDumpIndent(std::ostream & stream, int level) {
		while(level-- > 0) {
			stream << "  ";
		}
	};

private:
	const MetaRepo * metaRepo;
};


} // namespace metapp

#endif
