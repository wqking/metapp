#ifndef METAACCESSIBLE_H_969872685611
#define METAACCESSIBLE_H_969872685611

#include "metapp/variant.h"

namespace metapp {

class MetaAccessible
{
public:
	MetaAccessible(
		Variant (*get)(const Variant & accessible, const void * instance),
		void (*set)(const Variant & accessible, void * instance, const Variant & value)
	)
		:
			get(get),
			set(set)
	{}

	Variant (*get)(const Variant & accessible, const void * instance);
	void (*set)(const Variant & accessible, void * instance, const Variant & value);
};


} // namespace metapp


#endif
