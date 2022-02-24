#ifndef METATYPE_I_H_969872685611
#define METATYPE_I_H_969872685611

#include <type_traits>

namespace metapp {

class MetaType;
class MetaClass;

namespace internal_ {

struct NoneUpType {};

enum class ExtraInfoKind
{
	eikNone,
	eikClass,
	eikCallable,
	eikAccessible,
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
		HasFunctionGetMetaClass<T>::value
	>::type * = nullptr)
{
	return ExtraInfo {
		ExtraInfoKind::eikClass,
		(const void * (*)())&T::getMetaClass
	};
}

template <typename T>
ExtraInfo makeExtraInfo(typename std::enable_if<
		! HasFunctionGetMetaClass<T>::value
		&& HasFunctionGetMetaCallable<T>::value
	>::type * = nullptr)
{
	return ExtraInfo {
		ExtraInfoKind::eikCallable,
		(const void * (*)())&T::getMetaCallable
	};
}

template <typename T>
ExtraInfo makeExtraInfo(typename std::enable_if<
		! HasFunctionGetMetaClass<T>::value
		&& ! HasFunctionGetMetaCallable<T>::value
		&& HasFunctionGetMetaAccessible<T>::value
	>::type * = nullptr)
{
	return ExtraInfo {
		ExtraInfoKind::eikAccessible,
		(const void * (*)())&T::getMetaAccessible
	};
}

template <typename T>
ExtraInfo makeExtraInfo(typename std::enable_if<
		! HasFunctionGetMetaClass<T>::value
		&& ! HasFunctionGetMetaCallable<T>::value
		&& ! HasFunctionGetMetaAccessible<T>::value
		&& HasFunctionGetMetaArray<T>::value
	>::type * = nullptr)
{
	return ExtraInfo {
		ExtraInfoKind::eikArray,
		(const void * (*)())&T::getMetaArray
	};
}

template <typename T>
ExtraInfo makeExtraInfo(typename std::enable_if<
		! HasFunctionGetMetaClass<T>::value
		&& ! HasFunctionGetMetaCallable<T>::value
		&& ! HasFunctionGetMetaArray<T>::value
		&& ! HasFunctionGetMetaAccessible<T>::value
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
		! HasFunctionGetMetaClass<T>::value
		&& ! HasFunctionGetMetaCallable<T>::value
		&& ! HasFunctionGetMetaAccessible<T>::value
		&& ! HasFunctionGetMetaArray<T>::value
		&& ! HasFunctionGetMetaEnum<T>::value
	>::type * = nullptr)
{
	return ExtraInfo { ExtraInfoKind::eikNone, nullptr };
}

struct MetaMethodTable
{
	void * (*construct)(MetaTypeData * data, const void * copyFrom);

	void (*destroy)(void * instance);

	void * (*getAddress)(const MetaTypeData & data);

	bool (*canCast)(const MetaType * toMetaType);
	Variant (*cast)(const Variant & value, const MetaType * toMetaType);

	void (*streamIn)(std::istream & stream, Variant & value);
	void (*streamOut)(std::ostream & stream, const Variant & value);

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
