#ifndef METAPP_METAINDEXABLEBASE_H_969872685611
#define METAPP_METAINDEXABLEBASE_H_969872685611

#include "metapp/interfaces/metaindexable.h"
#include "metapp/metatype.h"
#include "metapp/exception.h"
#include "metapp/utils/utility.h"

namespace metapp {

template <typename ContainerType>
struct MetaIndexableBase
{
	static const MetaIndexable * getMetaIndexable() {
		static MetaIndexable metaIndexable(
			&metaIndexableGetSize,
			&metaIndexableGet,
			&metaIndexableSet
		);
		return &metaIndexable;
	}

private:
	using ValueType = decltype(std::declval<ContainerType &>()[0]);

	static size_t metaIndexableGetSize(const Variant & var)
	{
		return var.toReference().get<ContainerType &>().size();
	}

	static Variant metaIndexableGet(const Variant & var, const size_t index)
	{
		return Variant::create<ValueType>(var.toReference().get<ContainerType &>()[index]);
	}

	static void metaIndexableSet(const Variant & var, const size_t index, const Variant & value)
	{
		const Variant ref = var.toReference();

		verifyVariantWritable(ref);

		if(index >= metaIndexableGetSize(ref)) {
			errorInvalidIndex();
		}
		else {
			assignValue(ref.toReference().get<ContainerType &>()[index], value.get<ValueType &>());
		}
	}

};


} // namespace metapp


#endif

