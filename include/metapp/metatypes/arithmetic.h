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

#ifndef METAPP_ARITHMETIC_H_969872685611
#define METAPP_ARITHMETIC_H_969872685611

#include "metapp/metatype.h"
#include "metapp/utilities/typelist.h"
#include "metapp/utilities/utility.h"
#include "metapp/implement/internal/knowntypes_i.h"
#include "metapp/interfaces/bases/metastreamablebase.h"
#include "metapp/cast.h"

namespace metapp {

namespace internal_ {

} // namespace internal_

template <typename T>
struct DeclareMetaTypeBase <T,
	typename std::enable_if<TypeListIn<internal_::ArithmeticTypeList, T>::value>::type>
	: MetaStreamableBase<T>
{
	static constexpr TypeKind typeKind = TypeKind(tkFundamentalBegin + TypeListIndexOf<internal_::ArithmeticTypeList, T>::value);

	// The casting can be done by inheriting from CastToTypes<internal_::ArithmeticTypeList>, and no need to implement `cast` here.
	// The problem is CastToTypes has O(N) time complexity, N is the target type count which is large (near 20) here, so
	// the performance is bad (casting between arithmetics are quite commonly used so performance is important).
	// Here we implement `cast` using O(1) algorithm for better performance.
	// Another side effect is the binary size decreased a little bit.
	static bool cast(Variant * result, const Variant * fromVar, const MetaType * toMetaType) {
		// If we don't use getNonReferenceMetaType below, reference to T can still be handled
		// correctly by `commonCast`, but that will affect performance.
		const TypeKind toTypeKind = getNonReferenceMetaType(toMetaType)->getTypeKind();
		if(typeKindIsArithmetic(toTypeKind)) {
			if(result != nullptr) {
				getCastToFunc(toTypeKind - tkArithmeticBegin)(result, fromVar->get<T>());
			}
			return true;
		}
		return commonCast(result, fromVar, getMetaType<T>(), toMetaType);
	}

private:
	template <typename U>
	static void doCastTo(Variant * result, const T value) {
		*result = static_cast<U>(value);
	}

	using CastToFunc = void (*)(Variant * result, const T value);
	static CastToFunc getCastToFunc(const int index)
	{
		static CastToFunc castToFuncList[] {
			&doCastTo<bool>,
			&doCastTo<char>, &doCastTo<wchar_t>,
			&doCastTo<internal_::metappChar8_t>,
			&doCastTo<char16_t>, &doCastTo<char32_t>,
			&doCastTo<signed char>, &doCastTo<unsigned char>,
			&doCastTo<short>, &doCastTo<unsigned short>,
			&doCastTo<int>, &doCastTo<unsigned int>,
			&doCastTo<long>, &doCastTo<unsigned long>,
			&doCastTo<long long>, &doCastTo<unsigned long long>,
			&doCastTo<float>, &doCastTo<double>, &doCastTo<long double>,
		};
		
		assert(index >= 0 && index < (TypeKind)(sizeof(castToFuncList) / sizeof(castToFuncList[0])));

		return castToFuncList[index];
	}

};


} // namespace metapp


#endif

