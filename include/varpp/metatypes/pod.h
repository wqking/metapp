#ifndef POD_H_969872685611
#define POD_H_969872685611

#include "varpp/metatype.h"

namespace varpp {

template <typename T>
struct DeclareMetaType <T,
	typename std::enable_if<
		std::is_class<T>::value
		&& std::is_trivial<T>::value
		&& std::is_standard_layout<T>::value
		&& sizeof(T) <= podSize
		>::type> : public DeclarePodMetaType<T>
{
	static constexpr TypeKind typeKind = tkObject;

};


} // namespace varpp


#endif

