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

#ifndef CONTAINERDATAPROVIDER_H
#define CONTAINERDATAPROVIDER_H

#include "metapp/compiler.h"
#include "metapp/metatype.h"
#include "metapp/typekind.h"
#include "metapp/utils/typelist.h"

#include <array>
#include <deque>
#include <vector>
#include <initializer_list>

#ifdef METAPP_COMPILER_VC
#pragma warning(push)
#pragma warning(disable: 4310) // cast truncates constant value
#endif

template <typename Container>
class TestContainerDataProviderBase
{
private:
	using ValueType = typename Container::value_type;

public:
	TestContainerDataProviderBase(
			const metapp::TypeKind typeKind,
			const metapp::TypeKind valueTypeKind,
			const Container & container
		)
		: typeKind(typeKind), valueTypeKind(valueTypeKind), container(container)
	{
	}

	metapp::TypeKind getTypeKind() const {
		return typeKind;
	}

	metapp::TypeKind getValueTypeKind() const {
		return valueTypeKind;
	}

	/*
	size_t getSize() const {
		return container.size();
	}

	ValueType get(const size_t index) const {
		return container.at(index);
	}
	*/

	const Container & getContainer() const {
		return container;
	}

	Container & getContainer() {
		return container;
	}

private:
	metapp::TypeKind typeKind;
	metapp::TypeKind valueTypeKind;
	Container container;
};

template <typename T>
class TestContainerDataProvider;

template <>
class TestContainerDataProvider<std::array<int, 5> >
	: public TestContainerDataProviderBase<std::array<int, 5> >
{
private:
	using super = TestContainerDataProviderBase<std::array<int, 5> >;

public:
	TestContainerDataProvider()
		: super(
			metapp::tkStdArray,
			metapp::tkInt,
			{
				-5, 0, 123456789, 38, -98765, 
			}
			)
	{
	}
};

template <>
class TestContainerDataProvider<std::array<std::string, 5> >
	: public TestContainerDataProviderBase<std::array<std::string, 5> >
{
private:
	using super = TestContainerDataProviderBase<std::array<std::string, 5> >;

public:
	TestContainerDataProvider()
		: super(
			metapp::tkStdArray,
			metapp::tkStdString,
			{
				"hello", "", "world", "This is great!", "Very good"
			}
		)
	{
	}
};

template <>
class TestContainerDataProvider<std::vector<int> >
	: public TestContainerDataProviderBase<std::vector<int> >
{
private:
	using super = TestContainerDataProviderBase<std::vector<int> >;

public:
	TestContainerDataProvider()
		: super(
			metapp::tkStdVector,
			metapp::tkInt,
			{
				-5, 0, 123456789, 38, -98765, 
			}
			)
	{
	}
};

template <>
class TestContainerDataProvider<std::vector<std::string> >
	: public TestContainerDataProviderBase<std::vector<std::string> >
{
private:
	using super = TestContainerDataProviderBase<std::vector<std::string> >;

public:
	TestContainerDataProvider()
		: super(
			metapp::tkStdVector,
			metapp::tkStdString,
			{
				"hello", "", "world", "This is great!", "Very good"
			}
		)
	{
	}
};

template <>
class TestContainerDataProvider<std::deque<int> >
	: public TestContainerDataProviderBase<std::deque<int> >
{
private:
	using super = TestContainerDataProviderBase<std::deque<int> >;

public:
	TestContainerDataProvider()
		: super(
			metapp::tkStdDeque,
			metapp::tkInt,
			{
				-5, 0, 123456789, 38, -98765, 
			}
			)
	{
	}
};

template <>
class TestContainerDataProvider<std::deque<std::string> >
	: public TestContainerDataProviderBase<std::deque<std::string> >
{
private:
	using super = TestContainerDataProviderBase<std::deque<std::string> >;

public:
	TestContainerDataProvider()
		: super(
			metapp::tkStdDeque,
			metapp::tkStdString,
			{
				"hello", "", "world", "This is great!", "Very good"
			}
		)
	{
	}
};

using TestTypes_Indexables = metapp::TypeList<
	std::array<int, 5>,
	std::array<std::string, 5>,
	std::vector<int>,
	std::vector<std::string>,
	std::deque<int>,
	std::deque<std::string>
>;


#ifdef METAPP_COMPILER_VC
#pragma warning(pop)
#endif


#endif
