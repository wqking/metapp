#ifndef METATYPE_H_969872685611
#define METATYPE_H_969872685611

#include "metapp/typekind.h"
#include "metapp/metatypedata.h"
#include "metapp/exception.h"
#include "metapp/typelist.h"
#include "metapp/internal/metatype_i.h"

#include <type_traits>
#include <initializer_list>
#include <istream>
#include <ostream>

namespace metapp {

class MetaType;

template <typename T, typename Enabled = void>
struct DeclareMetaTypeBase;

template <typename T, typename Enabled = void>
struct DeclareMetaType;

template <typename T>
const MetaType * getMetaType();

class UnifiedType
{
public:
	constexpr UnifiedType();
	constexpr UnifiedType(const TypeKind typeKind, internal_::FuncMetaMethod metaMethod);

	TypeKind getTypeKind() const;

	void constructDefault(MetaTypeData & data) const;
	void constructWith(MetaTypeData & data, const void * value) const;
	const void * getAddress(const MetaTypeData & data) const;
	bool canCast(const MetaType * toMetaType) const;
	void cast(const MetaTypeData & data, const MetaType * toMetaType, void * toData) const;
	void streamIn(std::istream & stream, MetaTypeData & data) const;
	void streamOut(std::ostream & stream, const MetaTypeData & data) const;

private:
	TypeKind typeKind;
	internal_::FuncMetaMethod metaMethod;
};

template <typename T>
struct DeclareMetaTypeRoot
{
	using UpType = internal_::NoneUpType;

	static constexpr TypeFlags typeFlags = 0;

	static const void * getAddress(const MetaTypeData & /*data*/) {
		return nullptr;
	}

	static bool canCast(const MetaType * toMetaType) {
		return isPossibleSame(getMetaType<T>(), toMetaType, true);
	}

	static void cast(const MetaTypeData & data, const MetaType * /*toMetaType*/ , void * toData) {
		const void * value = getMetaType<T>()->getAddress(data);
		using U = typename std::remove_reference<T>::type;
		doCast((const U *)value, (U *)toData);
	}

	static void streamIn(std::istream & /*stream*/, MetaTypeData & /*data*/) {
		errorNoStreamIn();
	}

	static void streamOut(std::ostream & /*stream*/, const MetaTypeData & /*data*/) {
		errorNoStreamOut();
	}

private:
	template <typename U>
	static void doCast(const U * /*value*/, U * /*toData*/, typename std::enable_if<std::is_void<U>::value>::type * = nullptr) {
	}

	template <typename U>
	static void doCast(const U * value , U * toData, typename std::enable_if<! std::is_void<U>::value>::type * = nullptr) {
		*toData = *value;
	}
};

class MetaType
{
public:
	constexpr MetaType(
		const UnifiedType * unifiedType,
		const internal_::UpTypeData & upTypeData,
		const TypeFlags typeFlags
	);

	const UnifiedType * getUnifiedType() const;

	const MetaType * getUpType() const;
	const MetaType * getUpType(const size_t i) const;
	size_t getUpTypeCount() const;

	TypeKind getTypeKind() const;

	bool isConst() const;
	bool isVolatile() const;
	bool isPodStorage() const;

	void constructDefault(MetaTypeData & data) const;
	void constructWith(MetaTypeData & data, const void * value) const;
	const void * getAddress(const MetaTypeData & data) const;
	bool canCast(const MetaType * toMetaType) const;
	void cast(const MetaTypeData & data, const MetaType * toMetaType, void * toData) const;
	void streamIn(std::istream & stream, MetaTypeData & data) const;
	void streamOut(std::ostream & stream, const MetaTypeData & data) const;

private:
	const UnifiedType * unifiedType;
	internal_::UpTypeData upTypeData;
	TypeFlags typeFlags;
};

template <typename T, typename Enabled>
struct DeclareMetaType : public DeclareMetaTypeBase<T>
{
};

template <typename T>
const UnifiedType * getUnifiedType()
{
	using M = DeclareMetaType<T>;

	static const UnifiedType unifiedType (
		M::typeKind,
		&internal_::commonMetaMethod<M>
	);
	return &unifiedType;
}

template <typename T>
const MetaType * getMetaType();

template <typename T>
struct DeclarePodMetaType : public DeclareMetaTypeRoot<T>
{
	static constexpr TypeFlags typeFlags = tfPodStorage;

	static void constructDefault(MetaTypeData & data) {
		data.podAs<T>() = T();
	}

	static void constructWith(MetaTypeData & data, const void * value) {
		data.podAs<T>() = *(T *)value;
	}

	static const void * getAddress(const MetaTypeData & data) {
		return &data.podAs<T>();
	}

	static void streamIn(std::istream & stream, MetaTypeData & data) {
		podStreamIn<T>(stream, data);
	}

	static void streamOut(std::ostream & stream, const MetaTypeData & data) {
		podStreamOut<T>(stream, data);
	}

};

template <typename T>
struct DeclareObjectMetaType : public DeclareMetaTypeRoot<T>
{
	static constexpr TypeKind typeKind = tkObject;

	using U = typename std::remove_reference<T>::type;

	static void constructDefault(MetaTypeData & data) {
		data.object = std::make_shared<U>();
	}

	static void constructWith(MetaTypeData & data, const void * value) {
		data.object = std::make_shared<U>(*(U *)value);
	}

	static const void * getAddress(const MetaTypeData & data) {
		return data.object.get();
	}

	static void streamIn(std::istream & stream, MetaTypeData & data) {
		objectStreamIn<T>(stream, data);
	}

	static void streamOut(std::ostream & stream, const MetaTypeData & data) {
		objectStreamOut<T>(stream, data);
	}

};

template <typename T, typename Enabled>
struct DeclareMetaTypeBase : public DeclareObjectMetaType<T>
{
};

template <>
struct DeclareMetaTypeBase <internal_::DummyEmptyType> : public DeclareMetaTypeRoot<void>
{
	static constexpr TypeKind typeKind = tkEmpty;

	static void constructDefault(MetaTypeData & /*data*/) {
	}

	static void constructWith(MetaTypeData & /*data*/, const void * /*value*/) {
	}

};

template <typename Iterator>
bool matchUpTypeKinds(const MetaType * metaType, Iterator begin, Iterator end);

template <typename T>
bool matchUpTypeKinds(const MetaType * metaType, const std::initializer_list<T> & typeKindList);

template <typename T, typename U>
bool matchUpTypeKinds(const MetaType * metaType, const U & typeKindList);

bool isPossibleSame(const MetaType * fromMetaType, const MetaType * toMetaType, const bool strictMode);

#include "metapp/implement/metatype_impl.h"

} // namespace metapp

#endif
