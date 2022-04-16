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

#ifndef METAPP_TYPEKIND_H_969872685611
#define METAPP_TYPEKIND_H_969872685611

#include <cstdint>
#include <map>
#include <string>

namespace metapp {

using TypeKind = uint16_t;

constexpr TypeKind tkVoid = 0;

constexpr TypeKind tkFundamentalBegin = 1;

constexpr TypeKind tkArithmeticBegin = tkFundamentalBegin;

constexpr TypeKind tkIntegerBegin = tkArithmeticBegin;
constexpr TypeKind tkBool = 1;
constexpr TypeKind tkChar = 2;
constexpr TypeKind tkWideChar = 3;
constexpr TypeKind tkSignedChar = 4;
constexpr TypeKind tkUnsignedChar = 5;
constexpr TypeKind tkShort = 6;
constexpr TypeKind tkUnsignedShort = 7;
constexpr TypeKind tkInt = 8;
constexpr TypeKind tkUnsignedInt = 9;
constexpr TypeKind tkLong = 10;
constexpr TypeKind tkUnsignedLong = 11;
constexpr TypeKind tkLongLong = 12;
constexpr TypeKind tkUnsignedLongLong = 13;
constexpr TypeKind tkIntegerEnd = 13;

constexpr TypeKind tkRealBegin = 14;
constexpr TypeKind tkFloat = 14;
constexpr TypeKind tkDouble = 15;
constexpr TypeKind tkLongDouble = 16;
constexpr TypeKind tkRealEnd = 16;

constexpr TypeKind tkArithmeticEnd = tkRealEnd;

constexpr TypeKind tkFundamentalEnd = tkArithmeticEnd;

constexpr TypeKind tkObject = 30; // equivalent to unkown type
constexpr TypeKind tkPointer = 31;
constexpr TypeKind tkReference = 32;
constexpr TypeKind tkFunction = 33;
constexpr TypeKind tkMemberFunction = 34;
constexpr TypeKind tkMemberPointer = 35;
constexpr TypeKind tkConstructor = 36;
constexpr TypeKind tkDefaultArgsFunction = 37;
constexpr TypeKind tkVariadicFunction = 38;
constexpr TypeKind tkArray = 39;
constexpr TypeKind tkEnum = 40;
constexpr TypeKind tkAccessor = 41;
constexpr TypeKind tkVariant = 42; // metapp::Variant

constexpr TypeKind tkStdString = 70; // std::string
constexpr TypeKind tkStdWideString = 71; // std::wstring
constexpr TypeKind tkStdSharedPtr = 72;
constexpr TypeKind tkStdUniquePtr = 73;
constexpr TypeKind tkStdFunction = 74;
constexpr TypeKind tkStdVector = 75;
constexpr TypeKind tkStdList = 76;
constexpr TypeKind tkStdDeque = 77;
constexpr TypeKind tkStdArray = 78;
constexpr TypeKind tkStdForwardList = 79;
constexpr TypeKind tkStdStack = 80;
constexpr TypeKind tkStdQueue = 81;
constexpr TypeKind tkStdPriorityQueue = 82;
constexpr TypeKind tkStdMap = 83;
constexpr TypeKind tkStdMultimap = 84;
constexpr TypeKind tkStdSet = 85;
constexpr TypeKind tkStdMultiset = 86;
constexpr TypeKind tkStdUnorderedMap = 87;
constexpr TypeKind tkStdUnorderedMultimap = 88;
constexpr TypeKind tkStdUnorderedSet = 89;
constexpr TypeKind tkStdUnorderedMultiset = 90;
constexpr TypeKind tkStdPair = 91;
constexpr TypeKind tkStdTuple = 92;
constexpr TypeKind tkStdAny = 93;
constexpr TypeKind tkStdVariant = 94;

constexpr TypeKind tkUser = 1024;

using TypeFlags = uint16_t;

constexpr TypeFlags tfConst = 1 << 0;
constexpr TypeFlags tfVolatile = 1 << 1;
constexpr TypeFlags tfPointer = 1 << 2;
constexpr TypeFlags tfReference = 1 << 3;
constexpr TypeFlags tfClass = 1 << 4;
constexpr TypeFlags tfArray = 1 << 5;
constexpr TypeFlags tfEnum = 1 << 6;
constexpr TypeFlags tfMemberPointer = 1 << 7;
constexpr TypeFlags tfIntegral = 1 << 8;
constexpr TypeFlags tfFloat = 1 << 9;

inline constexpr bool typeKindIsArithmetic(const TypeKind typeKind)
{
	return typeKind >= tkArithmeticBegin && typeKind <= tkArithmeticEnd;
}

inline constexpr bool typeKindIsInteger(const TypeKind typeKind)
{
	return typeKind >= tkIntegerBegin && typeKind <= tkIntegerEnd;
}

inline constexpr bool typeKindIsReal(const TypeKind typeKind)
{
	return typeKind >= tkRealBegin && typeKind <= tkRealEnd;
}


} // namespace metapp

#endif
