#ifndef COMMON_H_969872685611
#define COMMON_H_969872685611

#include <memory>

namespace varpp {

constexpr size_t podSize = sizeof(long long);
using VarType = uint16_t;
using ExtendType = uint16_t;

static constexpr VarType vtEmpty = 0;

static constexpr VarType vtFundamentalBegin = 1;

static constexpr VarType vtVoid = vtFundamentalBegin;

static constexpr VarType vtArithmeticBegin  = vtVoid + 1; // 2
static constexpr VarType vtIntegerBegin = vtArithmeticBegin; // 2
static constexpr VarType vtBool = vtIntegerBegin;
static constexpr VarType vtChar = vtIntegerBegin + 1;
static constexpr VarType vtWchar = vtIntegerBegin + 2;
static constexpr VarType vtSignedChar = vtIntegerBegin + 3;
static constexpr VarType vtUnsignedChar = vtIntegerBegin + 4;
static constexpr VarType vtSignedShort = vtIntegerBegin + 5;
static constexpr VarType vtUnsignedShort = vtIntegerBegin + 6;
static constexpr VarType vtSignedInt = vtIntegerBegin + 7;
static constexpr VarType vtUnsignedInt = vtIntegerBegin + 8;
static constexpr VarType vtSignedLong = vtIntegerBegin + 9;
static constexpr VarType vtUnsignedLong = vtIntegerBegin + 10;
static constexpr VarType vtSignedLongLong = vtIntegerBegin + 11;
static constexpr VarType vtUnsignedLongLong = vtIntegerBegin + 12;
static constexpr VarType vtIntegerEnd = vtUnsignedLongLong;

static constexpr VarType vtRealBegin = vtIntegerEnd + 1; // 15
static constexpr VarType vtFloat = vtRealBegin;
static constexpr VarType vtDouble = vtRealBegin + 1;
static constexpr VarType vtLongDouble = vtRealBegin + 2;
static constexpr VarType vtRealEnd = vtLongDouble;
static constexpr VarType vtArithmeticEnd  = vtRealEnd;

static constexpr VarType vtFundamentalEnd = vtLongDouble;

static constexpr VarType vtObject = 31; // is equivalent to unkown type

static constexpr ExtendType etNone = 0;
static constexpr ExtendType etPointer = (1 << 0);
static constexpr ExtendType etReference = (1 << 1);

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
