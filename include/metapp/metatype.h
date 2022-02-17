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
	constexpr UnifiedType(const char * name, const TypeKind typeKind, internal_::FuncMetaMethod metaMethod) noexcept;
	~UnifiedType() = default;

	const char * getName() const noexcept;
	TypeKind getTypeKind() const noexcept;

	void constructDefault(MetaTypeData & data) const;
	void constructWith(MetaTypeData & data, const void * value) const;
	const void * getAddress(const MetaTypeData & data) const;
	bool canCast(const MetaType * toMetaType) const;
	void cast(const MetaTypeData & data, const MetaType * toMetaType, MetaTypeData * toData) const;
	void streamIn(std::istream & stream, MetaTypeData & data) const;
	void streamOut(std::ostream & stream, const MetaTypeData & data) const;

private:
	UnifiedType() = delete;
	UnifiedType(const UnifiedType &) = delete;
	UnifiedType(UnifiedType &&) = delete;

private:
	const char * name;
	TypeKind typeKind;
	internal_::FuncMetaMethod metaMethod;
};

class MetaType
{
public:
	constexpr MetaType(
		const UnifiedType * unifiedType,
		const internal_::UpTypeData & upTypeData,
		const TypeFlags typeFlags
	) noexcept;
	~MetaType() = default;

	const UnifiedType * getUnifiedType() const noexcept;

	const MetaType * getUpType() const noexcept;
	const MetaType * getUpType(const size_t i) const;
	size_t getUpTypeCount() const noexcept;

	const char * getName() const noexcept;
	TypeKind getTypeKind() const noexcept;

	bool isConst() const noexcept;
	bool isVolatile() const noexcept;
	bool isPodStorage() const noexcept;

	// meta methods
	void constructDefault(MetaTypeData & data) const;
	void constructWith(MetaTypeData & data, const void * value) const;
	const void * getAddress(const MetaTypeData & data) const;
	bool canCast(const MetaType * toMetaType) const;
	void cast(const MetaTypeData & data, const MetaType * toMetaType, MetaTypeData * toData) const;
	void streamIn(std::istream & stream, MetaTypeData & data) const;
	void streamOut(std::ostream & stream, const MetaTypeData & data) const;

private:
	MetaType() = delete;
	MetaType(const MetaType &) = delete;
	MetaType(MetaType &&) = delete;

private:
	const UnifiedType * unifiedType;
	internal_::UpTypeData upTypeData;
	TypeFlags typeFlags;
};

template <typename T>
struct DeclareMetaTypeRoot
{
	using UpType = internal_::NoneUpType;

	static constexpr TypeFlags typeFlags = 0;

	static const char * getName() {
		return "";
	}

	static const void * getAddress(const MetaTypeData & /*data*/) {
		return nullptr;
	}

	static bool canCast(const MetaType * toMetaType) {
		return isPossibleSame(getMetaType<T>(), toMetaType, true);
	}

	static void cast(const MetaTypeData & data, const MetaType * toMetaType , MetaTypeData * toData) {
		const void * value = getMetaType<T>()->getAddress(data);
		using U = typename std::remove_reference<T>::type;
		doCast((const U *)value, toMetaType, toData);
	}

	static void streamIn(std::istream & /*stream*/, MetaTypeData & /*data*/) {
		errorNoStreamIn();
	}

	static void streamOut(std::ostream & /*stream*/, const MetaTypeData & /*data*/) {
		errorNoStreamOut();
	}

private:
	template <typename U>
	static void doCast(const U * /*value*/, const MetaType * /*toMetaType*/, MetaTypeData * /*toData*/, typename std::enable_if<std::is_void<U>::value>::type * = nullptr) {
	}

	template <typename U>
	static void doCast(const U * value , const MetaType * toMetaType , MetaTypeData * toData, typename std::enable_if<! std::is_void<U>::value>::type * = nullptr) {
		toMetaType->constructWith(*toData, value);
	}
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
		M::getName(),
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

template <typename T, typename U>
void podCast(const MetaTypeData & data, const MetaType * toMetaType, MetaTypeData * toData)
{
	U value = (U)(data.podAs<T>());
	toMetaType->constructWith(*toData, &value);
}

template <typename Iterator>
bool matchUpTypeKinds(const MetaType * metaType, Iterator begin, Iterator end);

template <typename T>
bool matchUpTypeKinds(const MetaType * metaType, const std::initializer_list<T> & typeKindList);

template <typename T, typename U>
bool matchUpTypeKinds(const MetaType * metaType, const U & typeKindList);

bool isPossibleSame(const MetaType * fromMetaType, const MetaType * toMetaType, const bool strictMode);

} // namespace metapp

#include "metapp/implement/metatype_impl.h"

#endif
