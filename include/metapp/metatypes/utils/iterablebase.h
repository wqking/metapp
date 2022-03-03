#ifndef METAPP_ITERABLEBASE_FUNCTION_H_969872685611
#define METAPP_ITERABLEBASE_FUNCTION_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metaiterable.h"
#include "metapp/metatypes/utils/declareutil.h"

namespace metapp {

template <typename ContainerType>
struct IterableBase
{
	static const MetaIterable * getMetaIterable() {
		static MetaIterable metaIterable(
			&metaIterableForEach
		);
		return &metaIterable;
	}

private:
	static void metaIterableForEach(const Variant & value, MetaIterable::Callback callback)
	{
		ContainerType & container = value.get<ContainerType &>();
		for(auto & item : container) {
			if(! callback(Variant::create<decltype(item)>(item))) {
				break;
			}
		}
	}

};


} // namespace metapp


#endif

