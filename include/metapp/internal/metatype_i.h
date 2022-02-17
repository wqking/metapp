#ifndef METATYPE_I_H_969872685611
#define METATYPE_I_H_969872685611

#include <type_traits>

namespace metapp {

class MetaType;

namespace internal_ {

struct NoneUpType {};

enum class MetaMethodAction
{
	constructDefault,
	constructWith,
	getAddress,
	canCast,
	cast,
	streamIn,
	streamOut,
};

struct ParamConstruct
{
	MetaTypeData * data;
};

struct ParamConstructWith
{
	MetaTypeData * data;
	const void * value;
};
 
struct ParamGetAddress
{
	const MetaTypeData * data;
	const void * value;
};
 
struct ParamCanCast
{
	const MetaType * toMetaType;
	bool result;
};
 
struct ParamCast
{
	const MetaTypeData * data;
	const MetaType * toMetaType;
	MetaTypeData * toData;
};
 
struct ParamStreamIn
{
	std::istream * stream;
	MetaTypeData * data;
};
 
struct ParamStreamOut
{
	std::ostream * stream;
	const MetaTypeData * data;
};
 
struct MetaMethodParam
{
	MetaMethodAction action;
	union {
		ParamConstruct paramConstruct;
		ParamConstructWith paramConstructWith;
		ParamGetAddress paramGetAddress;
		ParamCanCast paramCanCast;
		ParamCast paramCast;
		ParamStreamIn paramStreamIn;
		ParamStreamOut paramStreamOut;
	};
};

using FuncMetaMethod = void (*)(MetaMethodParam & param);

template <typename M>
void commonMetaMethod(MetaMethodParam & param)
{
	switch(param.action) {
	case MetaMethodAction::constructDefault:
		M::constructDefault(*(param.paramConstruct.data));
		break;

	case MetaMethodAction::constructWith:
		M::constructWith(*(param.paramConstructWith.data), param.paramConstructWith.value);
		break;

	case MetaMethodAction::getAddress:
		param.paramGetAddress.value = M::getAddress(*(param.paramGetAddress.data));
		break;

	case MetaMethodAction::canCast:
		param.paramCanCast.result = M::canCast(param.paramCanCast.toMetaType);
		break;

	case MetaMethodAction::cast:
		M::cast(*(param.paramCast.data), param.paramCast.toMetaType, param.paramCast.toData);
		break;

	case MetaMethodAction::streamIn:
		M::streamIn(*(param.paramStreamIn.stream), *(param.paramStreamIn.data));
		break;

	case MetaMethodAction::streamOut:
		M::streamOut(*(param.paramStreamOut.stream), *(param.paramStreamOut.data));
		break;
	}
}

struct UpTypeData
{
	const MetaType ** upTypeList;
	uint16_t count;
};


} // namespace internal_


} // namespace metapp

#endif
