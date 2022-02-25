#ifndef STD_PAIR_H_969872685611
#define STD_PAIR_H_969872685611

#include "metapp/metatype.h"

#include <utility>

namespace metapp {

template <typename T1, typename T2>
struct DeclareMetaType <std::pair<T1, T2> >
	: public DeclareMetaTypeBase <std::pair<T1, T2> >
{
	using UpType = TypeList<T1, T2>;
	static constexpr TypeKind typeKind = tkStdPair;

};


} // namespace metapp


#endif

