#ifndef METATYPE_H_969872685611
#define METATYPE_H_969872685611

#include "varpp/common.h"
#include "varpp/internal/metatype_i.h"

#include <memory>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <array>
#include <forward_list>
#include <stack>
#include <queue>

namespace varpp {

class MetaType;

template <typename T, typename Enabled = void>
struct DeclareMetaType;

using FuncConstruct = void (*)(VariantData & data, const void * value);
using FuncGetAddress = const void * (*)(const VariantData & data);
using FuncCanCast = bool (*)(const MetaType * toMetaType);
using FuncCast = void (*)(const VariantData & data, const MetaType * toMetaType, void * toData);

class MetaType
{
public:
	constexpr MetaType() :
		construct(),
		getAddress(),
		canCast(),
		cast(),
		upType(),
		typeKind(tkEmpty),
		qualifiers()
	{
	}

	constexpr MetaType(
		const MetaType * upType,
		const TypeKind typeKind,
		const QualifierKind qualifiers,
		FuncConstruct construct,
		FuncGetAddress getAddress,
		FuncCanCast canCast,
		FuncCast cast
	) :
		construct(construct),
		getAddress(getAddress),
		canCast(canCast),
		cast(cast),
		upType(upType),
		typeKind(typeKind),
		qualifiers(qualifiers)
	{
	}

	const MetaType * getUpType() const {
		return upType;
	}

	TypeKind getVarType() const {
		return typeKind;
	}

	bool isConst() const {
		return qualifiers & qkConst;
	}

	bool isVolatile() const {
		return qualifiers & qkVolatile;
	}

	FuncConstruct construct;
	FuncGetAddress getAddress;
	FuncCanCast canCast;
	FuncCast cast;

private:
	const MetaType * upType;
	TypeKind typeKind;
	QualifierKind qualifiers;
};

constexpr MetaType emptyMetaType;

template <typename M>
auto getMetaType()
	-> typename std::enable_if<std::is_void<M>::value, const MetaType *>::type
{
	return nullptr;
}

template <typename M>
auto getMetaType()
	-> typename std::enable_if<! std::is_void<M>::value, const MetaType *>::type
{
	static const MetaType metaType (
		getMetaType<typename M::UpType>(),
		M::typeKind,
		M::qualifiers,
		&M::construct,
		&M::getAddress,
		&M::canCast,
		&M::cast
	);
	return &metaType;
}

template <typename T>
struct DeclarePodMetaType : public internal_::DeclareMetaTypeBase<T>
{
	static void construct(VariantData & data, const void * value) {
		data.podAs<T>() = *(T *)value;
	}

	static const void * getAddress(const VariantData & data) {
		return &data.podAs<T>();
	}

};

template <typename T>
struct DeclareObjectMetaType : public internal_::DeclareMetaTypeBase<T>
{
	static constexpr TypeKind typeKind = tkObject;

	static void construct(VariantData & data, const void * value) {
		data.object = std::make_shared<T>(*(T *)value);
	}

	static const void * getAddress(const VariantData & data) {
		return data.object.get();
	}
};

template <typename T, typename Enabled>
struct DeclareMetaType : public DeclareObjectMetaType<T>
{
};

template <typename T>
struct DeclareMetaType <T,
	typename std::enable_if<TypeListIn<T, internal_::FundamentalTypeList>::value>::type> : public DeclarePodMetaType<T>
{
	//using CastFunc = void (*)(const VariantData & data, void * toData);
	// this array requires either c++17 or definition in souce file
	/*static constexpr std::array<CastFunc, TypeListCount<internal_::FundamentalTypeList>::value> castFunctions{
		&internal_::podCast<T, bool>,
		&internal_::podCast<T, char>, &internal_::podCast<T, wchar_t>,
		&internal_::podCast<T, signed char>, &internal_::podCast<T, unsigned char>,
		&internal_::podCast<T, short>, &internal_::podCast<T, unsigned short>,
		&internal_::podCast<T, int>, &internal_::podCast<T, unsigned int>,
		&internal_::podCast<T, long>, &internal_::podCast<T, unsigned long>,
		&internal_::podCast<T, long long>, &internal_::podCast<T, unsigned long long>,
		&internal_::podCast<T, float>, &internal_::podCast<T, double>, &internal_::podCast<T, long double>
	};*/

	static constexpr TypeKind typeKind = TypeKind(tkFundamentalBegin + TypeListIndexOf<T, internal_::FundamentalTypeList>::value);

	static bool canCast(const MetaType * toMetaType) {
		return toMetaType->getVarType() >= tkArithmeticBegin
			&& toMetaType->getVarType() <= tkArithmeticEnd;
	}

	static void cast(const VariantData & data, const MetaType * toMetaType, void * toData) {
		//castFunctions[toMetaType->getVarType() - tkArithmeticBegin](data, toData);
		switch(toMetaType->getVarType()) {
		case tkBool: internal_::podCast<T, bool>(data, toData); break;
		case tkChar: internal_::podCast<T, char>(data, toData); break;
		case tkWideChar: internal_::podCast<T, wchar_t>(data, toData); break;
		case tkSignedChar: internal_::podCast<T, signed char>(data, toData); break;
		case tkUnsignedChar: internal_::podCast<T, unsigned char>(data, toData); break;
		case tkShort: internal_::podCast<T, short>(data, toData); break;
		case tkUnsignedShort: internal_::podCast<T, unsigned short>(data, toData); break;
		case tkInt: internal_::podCast<T, int>(data, toData); break;
		case tkUnsignedInt: internal_::podCast<T, unsigned int>(data, toData); break;
		case tkLong: internal_::podCast<T, long>(data, toData); break;
		case tkUnsignedLong: internal_::podCast<T, unsigned long>(data, toData); break;
		case tkLongLong: internal_::podCast<T, long long>(data, toData); break;
		case tkUnsignedLongLong: internal_::podCast<T, unsigned long long>(data, toData); break;
		case tkFloat: internal_::podCast<T, float>(data, toData); break;
		case tkDouble: internal_::podCast<T, double>(data, toData); break;
		case tkLongDouble: internal_::podCast<T, long double>(data, toData); break;
		}
	}
};

template <>
struct DeclareMetaType <void> : public DeclarePodMetaType<void>
{
	static constexpr TypeKind typeKind = tkVoid;

	static void construct(VariantData & /*data*/, const void * /*value*/) {
	}

	static const void * getAddress(const VariantData & /*data*/) {
		return nullptr;
	}
};

template <typename T>
struct DeclareMetaType <const T> : public DeclareMetaType<T>
{
	static constexpr QualifierKind qualifiers = qkConst;
};

template <typename T>
struct DeclareMetaType <volatile T> : public DeclareMetaType<T>
{
	static constexpr QualifierKind qualifiers = qkVolatile;
};

template <typename T>
struct DeclareMetaType <const volatile T> : public DeclareMetaType<T>
{
	static constexpr QualifierKind qualifiers = qkConst | qkVolatile;
};

template <typename T>
struct DeclareMetaType <T,
	typename std::enable_if<std::is_array<T>::value>::type> : public DeclareMetaType<typename std::decay<T>::type>
{
};

template <typename T>
struct DeclareMetaType <T,
	typename std::enable_if<std::is_function<T>::value>::type> : public DeclareMetaType<typename std::decay<T>::type>
{
};

template <typename T>
struct DeclareMetaType <T *> : public DeclarePodMetaType<T *>
{
public:
	// Seems MSVC treats T as pointer, so we need to remove the pointer.
	// All we need to remove all pointers, if there are more than one.
	using UpType = DeclareMetaType<typename std::remove_pointer<T>::type>;

	static constexpr TypeKind typeKind = tkPointer;

	static bool canCast(const MetaType * toMetaType) {
		return toMetaType->getVarType() == tkPointer;
	}

	static void cast(const VariantData & data, const MetaType * /*toMetaType*/, void * toData) {
		internal_::podCast<T *, void *>(data, toData);
	}
};

template <typename T>
struct DeclareMetaType <T &> : public DeclareMetaType<T>
{
public:
	using UpType = DeclareMetaType<T>;
	static constexpr TypeKind typeKind = tkReference;

	static void construct(VariantData & data, const void * value) {
		data.podAs<T *>() = *(T **)value;
	}

	static const void * getAddress(const VariantData & data) {
		return &data.podAs<T *>();
	}

	static bool canCast(const MetaType * toMetaType) {
		return toMetaType->getVarType() == tkReference;
	}

	static void cast(const VariantData & data, const MetaType * /*toMetaType*/, void * toData) {
		internal_::podCast<T *, void *>(data, toData);
	}
};

template <>
struct DeclareMetaType <std::string> : public DeclareObjectMetaType<std::string>
{
	static constexpr TypeKind typeKind = tkString;
};

template <>
struct DeclareMetaType <std::wstring> : public DeclareObjectMetaType<std::wstring>
{
	static constexpr TypeKind typeKind = tkWideString;
};

template <>
struct DeclareMetaType <std::nullptr_t> : public DeclareMetaType<void *>
{
};

template <typename T>
struct DeclareMetaType <std::shared_ptr<T> > : public DeclareObjectMetaType<std::shared_ptr<T> >
{
public:
	using UpType = DeclareMetaType<T>;
	static constexpr TypeKind typeKind = tkSharedPtr;

	static void construct(VariantData & data, const void * value) {
		data.object = std::static_pointer_cast<void>(*(std::shared_ptr<T> *)value);
	}

	static const void * getAddress(const VariantData & data) {
		return &data.object;
	}

};

template <typename T, typename Alloc>
struct DeclareMetaType <std::vector<T, Alloc> > : public DeclareObjectMetaType<std::vector<T, Alloc> >
{
public:
	using UpType = DeclareMetaType<T>;
	static constexpr TypeKind typeKind = tkVector;

};

template <typename T, typename Alloc>
struct DeclareMetaType <std::list<T, Alloc> > : public DeclareObjectMetaType<std::list<T, Alloc> >
{
public:
	using UpType = DeclareMetaType<T>;
	static constexpr TypeKind typeKind = tkList;

};

template <typename T, typename Alloc>
struct DeclareMetaType <std::deque<T, Alloc> > : public DeclareObjectMetaType<std::deque<T, Alloc> >
{
public:
	using UpType = DeclareMetaType<T>;
	static constexpr TypeKind typeKind = tkDeque;

};

template <typename T, size_t Size>
struct DeclareMetaType <std::array<T, Size> > : public DeclareObjectMetaType<std::array<T, Size> >
{
public:
	using UpType = DeclareMetaType<T>;
	static constexpr TypeKind typeKind = tkArray;

};

template <typename T, typename Alloc>
struct DeclareMetaType <std::forward_list<T, Alloc> > : public DeclareObjectMetaType<std::forward_list<T, Alloc> >
{
public:
	using UpType = DeclareMetaType<T>;
	static constexpr TypeKind typeKind = tkForwardList;

};

template <typename T, typename Container>
struct DeclareMetaType <std::stack<T, Container> > : public DeclareObjectMetaType<std::stack<T, Container> >
{
public:
	using UpType = DeclareMetaType<T>;
	static constexpr TypeKind typeKind = tkStack;

};

template <typename T, typename Container>
struct DeclareMetaType <std::queue<T, Container> > : public DeclareObjectMetaType<std::queue<T, Container> >
{
public:
	using UpType = DeclareMetaType<T>;
	static constexpr TypeKind typeKind = tkQueue;

};

template <typename T, typename Container>
struct DeclareMetaType <std::priority_queue<T, Container> > : public DeclareObjectMetaType<std::priority_queue<T, Container> >
{
public:
	using UpType = DeclareMetaType<T>;
	static constexpr TypeKind typeKind = tkPriorityQueue;

};

inline const MetaType * getUpTypeAt(const MetaType * metaType, size_t index)
{
	while(metaType != nullptr && index > 0) {
		metaType = metaType->getUpType();
		--index;
	}
	return metaType;
}

inline std::vector<TypeKind> getUpTypeVarTypes(const MetaType * metaType)
{
	std::vector<TypeKind> result;
	result.reserve(8);
	while(metaType != nullptr) {
		result.push_back(metaType->getVarType());
		metaType = metaType->getUpType();
	}
	return result;
}


} // namespace varpp

#endif
