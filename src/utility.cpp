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

#include "metapp/utilities/utility.h"
#include "metapp/typekind.h"
#include "metapp/metarepo.h"

namespace metapp {

namespace {

std::map<TypeKind, std::string> typeKindNameMap {
	{ tkVoid, "void" },
	{ tkBool, "bool" },
	{ tkChar, "char" },
	{ tkWideChar, "wchar_t" },
	{ tkSignedChar, "signed char" },
	{ tkUnsignedChar, "unsigned char" },
	{ tkShort, "short" },
	{ tkUnsignedShort, "unsigned short" },
	{ tkInt, "int" },
	{ tkUnsignedInt, "unsigned int" },
	{ tkLong, "long" },
	{ tkUnsignedLong, "unsigned long" },
	{ tkLongLong, "long long" },
	{ tkUnsignedLongLong, "unsigned long long" },
	{ tkFloat, "float" },
	{ tkDouble, "double" },
	{ tkLongDouble, "long double" },

	{ tkObject, "object" },
	{ tkPointer, "pointer" },
	{ tkReference, "reference" },
	{ tkFunction, "function" },
	{ tkMemberFunction, "member_func" },
	{ tkMemberPointer, "member_pointer" },
	{ tkConstructor, "constructor" },
	{ tkArray, "array" },
	{ tkEnum, "enum" },
	{ tkDefaultArgsFunction, "default_args_function" },
	{ tkVariadicFunction, "variadic_function" },

	{ tkStdString, "std::string" },
	{ tkStdWideString, "std::wstring" },
	{ tkStdSharedPtr, "std::shared_ptr" },
	{ tkStdFunction, "std::function" },
	{ tkStdVector, "std::vector" },
	{ tkStdList, "std::list" },
	{ tkStdDeque, "std::deque" },
	{ tkStdArray, "std::array" },
	{ tkStdForwardList, "std::forward_list" },
	{ tkStdStack, "std::stack" },
	{ tkStdQueue, "std::queue" },
	{ tkStdPriorityQueue, "std::priority_queue" },
	{ tkStdMap, "std::map" },
	{ tkStdMultimap, "std::multipmap" },
	{ tkStdSet, "std::set" },
	{ tkStdMultiset, "std::multiset" },
	{ tkStdUnorderedMap, "std::map" },
	{ tkStdUnorderedMultimap, "std::unordered_multimap" },
	{ tkStdUnorderedSet, "std::unordered_set" },
	{ tkStdUnorderedMultiset, "std::unordered_multiset" },
	{ tkStdPair, "std::pair" },
	{ tkStdTuple, "std::tuple" },
	{ tkStdAny, "std::any" },
	{ tkStdVariant, "std::variant" },
};

class MetaTypeDumper
{
public:
	explicit MetaTypeDumper(const MetaRepo * metaRepository_)
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
	}

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


} // namespace

std::string getNameByTypeKind(const TypeKind typeKind)
{
	auto it = typeKindNameMap.find(typeKind);
	if(it != typeKindNameMap.end()) {
		return it->second;
	}
	return "";
}

void dumpMetaType(std::ostream & stream, const metapp::MetaType * metaType, const MetaRepo * metaRepository)
{
	MetaTypeDumper dumper(metaRepository);
	dumper.dump(stream, metaType);
}

} // namespace metapp
