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

#ifndef METAPP_DISJOINTVIEW_I_H_969872685611
#define METAPP_DISJOINTVIEW_I_H_969872685611

#include <array>
#include <vector>
#include <iterator>

namespace metapp {

namespace internal_ {

template <typename ValueType, typename Container, int smallListSize = 4>
class DisjointView
{
private:
	static_assert(smallListSize > 0, "Invalid smallListSize");

	struct ContainerItem
	{
		const Container * container;
		int totalSize;
	};

public:
	class Iterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = const ValueType;
		using pointer = const value_type *;
		using reference = const value_type &;

	public:
		Iterator(const DisjointView * view, const int listIndex, const int itemIndex)
			: view(view), listIndex(listIndex), itemIndex(itemIndex)
		{}

		reference operator * () const {
			return view->listPointer[listIndex].container->at(itemIndex);
		}

		pointer operator -> () const {
			return &view->listPointer[listIndex].container->at(itemIndex);
		}

		Iterator & operator ++ () {
			++itemIndex;
			if(itemIndex >= (int)view->listPointer[listIndex].container->size()) {
				itemIndex = 0;
				++listIndex;
			}
			return *this;
		}  

		Iterator operator ++ (int) {
			Iterator temp = *this;
			++(*this);
			return temp;
		}

		friend bool operator == (const Iterator & a, const Iterator & b) {
			return a.listIndex == b.listIndex && a.itemIndex == b.itemIndex;
		};

		friend bool operator != (const Iterator & a, const Iterator & b) {
			return ! operator == (a, b);
		};  

	private:
		const DisjointView * view;
		int listIndex;
		int itemIndex;
	};

	using value_type = const ValueType;
	using size_type = std::size_t;
	using difference_type = typename Iterator::difference_type;
	using reference = const value_type &;
	using const_reference = const value_type &;
	using pointer = const value_type *;
	using const_pointer = const value_type *;
	using iterator = Iterator;
	using const_iterator = Iterator;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
	DisjointView()
		:
			smallList(),
			largeList(),
			listPointer(smallList.data()),
			listCount(0)
	{
	}

	explicit DisjointView(const Container * container)
		: DisjointView()
	{
		addContainer(container);
	}

	DisjointView(const DisjointView & other)
		:
			smallList(other.smallList),
			largeList(other.largeList),
			listPointer(),
			listCount(other.listCount)
	{
		resetListPointer();
	}

	DisjointView(DisjointView && other) noexcept(
			noexcept(std::move(other.smallList))
			&& noexcept(std::move(other.largeList))
		)
		:
			smallList(std::move(other.smallList)),
			largeList(std::move(other.largeList)),
			listPointer(),
			listCount(std::move(other.listCount))
	{
		resetListPointer();
	}

	DisjointView & operator = (const DisjointView & other) {
		if(this != &other) {
			smallList = other.smallList;
			largeList = other.largeList;
			listCount = other.listCount;

			resetListPointer();
		}
		return *this;
	}

	DisjointView & operator = (DisjointView && other) noexcept {
		if(this != &other) {
			smallList = std::move(other.smallList);
			largeList = std::move(other.largeList);
			listCount = std::move(other.listCount);

			resetListPointer();
		}
		return *this;
	}

	iterator begin() const {
		return Iterator(this, 0, 0);
	}

	iterator end() const {
		return Iterator(this, listCount, 0);
	}

	bool empty() const {
		return listCount == 0;
	}

	size_type size() const {
		return listCount == 0 ? 0 : listPointer[listCount - 1].totalSize;
	}

	reference at(const size_type index) const {
		int listIndex;
		int itemIndex;
		splitIndex((int)index, &listIndex, &itemIndex);
		return listPointer[listIndex].container->at(itemIndex);
	}

	reference operator [] (const size_type index) const {
		return at(index);
	}

	void addContainer(const Container * container) {
		int totalSize = (int)container->size();
		if(totalSize == 0) {
			return;
		}
		if(listCount > 0) {
			totalSize += listPointer[listCount - 1].totalSize;
		}
		if(listCount < smallListSize) {
			smallList[listCount] = { container, totalSize };
		}
		else {
			if(listCount == smallListSize) {
				largeList.insert(largeList.end(), smallList.begin(), smallList.end());
			}
			largeList.push_back({ container, totalSize });
		}
		++listCount;
		resetListPointer(); // must be after ++listCount
	}

private:
	void splitIndex(const int index, int * listIndex, int * itemIndex) const {
		*listIndex = 0;
		*itemIndex = 0;
		while(*listIndex < listCount) {
			if(index < listPointer[*listIndex].totalSize) {
				*itemIndex = index;
				if(*listIndex > 0) {
					*itemIndex -= listPointer[*listIndex - 1].totalSize;
				}
				break;
			}
			++*listIndex;
		}
	}

	void resetListPointer() {
		listPointer = listCount <= smallListSize ? smallList.data() : largeList.data();
	}

private:
	std::array<ContainerItem, smallListSize> smallList;
	std::vector<ContainerItem> largeList;
	ContainerItem * listPointer;
	int listCount;
};

} // namespace internal_

} // namespace metapp

#endif
