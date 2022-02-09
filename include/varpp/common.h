#ifndef COMMON_H_969872685611
#define COMMON_H_969872685611

#include <memory>

namespace varpp {

constexpr size_t podSize = (sizeof(long long) > sizeof(long double) ? sizeof(long long) : sizeof(long double));

using VarType = uint16_t;

constexpr VarType vtEmpty(0);

constexpr VarType vtVoid(1);

constexpr VarType vtFundamentalBegin(vtVoid + 1); // 2

constexpr VarType vtArithmeticBegin(vtFundamentalBegin); // 2
constexpr VarType vtIntegerBegin(vtArithmeticBegin); // 2
constexpr VarType vtBool(vtIntegerBegin); // 2
constexpr VarType vtChar(vtIntegerBegin + 1); // 3
constexpr VarType vtWideChar(vtIntegerBegin + 2); // 4
constexpr VarType vtSignedChar(vtIntegerBegin + 3); // 5
constexpr VarType vtUnsignedChar(vtIntegerBegin + 4); // 6
constexpr VarType vtShort(vtIntegerBegin + 5); // 7
constexpr VarType vtUnsignedShort(vtIntegerBegin + 6); // 8
constexpr VarType vtInt(vtIntegerBegin + 7); // 9
constexpr VarType vtUnsignedInt(vtIntegerBegin + 8); // 10
constexpr VarType vtLong(vtIntegerBegin + 9); // 11
constexpr VarType vtUnsignedLong(vtIntegerBegin + 10); // 12
constexpr VarType vtLongLong(vtIntegerBegin + 11); // 13
constexpr VarType vtUnsignedLongLong(vtIntegerBegin + 12); // 14
constexpr VarType vtIntegerEnd(vtUnsignedLongLong); // 14

constexpr VarType vtRealBegin(vtIntegerEnd + 1); // 15
constexpr VarType vtFloat(vtRealBegin); // 15
constexpr VarType vtDouble(vtRealBegin + 1); // 16
constexpr VarType vtLongDouble(vtRealBegin + 2); // 17
constexpr VarType vtRealEnd(vtLongDouble); // 18
constexpr VarType vtArithmeticEnd(vtRealEnd); // 19

constexpr VarType vtFundamentalEnd(vtArithmeticEnd); // 19

constexpr VarType vtOtherBegin(30); // 30

constexpr VarType vtObject(vtOtherBegin); // 30, equivalent to unkown type
constexpr VarType vtString(vtOtherBegin + 1); // 31, std::string
constexpr VarType vtWideString(vtOtherBegin + 1); // 32, std::wstring

constexpr VarType vtPointer(vtOtherBegin + 3); // 33
constexpr VarType vtReference(vtOtherBegin + 4); // 34
constexpr VarType vtSharedPtr(vtOtherBegin + 5); // 35
constexpr VarType vtVector(vtOtherBegin + 6); // 36
constexpr VarType vtList(vtOtherBegin + 7); // 37
constexpr VarType vtDeque(vtOtherBegin + 8); // 38
constexpr VarType vtArray(vtOtherBegin + 9); // 39
constexpr VarType vtForwardList(vtOtherBegin + 10); // 40
constexpr VarType vtStack(vtOtherBegin + 11); // 41
constexpr VarType vtQueue(vtOtherBegin + 12); // 42
constexpr VarType vtPriorityQueue(vtOtherBegin + 13); // 43

using ExtendType = uint16_t;

constexpr ExtendType etNone(0);
constexpr ExtendType etOne(1);
constexpr ExtendType etConst = etOne;
constexpr ExtendType etVolatile = etOne << 1;

struct VariantData
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
