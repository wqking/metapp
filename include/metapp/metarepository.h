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

	MetaRepository * addRepository(const std::string & name);

	std::vector<std::string> getRepositionNameList() const;

private:
	MetaRepository(const MetaRepository &) = delete;
	MetaRepository(MetaRepository &&) = delete;

	void registerBuiltinTypes();

private:
	std::map<std::string, std::unique_ptr<MetaRepository> > repositoryMap;
};

MetaRepository * getMetaRepository();

} // namespace metapp

#include "metapp/implement/metarepository_impl.h"

#endif
