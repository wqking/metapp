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

struct MetaMethodParam
{
	MetaMethodAction action;
	const MetaType * metaType;
	MetaTypeData * data;
	const void * constValue;
	void * writableValue;
	bool result;
	std::istream * inputStream;
	std::ostream * outputStream;
};

using FuncMetaMethod = void (*)(MetaMethodParam & param);

template <typename M>
void commonMetaMethod(MetaMethodParam & param)
{
	switch(param.action) {
	case MetaMethodAction::constructDefault:
		M::constructDefault(*(param.data));
		break;

	case MetaMethodAction::constructWith:
		M::constructWith(*(param.data), param.constValue);
		break;

	case MetaMethodAction::getAddress:
		param.constValue = M::getAddress(*(param.data));
		break;

	case MetaMethodAction::canCast:
		param.result = M::canCast(param.metaType);
		break;

	case MetaMethodAction::cast:
		M::cast(*(param.data), param.metaType, param.writableValue);
		break;

	case MetaMethodAction::streamIn:
		M::streamIn(*(param.inputStream), *(param.data));
		break;

	case MetaMethodAction::streamOut:
		M::streamOut(*(param.outputStream), *(param.data));
		break;
	}
}


} // namespace internal_


} // namespace metapp

#endif
