#ifndef METAPP_INDEXABLEBASE_FUNCTION_H_969872685611
#define METAPP_INDEXABLEBASE_FUNCTION_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metaindexable.h"
#include "metapp/metatypes/utils/declareutil.h"

namespace metapp {

template <typename ContainerType>
struct IndexableBase
{
	static const MetaIndexable * getMetaIndexable() {
		static MetaIndexable metaIndexable(
			&metaIndexableGetSize,
			&metaIndexableGetAt
		);
		return &metaIndexable;
	}

private:
	static size_t metaIndexableGetSize(const Variant & value)
	{
		return value.get<ContainerType &>().size();
	}

	static Variant metaIndexableGetAt(const Variant & value, const size_t index)
	{
		using ValueType = decltype(std::declval<ContainerType &>()[0]);
		return Variant::create<ValueType>(value.get<ContainerType &>()[index]);
	}

};


} // namespace metapp


#endif

