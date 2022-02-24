#ifndef METAACCESSIBLE_H_969872685611
#define METAACCESSIBLE_H_969872685611

#include "metapp/variant.h"

namespace metapp {

class MetaAccessible
{
public:
	MetaAccessible(
		Variant (*accessibleGet)(const Variant & accessible, const void * instance),
		void (*accessibleSet)(const Variant & accessible, void * instance, const Variant & value)
	)
		:
			accessibleGet(accessibleGet),
			accessibleSet(accessibleSet)
	{}

	Variant (*accessibleGet)(const Variant & accessible, const void * instance);
	void (*accessibleSet)(const Variant & accessible, void * instance, const Variant & value);
};


} // namespace metapp


#endif
