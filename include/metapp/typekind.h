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
constexpr TypeKind tkRealEnd = 17;

constexpr TypeKind tkArithmeticEnd = 17;

constexpr TypeKind tkFundamentalEnd = tkArithmeticEnd;

constexpr TypeKind tkObject = 30; // equivalent to unkown type
constexpr TypeKind tkPointer = 31;
constexpr TypeKind tkReference = 32;
constexpr TypeKind tkFunction = 33;
constexpr TypeKind tkMemberFunction = 34;
constexpr TypeKind tkMemberPointer = 35;
constexpr TypeKind tkConstructor = 36;
constexpr TypeKind tkArray = 37;
constexpr TypeKind tkEnum = 38;
constexpr TypeKind tkCharPtr = 39;
constexpr TypeKind tkWideCharPtr = 40;
constexpr TypeKind tkDefaultArgsFunction = 41;

constexpr TypeKind tkStdString = 60; // std::string
constexpr TypeKind tkStdWideString = 61; // std::wstring
constexpr TypeKind tkStdSharedPtr = 62;
constexpr TypeKind tkStdFunction = 63;
constexpr TypeKind tkStdVector = 64;
constexpr TypeKind tkStdList = 65;
constexpr TypeKind tkStdDeque = 66;
constexpr TypeKind tkStdArray = 67;
constexpr TypeKind tkStdForwardList = 68;
constexpr TypeKind tkStdStack = 69;
constexpr TypeKind tkStdQueue = 70;
constexpr TypeKind tkStdPriorityQueue = 71;
constexpr TypeKind tkStdMap = 72;
constexpr TypeKind tkStdMultimap = 73;
constexpr TypeKind tkStdSet = 74;
constexpr TypeKind tkStdMultiset = 75;
constexpr TypeKind tkStdUnorderedMap = 76;
constexpr TypeKind tkStdUnorderedMultimap = 77;
constexpr TypeKind tkStdUnorderedSet = 78;
constexpr TypeKind tkStdUnorderedMultiset = 79;
constexpr TypeKind tkStdPair = 80;
constexpr TypeKind tkStdTuple = 81;
constexpr TypeKind tkStdAny = 81;
constexpr TypeKind tkStdVariant = 83;

constexpr TypeKind tkUser = 1024;

using TypeFlags = uint16_t;

constexpr TypeFlags tfConst = 1 << 0;
constexpr TypeFlags tfVolatile = 1 << 1;
constexpr TypeFlags tfPodStorage = 1 << 2;
constexpr TypeFlags tfClassMember = 1 << 3;

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


} // namespace metapp

#endif
