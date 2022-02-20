#ifndef TYPEKIND_H_969872685611
#define TYPEKIND_H_969872685611

#include <cstdint>

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
constexpr TypeKind tkRealEnd = 17;

constexpr TypeKind tkArithmeticEnd = 17;

constexpr TypeKind tkFundamentalEnd = tkArithmeticEnd;

constexpr TypeKind tkObject = 30; // equivalent to unkown type
constexpr TypeKind tkString = 31; // std::string
constexpr TypeKind tkWideString = 32; // std::wstring

constexpr TypeKind tkPointer = 33;
constexpr TypeKind tkReference = 34;
constexpr TypeKind tkFunction = 35;
constexpr TypeKind tkMemberFunction = 36;
constexpr TypeKind tkMemberPointer = 37;
constexpr TypeKind tkConstructor = 38;

constexpr TypeKind tkSharedPtr = 60;
constexpr TypeKind tkStdFunction = 61;
constexpr TypeKind tkVector = 62;
constexpr TypeKind tkList = 63;
constexpr TypeKind tkDeque = 64;
constexpr TypeKind tkStdArray = 65;
constexpr TypeKind tkForwardList = 66;
constexpr TypeKind tkStack = 67;
constexpr TypeKind tkQueue = 68;
constexpr TypeKind tkPriorityQueue = 69;
constexpr TypeKind tkMap = 70;
constexpr TypeKind tkMultimap = 71;
constexpr TypeKind tkSet = 72;
constexpr TypeKind tkMultiset = 73;
constexpr TypeKind tkUnorderedMap = 74;
constexpr TypeKind tkUnorderedMultimap = 75;
constexpr TypeKind tkUnorderedSet = 76;
constexpr TypeKind tkUnorderedMultiset = 77;
constexpr TypeKind tkPair = 78;
constexpr TypeKind tkTuple = 79;
constexpr TypeKind tkAny = 80;
constexpr TypeKind tkStdVariant = 81;

constexpr TypeKind tkUser = 1024;

using TypeFlags = uint16_t;

constexpr TypeFlags tfConst = 1 << 0;
constexpr TypeFlags tfVolatile = 1 << 1;
constexpr TypeFlags tfPodStorage = 1 << 2;

} // namespace metapp

#endif
