#ifndef METAPP_METAMAPBASE_H_969872685611
#define METAPP_METAMAPBASE_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metamap.h"

namespace metapp {

template <typename ContainerType>
struct MetaMapBase
{
	static const MetaMap * getMetaMap() {
		static MetaMap metaMap(
			&metaMapGet,
			&metaMapSet
		);
		return &metaMap;
	}

private:
	using KeyType = typename ContainerType::key_type;
	using MappedType = typename ContainerType::mapped_type;

	static Variant metaMapGet(const Variant & var, const Variant & key)
	{
		const auto & container = var.get<const ContainerType &>();
		auto it = container.find(key.get<const KeyType &>());
		if(it != container.end()) {
			return Variant::create<const MappedType &>(it->second);
		}
		return Variant();
	}

	static void metaMapSet(const Variant & var, const Variant & key, const Variant & value)
	{
		auto & container = var.get<ContainerType &>();
		auto it = container.find(key.get<const KeyType &>());
		if(it != container.end()) {
			assignValue(it->second, value.get<const MappedType &>());
		}
		else {
			errorUnwritable();
		}
	}

};


} // namespace metapp


#endif

