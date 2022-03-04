#ifndef METAPP_METAMAPBASE_H_969872685611
#define METAPP_METAMAPBASE_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metamap.h"

namespace metapp {

template <typename ContainerType>
struct MetaMapBase
{
	static const MetaMap * getMetaMap() {
		static MetaMap metaMap;
		return &metaMap;
	}

};


} // namespace metapp


#endif

