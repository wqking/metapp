#ifndef COMMON_H_969872685611
#define COMMON_H_969872685611

#include <memory>

namespace varpp {

constexpr size_t podSize = (sizeof(long long) > sizeof(long double) ? sizeof(long long) : sizeof(long double));

using TypeKind = uint16_t;

constexpr TypeKind tkEmpty(0);

constexpr TypeKind tkVoid(1);

constexpr TypeKind tkFundamentalBegin(tkVoid + 1); // 2

constexpr TypeKind tkArithmeticBegin(tkFundamentalBegin); // 2
constexpr TypeKind tkIntegerBegin(tkArithmeticBegin); // 2
constexpr TypeKind tkBool(tkIntegerBegin); // 2
constexpr TypeKind tkChar(tkIntegerBegin + 1); // 3
constexpr TypeKind tkWideChar(tkIntegerBegin + 2); // 4
constexpr TypeKind tkSignedChar(tkIntegerBegin + 3); // 5
constexpr TypeKind tkUnsignedChar(tkIntegerBegin + 4); // 6
constexpr TypeKind tkShort(tkIntegerBegin + 5); // 7
constexpr TypeKind tkUnsignedShort(tkIntegerBegin + 6); // 8
constexpr TypeKind tkInt(tkIntegerBegin + 7); // 9
constexpr TypeKind tkUnsignedInt(tkIntegerBegin + 8); // 10
constexpr TypeKind tkLong(tkIntegerBegin + 9); // 11
constexpr TypeKind tkUnsignedLong(tkIntegerBegin + 10); // 12
constexpr TypeKind tkLongLong(tkIntegerBegin + 11); // 13
constexpr TypeKind tkUnsignedLongLong(tkIntegerBegin + 12); // 14
constexpr TypeKind tkIntegerEnd(tkUnsignedLongLong); // 14

constexpr TypeKind tkRealBegin(tkIntegerEnd + 1); // 15
constexpr TypeKind tkFloat(tkRealBegin); // 15
constexpr TypeKind tkDouble(tkRealBegin + 1); // 16
constexpr TypeKind tkLongDouble(tkRealBegin + 2); // 17
constexpr TypeKind tkRealEnd(tkLongDouble); // 18
constexpr TypeKind tkArithmeticEnd(tkRealEnd); // 19

constexpr TypeKind tkFundamentalEnd(tkArithmeticEnd); // 19

constexpr TypeKind tkOtherBegin(30); // 30

constexpr TypeKind tkObject(tkOtherBegin); // 30, equivalent to unkown type
constexpr TypeKind tkString(tkOtherBegin + 1); // 31, std::string
constexpr TypeKind tkWideString(tkOtherBegin + 1); // 32, std::wstring

constexpr TypeKind tkPointer(tkOtherBegin + 3); // 33
constexpr TypeKind tkReference(tkOtherBegin + 4); // 34
constexpr TypeKind tkSharedPtr(tkOtherBegin + 5); // 35
constexpr TypeKind tkVector(tkOtherBegin + 6); // 36
constexpr TypeKind tkList(tkOtherBegin + 7); // 37
constexpr TypeKind tkDeque(tkOtherBegin + 8); // 38
constexpr TypeKind tkArray(tkOtherBegin + 9); // 39
constexpr TypeKind tkForwardList(tkOtherBegin + 10); // 40
constexpr TypeKind tkStack(tkOtherBegin + 11); // 41
constexpr TypeKind tkQueue(tkOtherBegin + 12); // 42
constexpr TypeKind tkPriorityQueue(tkOtherBegin + 13); // 43

constexpr TypeKind tkUser(1024);


using QualifierKind = uint16_t;

constexpr QualifierKind qkConst = 1 << 0;
constexpr QualifierKind qkVolatile = 1 << 1;

struct MetaTypeData
{
	uint8_t pod[podSize];
	std::shared_ptr<void> object;

	template <typename T>
	T & podAs() const {
		return *(T *)pod;
	}
};


} // namespace varpp

#endif
