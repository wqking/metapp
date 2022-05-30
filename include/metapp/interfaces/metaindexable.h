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

#ifndef METAPP_METAINDEXABLE_H_969872685611
#define METAPP_METAINDEXABLE_H_969872685611

#include "metapp/variant.h"
#include "metapp/utilities/utility.h"

#include <limits>

namespace metapp {

class MetaIndexable
{
public:
	class SizeInfo
	{
	public:
		SizeInfo() : SizeInfo(0) {}
		explicit SizeInfo(const std::size_t size) : size(size), resizable(true), unknowSize(false) {}

		std::size_t getSize() const {
			return size;
		}

		bool isResizable() const {
			return resizable;
		}

		bool isUnknownSize() const {
			return unknowSize;
		}

		void setResizable(const bool value) {
			resizable = value;
		}

		void setUnknowSize(const bool value) {
			unknowSize = value;
		}

	private:
		std::size_t size;
		bool resizable;
		bool unknowSize;
	};

public:
	MetaIndexable() = delete;

	MetaIndexable(
		SizeInfo (*getSizeInfo)(const Variant & indexable),
		const MetaType * (*getValueType)(const Variant & indexable, const std::size_t index),
		void (*resize)(const Variant & indexable, const std::size_t size),
		Variant (*get)(const Variant & indexable, const std::size_t index),
		void (*set)(const Variant & indexable, const std::size_t index, const Variant & value)
	)
		:
			getSizeInfo(getSizeInfo),
			getValueType(getValueType),
			get(get),
			set(set),
			resize_(resize)
	{
	}

	SizeInfo (*getSizeInfo)(const Variant & indexable);
	const MetaType * (*getValueType)(const Variant & indexable, const std::size_t index);
	void resize(const Variant & indexable, const std::size_t size) const {
		if(resize_ != nullptr) {
			resize_(indexable, size);
		}
	}
	Variant (*get)(const Variant & indexable, const std::size_t index);
	void (*set)(const Variant & indexable, const std::size_t index, const Variant & value);

private:
	void (*resize_)(const Variant & indexable, const std::size_t size);
};

inline MetaIndexable::SizeInfo indexableGetSizeInfo(const Variant & indexable)
{
	return getNonReferenceMetaType(indexable)->getMetaIndexable()->getSizeInfo(indexable);
}

inline void indexableResize(const Variant & indexable, const std::size_t size)
{
	getNonReferenceMetaType(indexable)->getMetaIndexable()->resize(indexable, size);
}

inline Variant indexableGet(const Variant & indexable, const std::size_t index)
{
	return getNonReferenceMetaType(indexable)->getMetaIndexable()->get(indexable, index);
}

inline void indexableSet(const Variant & indexable, const std::size_t index, const Variant & value)
{
	getNonReferenceMetaType(indexable)->getMetaIndexable()->set(indexable, index, value);
}


} // namespace metapp

#endif
