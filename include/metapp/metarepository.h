#ifndef METAREPOSITORY_H_969872685611
#define METAREPOSITORY_H_969872685611

#include "metapp/metatypes/metatypes.h"
#include "metapp/internal/metarepobase_i.h"

namespace metapp {

class MetaRepository : public internal_::MetaRepoBase
{
public:
private:
};

MetaRepository * getMetaRepository();

} // namespace metapp

#include "metapp/implement/metarepository_impl.h"

#endif
