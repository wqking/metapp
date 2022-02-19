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
	canInvoke,
	invoke,
	streamIn,
	streamOut,
};

struct MetaMethodTable
{
	void (*constructDefault)(MetaTypeData & data);
	void (*constructWith)(MetaTypeData & data, const void * value);

	void (*destroy)(void * instance);

	void * (*getAddress)(const MetaTypeData & data);

	bool (*canCast)(const MetaType * toMetaType);
	Variant (*cast)(const Variant & value, const MetaType * toMetaType);

	bool (*canInvoke)(const Variant * arguments, const size_t argumentCount);
	Variant (*invoke)(void * instance, const Variant & func, const Variant * arguments, const size_t argumentCount);

	void (*streamIn)(std::istream & stream, Variant & value);
	void (*streamOut)(std::ostream & stream, const Variant & value);
};

struct UpTypeData
{
	const MetaType ** upTypeList;
	uint16_t count;
};


} // namespace internal_


} // namespace metapp

#endif
