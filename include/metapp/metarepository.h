#ifndef METAREPOSITORY_H_969872685611
#define METAREPOSITORY_H_969872685611

#include "metapp/metatypes/metatypes.h"
#include "metapp/internal/metarepobase_i.h"

namespace metapp {

class MetaRepository : public internal_::MetaRepoBase
{
public:
	MetaRepository() = default;
	~MetaRepository() = default;

	MetaRepository * addRepository(const std::string & name);

	std::vector<std::string> getRepositionNameList() const;

private:
	MetaRepository(const MetaRepository &) = delete;
	MetaRepository(MetaRepository &&) = delete;

private:
	std::map<std::string, std::unique_ptr<MetaRepository> > repositoryMap;
};

MetaRepository * getMetaRepository();

} // namespace metapp

#include "metapp/implement/metarepository_impl.h"

#endif
