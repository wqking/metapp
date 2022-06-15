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

#ifndef METAPP_METAREPO_H_969872685611
#define METAPP_METAREPO_H_969872685611

#include "metapp/metaitem.h"
#include "metapp/implement/internal/metarepobase_i.h"
#include "metapp/implement/internal/inheritancerepo_i.h"

namespace metapp {

class MetaRepoList;

namespace internal_ {

MetaRepoList * doGetMetaRepoList();

} // namespace internal_

class MetaRepo : public internal_::MetaRepoBase, public internal_::InheritanceRepo
{
public:
	MetaRepo();
	~MetaRepo();

	MetaRepo(const MetaRepo &) = default;
	MetaRepo(MetaRepo &&) = default;

	const MetaItem & getAccessible(const std::string & name) const;
	MetaItemView getAccessibleView() const;

	const MetaItem & getCallable(const std::string & name) const;
	MetaItemView getCallableView() const;

	const MetaItem & getVariable(const std::string & name) const;
	MetaItemView getVariableView() const;

	const MetaItem & getType(const std::string & name) const;
	const MetaItem & getType(const TypeKind kind) const;
	const MetaItem & getType(const MetaType * metaType) const;
	MetaItemView getTypeView() const;

	MetaItem & registerRepo(const std::string & name, Variant repo = Variant());
	const MetaItem & getRepo(const std::string & name) const;
	MetaItemView getRepoView() const;

	const MetaItem & getItem(const std::string & name) const;

private:
	std::shared_ptr<ItemData> repoData;

	// previous and next are used by MetaRepoList
	MetaRepo * previous;
	MetaRepo * next;

	friend class MetaRepoList;
};

class MetaRepoList
{
public:
	class Iterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = const MetaRepo *;
		using pointer = const value_type *;
		using reference = const value_type &;

	public:
		Iterator(const MetaRepo * repo)
			: repo(repo)
		{}

		reference operator * () const {
			return repo;
		}

		pointer operator -> () const {
			return &repo;
		}

		Iterator & operator ++ () {
			repo = repo->next;
			return *this;
		}  

		Iterator operator ++ (int) {
			Iterator temp = *this;
			++(*this);
			return temp;
		}

		friend bool operator == (const Iterator & a, const Iterator & b) {
			return a.repo == b.repo;
		};

		friend bool operator != (const Iterator & a, const Iterator & b) {
			return ! operator == (a, b);
		};  

	private:
		const MetaRepo * repo;
	};

	using value_type = const MetaRepo *;
	using size_type = int;
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
	~MetaRepoList();

	iterator begin() const {
		return Iterator(head);
	}

	iterator end() const {
		return Iterator(nullptr);
	}

	bool empty() const {
		return head == nullptr;
	}

	const MetaRepo * findMetaRepoForHierarchy(const MetaType * classMetaType) const;

	template <typename FT>
	bool traverseBases(
		const MetaType * classMetaType,
		FT && callback) const
	{
		const MetaRepo * repo = findMetaRepoForHierarchy(classMetaType);
		if(repo != nullptr) {
			return repo->traverseBases(classMetaType, std::forward<FT>(callback));
		}
		else {
			// callback is always called with metaType, even there is no base classes.
			return callback(classMetaType);
		}
	}

private:
	MetaRepoList();

	void addMetaRepo(MetaRepo * repo);
	void removeMetaRepo(MetaRepo * repo);

private:
	MetaRepo * head;
	MetaRepo * tail;

	friend class MetaRepo;
	friend MetaRepoList * internal_::doGetMetaRepoList();
};

const MetaRepoList * getMetaRepoList();

} // namespace metapp


#endif
