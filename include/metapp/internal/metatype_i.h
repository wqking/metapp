#ifndef METATYPE_I_H_969872685611
#define METATYPE_I_H_969872685611

#include <type_traits>

namespace metapp {

class MetaType;

namespace internal_ {

struct NoneUpType {};

struct InvokeMethdTable
{
	bool (*canInvoke)(const Variant * arguments, const size_t argumentCount);
	Variant (*invoke)(void * instance, const Variant & func, const Variant * arguments, const size_t argumentCount);
};

template <typename T>
const InvokeMethdTable * getInvokeMethdTable(typename std::enable_if<HasFunctionInvoke<T>::value>::type * = nullptr)
{
	static const InvokeMethdTable invokeMethdTable {
		&T::canInvoke,
		&T::invoke
	};
	return &invokeMethdTable;
}

template <typename T>
const InvokeMethdTable * getInvokeMethdTable(typename std::enable_if<! HasFunctionInvoke<T>::value>::type * = nullptr)
{
	return nullptr;
}

struct MetaMethodTable
{
	void (*construct)(MetaTypeData & data, const void * value);

	void (*destroy)(void * instance);

	void * (*getAddress)(const MetaTypeData & data);

	bool (*canCast)(const MetaType * toMetaType);
	Variant (*cast)(const Variant & value, const MetaType * toMetaType);

	const InvokeMethdTable * invokeMethdTable;

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
