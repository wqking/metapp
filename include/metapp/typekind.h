#ifndef TYPEKIND_H_969872685611
#define TYPEKIND_H_969872685611

#include <cstdint>

namespace metapp {

using TypeKind = uint16_t;

constexpr TypeKind tkEmpty = 0;

constexpr TypeKind tkVoid = 1;

constexpr TypeKind tkFundamentalBegin = 2;

constexpr TypeKind tkArithmeticBegin = tkFundamentalBegin;
constexpr TypeKind tkIntegerBegin = tkArithmeticBegin;
constexpr TypeKind tkBool = 2;
constexpr TypeKind tkChar = 3;
constexpr TypeKind tkWideChar = 4;
constexpr TypeKind tkSignedChar = 5;
constexpr TypeKind tkUnsignedChar = 6;
constexpr TypeKind tkShort = 7;
constexpr TypeKind tkUnsignedShort = 8;
constexpr TypeKind tkInt = 9;
constexpr TypeKind tkUnsignedInt = 10;
constexpr TypeKind tkLong = 11;
constexpr TypeKind tkUnsignedLong = 12;
constexpr TypeKind tkLongLong = 13;
constexpr TypeKind tkUnsignedLongLong = 14;
constexpr TypeKind tkIntegerEnd = 14;

constexpr TypeKind tkRealBegin = 15;
constexpr TypeKind tkFloat = 15;
constexpr TypeKind tkDouble = 16;
constexpr TypeKind tkLongDouble = 17;
constexpr TypeKind tkRealEnd = 18;
constexpr TypeKind tkArithmeticEnd = 18;

constexpr TypeKind tkFundamentalEnd = tkArithmeticEnd;

constexpr TypeKind tkObject = 30; // equivalent to unkown type
constexpr TypeKind tkString = 31; // std::string
constexpr TypeKind tkWideString = 32; // std::wstring

constexpr TypeKind tkPointer = 33;
constexpr TypeKind tkReference = 34;
constexpr TypeKind tkFunction = 35; // TBI
constexpr TypeKind tkMemberFunction = 36; // TBI
constexpr TypeKind tkMemberPointer = 37; // TBI

constexpr TypeKind tkSharedPtr = 60;
constexpr TypeKind tkStdFunction = 61;
constexpr TypeKind tkVector = 62;
constexpr TypeKind tkList = 63;
constexpr TypeKind tkDeque = 64;
constexpr TypeKind tkArray = 65;
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
