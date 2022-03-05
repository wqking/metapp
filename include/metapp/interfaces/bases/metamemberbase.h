#ifndef METAPP_METAMEMBERBASE_H_969872685611
#define METAPP_METAMEMBERBASE_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metamember.h"

namespace metapp {

template <typename ClassType>
struct MetaMemberBase
{
	static const MetaMember * getMetaMember() {
		static MetaMember metaMember(&metaMemberGetClassType);
		return &metaMember;
	}

private:
	static const MetaType * metaMemberGetClassType()
	{
		return getMetaType<ClassType>();
	}

};


} // namespace metapp


#endif

