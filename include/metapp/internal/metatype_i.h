#ifndef METATYPE_I_H_969872685611
#define METATYPE_I_H_969872685611

#include <type_traits>

namespace metapp {

class MetaType;
class MetaClass;

namespace internal_ {

struct NoneUpType {};

struct InvokeMethodTable
{
	size_t (*getParameterCount)();
	int (*rankInvoke)(const Variant * arguments, const size_t argumentCount);
	bool (*canInvoke)(const Variant * arguments, const size_t argumentCount);
	Variant (*invoke)(void * instance, const Variant & func, const Variant * arguments, const size_t argumentCount);
};

template <typename T>
const InvokeMethodTable * getInvokeMethdTable(typename std::enable_if<HasFunctionInvoke<T>::value>::type * = nullptr)
{
	static const InvokeMethodTable invokeMethodTable {
		&T::getParameterCount,
		&T::rankInvoke,
		&T::canInvoke,
		&T::invoke
	};
	return &invokeMethodTable;
}

template <typename T>
const InvokeMethodTable * getInvokeMethdTable(typename std::enable_if<! HasFunctionInvoke<T>::value>::type * = nullptr)
{
	return nullptr;
}

struct AccessibleMethodTable
{
	Variant (*accessibleGet)(const Variant & accessible, const void * instance);
	void (*accessibleSet)(const Variant & accessible, void * instance, const Variant & value);
};

template <typename T>
const AccessibleMethodTable * getAccessibleMethodTable(typename std::enable_if<HasFunctionAccessibleGet<T>::value>::type * = nullptr)
{
	static const AccessibleMethodTable accessibleMethodTable {
		&T::accessibleGet,
		&T::accessibleSet
	};
	return &accessibleMethodTable;
}

template <typename T>
const AccessibleMethodTable * getAccessibleMethodTable(typename std::enable_if<! HasFunctionAccessibleGet<T>::value>::type * = nullptr)
{
	return nullptr;
}

enum class ExtraInfoKind
{
	eikNone,
	eikArray,
	eikEnum,
};

struct ExtraInfo
{
	ExtraInfoKind kind;
	const void * (*getter)();
};

template <typename T>
ExtraInfo makeExtraInfo(typename std::enable_if<
		HasFunctionGetMetaArray<T>::value
	>::type * = nullptr)
{
	return ExtraInfo {
		ExtraInfoKind::eikArray,
		(const void * (*)())&T::getMetaArray
	};
}

template <typename T>
ExtraInfo makeExtraInfo(typename std::enable_if<
		! HasFunctionGetMetaArray<T>::value
		&& HasFunctionGetMetaEnum<T>::value
	>::type * = nullptr)
{
	return ExtraInfo {
		ExtraInfoKind::eikEnum,
		(const void * (*)())&T::getMetaEnum
	};
}

template <typename T>
ExtraInfo makeExtraInfo(typename std::enable_if<
		! HasFunctionGetMetaArray<T>::value
		&& ! HasFunctionGetMetaEnum<T>::value
	>::type * = nullptr)
{
	return ExtraInfo { ExtraInfoKind::eikNone, nullptr };
}

struct MetaMethodTable
{
	void * (*construct)(MetaTypeData * data, const void * copyFrom);

	void (*destroy)(void * instance);

	const MetaClass * (*getMetaClass)();

	void * (*getAddress)(const MetaTypeData & data);

	bool (*canCast)(const MetaType * toMetaType);
	Variant (*cast)(const Variant & value, const MetaType * toMetaType);

	void (*streamIn)(std::istream & stream, Variant & value);
	void (*streamOut)(std::ostream & stream, const Variant & value);

	const InvokeMethodTable * invokeMethodTable;
	const AccessibleMethodTable * accessibleMethodTable;

	ExtraInfo extraInfo;
};

struct UpTypeData
{
	const MetaType ** upTypeList;
	uint16_t count;
};

bool isPossibleSame(const MetaType * fromMetaType, const MetaType * toMetaType, const bool strictMode);

} // namespace internal_


} // namespace metapp

#endif
