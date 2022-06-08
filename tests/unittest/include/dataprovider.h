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

#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include "metapp/compiler.h"
#include "metapp/metatype.h"
#include "metapp/typekind.h"
#include "metapp/utilities/typelist.h"

#include <vector>
#include <unordered_set>
#include <tuple>
#include <initializer_list>
#include <algorithm>

#ifdef METAPP_COMPILER_VC
#pragma warning(push)
#pragma warning(disable: 4310) // cast truncates constant value
#endif

template <typename T>
class TestDataProviderBase
{
public:
	using DataType = T;

public:
	TestDataProviderBase(const metapp::TypeKind typeKind, std::initializer_list<T> dataList)
		: typeKind(typeKind), dataList(dataList)
	{
		// remove duplicates
		auto end = this->dataList.end();
		for (auto it = this->dataList.begin(); it != end; ++it) {
			end = std::remove(it + 1, end, *it);
		}
		this->dataList.erase(end, this->dataList.end());
	}

	metapp::TypeKind getTypeKind() const {
		return typeKind;
	}

	int getDataCount() const {
		return (int)dataList.size();
	}

	T getData(const int index) const {
		return dataList.at(index % getDataCount());
	}

private:
	metapp::TypeKind typeKind;
	std::vector<T> dataList;
};

template <typename T, metapp::TypeKind kind>
class TestDataProviderArithmetic : public TestDataProviderBase<T>
{
private:
	using super = TestDataProviderBase<T>;

public:
	TestDataProviderArithmetic()
		: super(kind, {
			(T)0,
			std::numeric_limits<T>::min(),
			std::numeric_limits<T>::max(),
			(T)((std::numeric_limits<T>::min() + std::numeric_limits<T>::max()) / 2),
			(T)100,
			(T)99999,
			(T)-7654321,
		})
	{
	}

};

template <typename T, typename Enabled = void>
class TestDataProvider
{
};

template <> class TestDataProvider<bool> : public TestDataProviderArithmetic<bool, metapp::tkBool> {};
template <> class TestDataProvider<char> : public TestDataProviderArithmetic<char, metapp::tkChar> {};
template <> class TestDataProvider<wchar_t> : public TestDataProviderArithmetic<wchar_t, metapp::tkWideChar> {};
template <> class TestDataProvider<signed char> : public TestDataProviderArithmetic<signed char, metapp::tkSignedChar> {};
template <> class TestDataProvider<unsigned char> : public TestDataProviderArithmetic<unsigned char, metapp::tkUnsignedChar> {};
template <> class TestDataProvider<short> : public TestDataProviderArithmetic<short, metapp::tkShort> {};
template <> class TestDataProvider<unsigned short> : public TestDataProviderArithmetic<unsigned short, metapp::tkUnsignedShort> {};
template <> class TestDataProvider<int> : public TestDataProviderArithmetic<int, metapp::tkInt> {};
template <> class TestDataProvider<unsigned int> : public TestDataProviderArithmetic<unsigned int, metapp::tkUnsignedInt> {};
template <> class TestDataProvider<long> : public TestDataProviderArithmetic<long, metapp::tkLong> {};
template <> class TestDataProvider<unsigned long> : public TestDataProviderArithmetic<unsigned long, metapp::tkUnsignedLong> {};
template <> class TestDataProvider<long long> : public TestDataProviderArithmetic<long long, metapp::tkLongLong> {};
template <> class TestDataProvider<unsigned long long> : public TestDataProviderArithmetic<unsigned long long, metapp::tkUnsignedLongLong> {};
template <> class TestDataProvider<float> : public TestDataProviderArithmetic<float, metapp::tkFloat> {};
template <> class TestDataProvider<double> : public TestDataProviderArithmetic<double, metapp::tkDouble> {};
template <> class TestDataProvider<long double> : public TestDataProviderArithmetic<long double, metapp::tkLongDouble> {};

template <>
class TestDataProvider <std::string> : public TestDataProviderBase<std::string>
{
private:
	using super = TestDataProviderBase<std::string>;

public:
	TestDataProvider()
		: super(metapp::tkStdString, {
			"",
			"hello",
			"good",
			"great",
			"perfect"
		})
	{
	}

};

template <>
class TestDataProvider <std::wstring> : public TestDataProviderBase<std::wstring>
{
private:
	using super = TestDataProviderBase<std::wstring>;

public:
	TestDataProvider()
		: super(metapp::tkStdWideString, {
			L"",
			L"hello",
			L"good",
			L"great",
			L"perfect"
		})
	{
	}

};

using TestTypes_Arithmetic = metapp::TypeList<
	bool,
	char, wchar_t,
	signed char, unsigned char,
	short, unsigned short,
	int, unsigned int,
	long, unsigned long,
	long long, unsigned long long,
	float, double, long double
>;

using TestTypes_All = typename metapp::TypeListConcat<
	TestTypes_Arithmetic,
	metapp::TypeList<std::string, std::wstring>
>::Type;

using TestTypes_Pairs_Arithmetic = typename metapp::TypeListProduct<
	metapp::TypeList<char, int, unsigned int, long long, double>,
	metapp::TypeList<bool, unsigned short, int, unsigned long, float, long double>
>::Type;

#ifdef METAPP_COMPILER_VC
#pragma warning(pop)
#endif


#endif
