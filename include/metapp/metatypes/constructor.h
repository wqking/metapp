#ifndef METAPP_CONSTRUCTOR_H_969872685611
#define METAPP_CONSTRUCTOR_H_969872685611

#include "metapp/metatype.h"
#include "metapp/variant.h"
#include "metapp/metatypes/utils/metacallablebase.h"
#include "metapp/metatypes/utils/declareutil.h"

namespace metapp {

template <typename T>
struct Constructor;

template <typename Class, typename ...Args>
struct Constructor<Class (Args...)>
{
	Class * operator() (Args ... args) {
		return new Class(args...);
	}
};

template <typename Class, typename ...Args>
struct DeclareMetaTypeBase <Constructor<Class (Args...)> >
	: public DeclareMetaTypeObject<Constructor<Class (Args...)> >,
		public MetaCallableBase<Constructor<Class (Args...)>, void, Class *, Args...>
{
public:
	using UpType = TypeList<Class, Args...>;
	static constexpr TypeKind typeKind = tkConstructor;

};


} // namespace metapp


#endif

