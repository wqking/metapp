// metapp library
// 
// Copyright (C) 2022 Wang Qi (wqking)
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef METAPP_VARIANT_INTF_H_969872685611
#define METAPP_VARIANT_INTF_H_969872685611

#include "metapp/metatypedata.h"
#include "metapp/typekind.h"
#include "metapp/implement/internal/typeutil_i.h"
#include "metapp/exception.h"

#include <type_traits>
#include <cassert>

namespace metapp {

class MetaType;

class Variant
{
public:
	template <typename T>
	static Variant create(T value,
		typename std::enable_if<
			std::is_same<Variant, typename std::remove_cv<typename std::remove_reference<T>::type>::type>::value
		>::type * = nullptr);
	template <typename T>
	static Variant create(T value,
		typename std::enable_if<
			! std::is_same<Variant, typename std::remove_cv<typename std::remove_reference<T>::type>::type>::value
		>::type * = nullptr);
	static Variant retype(const MetaType * metaType, const Variant & var);
	static Variant takeFrom(const MetaType * metaType, void * instance);
	static Variant takeFrom(const Variant & var);

	Variant() noexcept;
	~Variant() = default;

	template <typename T>
	Variant(T value);

	explicit Variant(const MetaType * metaType);
	Variant(const MetaType * metaType, const void * copyFrom);

	Variant(const Variant & other) noexcept;
	Variant(Variant && other) noexcept;

	template <typename T>
	Variant & operator = (T value);
	Variant & operator = (const Variant & other) noexcept;
	Variant & operator = (Variant && other) noexcept;

	Variant clone() const;

	template <typename T>
	bool canGet() const;
	bool canGet(const MetaType * toMetaType) const;

	template <typename T>
	auto get(
		typename std::enable_if<! std::is_same<
			typename std::remove_cv<typename std::remove_reference<T>::type>::type, Variant>::value>::type * = nullptr
	) const -> typename internal_::VariantReturnType<T>::Type;
	template <typename T>
	auto get(
		typename std::enable_if<std::is_same<
			typename std::remove_cv<typename std::remove_reference<T>::type>::type, Variant>::value>::type * = nullptr
	) const -> typename internal_::VariantReturnType<T>::Type;

	void * getAddress() const;
	Variant toReference() const;
	Variant dereference() const;

	bool canCast(const MetaType * toMetaType) const;
	template <typename T>
	bool canCast() const;

	Variant cast(const MetaType * toMetaType) const;
	template <typename T>
	Variant cast() const;

	Variant castSilently(const MetaType * toMetaType) const;
	template <typename T>
	Variant castSilently() const;

	bool isEmpty() const noexcept;

	const MetaType * getMetaType() const noexcept;

	void swap(Variant & other) noexcept;

	friend std::istream & operator >> (std::istream & stream, Variant & v);
	friend std::ostream & operator << (std::ostream & stream, const Variant & v);

private:
	const MetaType * metaType;
	MetaTypeData data;
};

void swap(Variant & a, Variant & b) noexcept;

TypeKind getTypeKind(const Variant & v);

// This is to avoid copying Variant when returning empty Variant from a function
const Variant & getEmptyVariant();

} // namespace metapp


#endif
