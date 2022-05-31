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

using TypeKind = int32_t;

constexpr TypeKind tkVoid = 0;

constexpr TypeKind tkFundamentalBegin = 1;

constexpr TypeKind tkArithmeticBegin = tkFundamentalBegin;

constexpr TypeKind tkIntegerBegin = tkArithmeticBegin;
constexpr TypeKind tkBool = 1;
constexpr TypeKind tkChar = 2;
constexpr TypeKind tkWideChar = 3;
constexpr TypeKind tkChar8 = 4;
constexpr TypeKind tkChar16 = 5;
constexpr TypeKind tkChar32 = 6;
constexpr TypeKind tkSignedChar = 7;
constexpr TypeKind tkUnsignedChar = 8;
constexpr TypeKind tkShort = 9;
constexpr TypeKind tkUnsignedShort = 10;
constexpr TypeKind tkInt = 11;
constexpr TypeKind tkUnsignedInt = 12;
constexpr TypeKind tkLong = 13;
constexpr TypeKind tkUnsignedLong = 14;
constexpr TypeKind tkLongLong = 15;
constexpr TypeKind tkUnsignedLongLong = 16;
constexpr TypeKind tkIntegerEnd = 16;

constexpr TypeKind tkRealBegin = 17;
constexpr TypeKind tkFloat = 17;
constexpr TypeKind tkDouble = 18;
constexpr TypeKind tkLongDouble = 19;
constexpr TypeKind tkRealEnd = 19;

constexpr TypeKind tkArithmeticEnd = tkRealEnd;

constexpr TypeKind tkFundamentalEnd = tkArithmeticEnd;

constexpr TypeKind tkObject = 40; // equivalent to unkown type
constexpr TypeKind tkPointer = 41;
constexpr TypeKind tkReference = 42;
constexpr TypeKind tkFunction = 43;
constexpr TypeKind tkMemberFunction = 44;
constexpr TypeKind tkMemberPointer = 45;
constexpr TypeKind tkConstructor = 46;
constexpr TypeKind tkOverloadedFunction = 47;
constexpr TypeKind tkDefaultArgsFunction = 48;
constexpr TypeKind tkVariadicFunction = 49;
constexpr TypeKind tkArray = 50;
constexpr TypeKind tkEnum = 51;
constexpr TypeKind tkAccessor = 52;
constexpr TypeKind tkVariant = 53; // metapp::Variant
constexpr TypeKind tkMetaType = 54; // metapp::MetaType
constexpr TypeKind tkMetaRepo = 55; // metapp::MetaRepo

constexpr TypeKind tkStdString = 100; // std::string
constexpr TypeKind tkStdWideString = 101; // std::wstring
constexpr TypeKind tkStdSharedPtr = 102;
constexpr TypeKind tkStdUniquePtr = 103;
constexpr TypeKind tkStdWeakPtr = 104;
constexpr TypeKind tkStdFunction = 105;
constexpr TypeKind tkStdVector = 106;
constexpr TypeKind tkStdList = 107;
constexpr TypeKind tkStdDeque = 108;
constexpr TypeKind tkStdArray = 109;
constexpr TypeKind tkStdForwardList = 110;
constexpr TypeKind tkStdStack = 111;
constexpr TypeKind tkStdQueue = 112;
constexpr TypeKind tkStdPriorityQueue = 113;
constexpr TypeKind tkStdMap = 114;
constexpr TypeKind tkStdMultimap = 115;
constexpr TypeKind tkStdSet = 116;
constexpr TypeKind tkStdMultiset = 117;
constexpr TypeKind tkStdUnorderedMap = 118;
constexpr TypeKind tkStdUnorderedMultimap = 119;
constexpr TypeKind tkStdUnorderedSet = 120;
constexpr TypeKind tkStdUnorderedMultiset = 121;
constexpr TypeKind tkStdPair = 122;
constexpr TypeKind tkStdTuple = 123;
constexpr TypeKind tkStdAny = 124;
constexpr TypeKind tkStdVariant = 125;

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


} // namespace metapp

#endif
