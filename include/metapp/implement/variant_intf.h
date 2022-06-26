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

#include "metapp/variantdata.h"
#include "metapp/typekind.h"
#include "metapp/implement/internal/typeutil_i.h"
#include "metapp/exception.h"

#include <type_traits>
#include <cassert>

namespace metapp {

class MetaType;
class MetaItem;

class Variant
{
private:
	struct ConstructTag {};

public:
	template <typename T>
	static Variant create(const typename std::remove_reference<T>::type & value,
		typename std::enable_if<! internal_::IsVariant<T>::value>::type * = nullptr);
	template <typename T>
	static Variant create(typename std::remove_reference<T>::type && value,
		typename std::enable_if<! internal_::IsVariant<T>::value>::type * = nullptr);
	static Variant create(const Variant & value);

	template <typename T>
	static Variant reference(T && value);
	static Variant retype(const MetaType * metaType, const Variant & var);
	static Variant takeFrom(const MetaType * metaType, void * instance);
	static Variant takeFrom(const Variant & var);

	Variant() noexcept;
	~Variant() = default;

	template <typename T>
	Variant(T && value,
		typename std::enable_if<! internal_::IsVariant<T>::value, ConstructTag>::type = ConstructTag{});

	Variant(const MetaType * metaType, const void * copyFrom);
	Variant(const MetaType * metaType, const void * copyFrom, const CopyStrategy copyStrategy);

	Variant(const Variant & other) noexcept;
	Variant(Variant && other) noexcept;

	// Disable construct from MetaItem to avoid pass the item
	// to variant where the underlying variant in the item
	// (such as MetaItem::asCallable()) should be used
	Variant(const MetaItem &) = delete;

	template <typename T>
	auto operator = (T && value)
		-> typename std::enable_if<! internal_::IsVariant<T>::value, Variant &>::type;

	Variant & operator = (const Variant & other) noexcept;
	Variant & operator = (Variant && other) noexcept;

	const MetaType * getMetaType() const noexcept {
		return metaType;
	}

	void * getAddress() const {
		return data.getAddress();
	}

	template <typename T>
	bool canGet(typename std::enable_if<! internal_::IsVariant<T>::value>::type * = nullptr) const;
	template <typename T>
	bool canGet(typename std::enable_if<internal_::IsVariant<T>::value>::type * = nullptr) const;
	bool canGet(const MetaType * toMetaType) const;

	template <typename T>
	auto get(typename std::enable_if<! internal_::IsVariant<T>::value>::type * = nullptr) const
		-> typename internal_::VariantReturnType<T>::Type;
	template <typename T>
	auto get(typename std::enable_if<internal_::IsVariant<T>::value>::type * = nullptr) const
		-> typename internal_::VariantReturnType<T>::Type;
	template <typename T>
	auto checkedGet() const -> typename internal_::VariantReturnType<T>::Type;

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

	Variant clone() const;

	Variant & assign(const Variant & other);

	void swap(Variant & other) noexcept;

	friend std::istream & operator >> (std::istream & stream, Variant & v);
	friend std::ostream & operator << (std::ostream & stream, const Variant & v);

private:
	Variant(const MetaType * metaType, const VariantData & data);

private:
	const MetaType * metaType;
	VariantData data;
};

void swap(Variant & a, Variant & b) noexcept;

TypeKind getTypeKind(const Variant & v);

} // namespace metapp


#endif

