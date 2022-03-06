#ifndef METAPP_METAINDEXABLE_H_969872685611
#define METAPP_METAINDEXABLE_H_969872685611

#include "metapp/variant.h"

namespace metapp {

class MetaIndexable
{
public:
	enum { unknowSize = (size_t)-1 };

	MetaIndexable() = delete;

	MetaIndexable(
		size_t (*getSize)(const Variant & var),
		Variant (*get)(const Variant & var, const size_t index),
		void (*set)(const Variant & var, const size_t index, const Variant & value)
	)
		: getSize(getSize), get(get), set(set)
	{
	}

	size_t (*getSize)(const Variant & var);
	Variant (*get)(const Variant & var, const size_t index);
	void (*set)(const Variant & var, const size_t index, const Variant & value);

};


} // namespace metapp

#endif
