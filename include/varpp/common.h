#ifndef COMMON_H_969872685611
#define COMMON_H_969872685611

#include <memory>

namespace varpp {

constexpr size_t podSize = sizeof(long long);
using VarType = uint16_t;
using ExtendType = uint16_t;

constexpr VarType vtEmpty(0);

constexpr VarType vtFundamentalBegin(1);

constexpr VarType vtVoid(vtFundamentalBegin);

constexpr VarType vtArithmeticBegin(vtVoid + 1); // 2
constexpr VarType vtIntegerBegin(vtArithmeticBegin); // 2
constexpr VarType vtBool(vtIntegerBegin);
constexpr VarType vtChar(vtIntegerBegin + 1);
constexpr VarType vtWchar(vtIntegerBegin + 2);
constexpr VarType vtSignedChar(vtIntegerBegin + 3);
constexpr VarType vtUnsignedChar(vtIntegerBegin + 4);
constexpr VarType vtSignedShort(vtIntegerBegin + 5);
constexpr VarType vtUnsignedShort(vtIntegerBegin + 6);
constexpr VarType vtSignedInt(vtIntegerBegin + 7);
constexpr VarType vtUnsignedInt(vtIntegerBegin + 8);
constexpr VarType vtSignedLong(vtIntegerBegin + 9);
constexpr VarType vtUnsignedLong(vtIntegerBegin + 10);
constexpr VarType vtSignedLongLong(vtIntegerBegin + 11);
constexpr VarType vtUnsignedLongLong(vtIntegerBegin + 12);
constexpr VarType vtIntegerEnd(vtUnsignedLongLong);

constexpr VarType vtRealBegin(vtIntegerEnd + 1); // 15
constexpr VarType vtFloat(vtRealBegin);
constexpr VarType vtDouble(vtRealBegin + 1);
constexpr VarType vtLongDouble(vtRealBegin + 2);
constexpr VarType vtRealEnd(vtLongDouble);
constexpr VarType vtArithmeticEnd(vtRealEnd);

constexpr VarType vtFundamentalEnd(vtLongDouble);

constexpr VarType vtObject(31); // is equivalent to unkown type

constexpr ExtendType etNone = 0;
constexpr ExtendType etPointer = (1 << 0);
constexpr ExtendType etReference = (1 << 1);

struct VariantData
{
	uint8_t pod[podSize];
	std::shared_ptr<void> object;

	template <typename T>
	T & podAs() const {
		return *(T *)pod;
	}
};

template <typename T>
struct MakeVariantType
{
	using T1 = typename std::remove_cv<T>::type;
	using T2 = typename std::conditional<
		std::is_lvalue_reference<T1>::value,
		typename std::decay<T1>::type &,
		typename std::conditional<
		std::is_rvalue_reference<T1>::value,
		typename std::decay<T1>::type &&,
		typename std::decay<T1>::type
		>::type
	>::type;
	using Type = T2;
};


} // namespace varpp

#endif
