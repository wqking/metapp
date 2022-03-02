#ifndef METATYPEDUMPER_H_969872685611
#define METATYPEDUMPER_H_969872685611

#include "metapp/metatype.h"
#include "metapp/metarepository.h"

#include <ostream>

namespace metapp {

class MetaTypeDumper
{
public:
	explicit MetaTypeDumper(const MetaRepository * metaRepository_ = nullptr)
		: metaRepository(metaRepository_) {
		if(metaRepository == nullptr) {
			metaRepository = getMetaRepository();
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
		std::string name;
		name = metaRepository->getNameByKind(metaType->getTypeKind());
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
	const MetaRepository * metaRepository;
};


} // namespace metapp

#endif
