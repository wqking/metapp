#ifndef TYPEKIND_H_969872685611
#define TYPEKIND_H_969872685611

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
constexpr TypeKind tkCharPtr = 41;
constexpr TypeKind tkWideCharPtr = 42;
constexpr TypeKind tkAccessor = 43;
constexpr TypeKind tkVariant = 44; // metapp::Variant

constexpr TypeKind tkStdString = 70; // std::string
constexpr TypeKind tkStdWideString = 71; // std::wstring
constexpr TypeKind tkStdSharedPtr = 72;
constexpr TypeKind tkStdFunction = 73;
constexpr TypeKind tkStdVector = 74;
constexpr TypeKind tkStdList = 75;
constexpr TypeKind tkStdDeque = 76;
constexpr TypeKind tkStdArray = 77;
constexpr TypeKind tkStdForwardList = 78;
constexpr TypeKind tkStdStack = 79;
constexpr TypeKind tkStdQueue = 80;
constexpr TypeKind tkStdPriorityQueue = 81;
constexpr TypeKind tkStdMap = 82;
constexpr TypeKind tkStdMultimap = 83;
constexpr TypeKind tkStdSet = 84;
constexpr TypeKind tkStdMultiset = 85;
constexpr TypeKind tkStdUnorderedMap = 86;
constexpr TypeKind tkStdUnorderedMultimap = 87;
constexpr TypeKind tkStdUnorderedSet = 88;
constexpr TypeKind tkStdUnorderedMultiset = 89;
constexpr TypeKind tkStdPair = 90;
constexpr TypeKind tkStdTuple = 91;
constexpr TypeKind tkStdAny = 92;
constexpr TypeKind tkStdVariant = 93;

constexpr TypeKind tkUser = 1024;

using TypeFlags = uint16_t;

constexpr TypeFlags tfConst = 1 << 0;
constexpr TypeFlags tfVolatile = 1 << 1;
constexpr TypeFlags tfClassMember = 1 << 2;

inline std::map<TypeKind, std::string> & getTypeKindNameMap()
{
	static std::map<TypeKind, std::string> typeKindNameMap {
		{ tkVoid, "void" },
		{ tkBool, "bool" },
		{ tkChar, "char" },
		{ tkWideChar, "wchar_t" },
		{ tkSignedChar, "signed char" },
		{ tkUnsignedChar, "unsigned char" },
		{ tkShort, "short" },
		{ tkUnsignedShort, "unsigned short" },
		{ tkInt, "int" },
		{ tkUnsignedInt, "unsigned int" },
		{ tkLong, "long" },
		{ tkUnsignedLong, "unsigned long" },
		{ tkLongLong, "long long" },
		{ tkUnsignedLongLong, "unsigned long long" },
		{ tkFloat, "float" },
		{ tkDouble, "double" },
		{ tkLongDouble, "long double" },

		{ tkObject, "object" },
		{ tkPointer, "pointer" },
		{ tkReference, "reference" },
		{ tkFunction, "function" },
		{ tkMemberFunction, "member_func" },
		{ tkMemberPointer, "member_pointer" },
		{ tkConstructor, "constructor" },
		{ tkArray, "array" },
		{ tkEnum, "enum" },
		{ tkCharPtr, "const char *" },
		{ tkWideCharPtr, "const wchar_t *" },
		{ tkDefaultArgsFunction, "default_args_function" },
		{ tkVariadicFunction, "variadic_function" },

		{ tkStdString, "std::string" },
		{ tkStdWideString, "std::wstring" },
		{ tkStdSharedPtr, "std::shared_ptr" },
		{ tkStdFunction, "std::function" },
		{ tkStdVector, "std::vector" },
		{ tkStdList, "std::list" },
		{ tkStdDeque, "std::deque" },
		{ tkStdArray, "std::array" },
		{ tkStdForwardList, "std::forward_list" },
		{ tkStdStack, "std::stack" },
		{ tkStdQueue, "std::queue" },
		{ tkStdPriorityQueue, "std::priority_queue" },
		{ tkStdMap, "std::map" },
		{ tkStdMultimap, "std::multipmap" },
		{ tkStdSet, "std::set" },
		{ tkStdMultiset, "std::multiset" },
		{ tkStdUnorderedMap, "std::map" },
		{ tkStdUnorderedMultimap, "std::unordered_multimap" },
		{ tkStdUnorderedSet, "std::unordered_set" },
		{ tkStdUnorderedMultiset, "std::unordered_multiset" },
		{ tkStdPair, "std::pair" },
		{ tkStdTuple, "std::tuple" },
		{ tkStdAny, "std::any" },
		{ tkStdVariant, "std::variant" },
	};
	return typeKindNameMap;
}

inline std::string getNameByTypeKind(const TypeKind typeKind)
{
	auto it = getTypeKindNameMap().find(typeKind);
	if(it != getTypeKindNameMap().end()) {
		return it->second;
	}
	return "";
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
