#ifndef METAPP_METAMAP_H_969872685611
#define METAPP_METAMAP_H_969872685611

#include "metapp/variant.h"

namespace metapp {

class MetaMap
{
public:
	MetaMap(
		Variant (*get)(const Variant & var, const Variant & key),
		void (*set)(const Variant & var, const Variant & key, const Variant & value)
	)
		: get(get), set(set)
	{
	}

	Variant (*get)(const Variant & var, const Variant & key);
	void (*set)(const Variant & var, const Variant & key, const Variant & value);
};


} // namespace metapp

#endif
