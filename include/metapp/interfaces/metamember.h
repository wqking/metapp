#ifndef METAPP_METAMEMBER_H_969872685611
#define METAPP_METAMEMBER_H_969872685611

namespace metapp {

class MetaType;

class MetaMember
{
public:
	explicit MetaMember(
		const MetaType * (*getClassType)()
	)
		: getClassType(getClassType)
	{
	}

	const MetaType * (*getClassType)();
};


} // namespace metapp

#endif
