#ifndef METAPP_METAINDEXABLE_H_969872685611
#define METAPP_METAINDEXABLE_H_969872685611

#include "metapp/variant.h"

namespace metapp {

class MetaIndexable
{
public:
	enum { unknowSize = (size_t)-1 };

	MetaIndexable(
		size_t (*getSize)(const Variant & value),
		Variant (*getAt)(const Variant & value, const size_t index)
	)
		: getSize(getSize), getAt(getAt)
	{
	}

	size_t (*getSize)(const Variant & value);
	Variant (*getAt)(const Variant & value, const size_t index);

private:
	MetaIndexable() = delete;
};


} // namespace metapp

#endif
