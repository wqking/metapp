#ifndef MEMBER_DATA_H_969872685611
#define MEMBER_DATA_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <typename C, typename T>
struct DeclareMetaType <T C::*, typename std::enable_if<! std::is_function<T>::value>::type>
	: public DeclareMetaTypeBase <T C::*>
{
	using UpType = TypeList <C, T>;
	static constexpr TypeKind typeKind = tkMemberPointer;
};

} // namespace metapp


#endif

