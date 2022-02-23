#ifndef TYPEKIND_H_969872685611
#define TYPEKIND_H_969872685611

#include <cstdint>
#include <unordered_map>
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
constexpr TypeKind tkString = 31; // std::string
constexpr TypeKind tkWideString = 32; // std::wstring

constexpr TypeKind tkPointer = 33;
constexpr TypeKind tkReference = 34;
constexpr TypeKind tkFunction = 35;
constexpr TypeKind tkMemberFunction = 36;
constexpr TypeKind tkMemberPointer = 37;
constexpr TypeKind tkConstructor = 38;
constexpr TypeKind tkDefaultArgsFunction = 39;

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

inline std::unordered_map<TypeKind, std::string> & getTypeKindNameMap()
{
	static std::unordered_map<TypeKind, std::string> typeKindNameMap {
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
		{ tkString, "std::string" },
		{ tkWideString, "std::wstring" },
		
		{ tkPointer, "pointer" },
		{ tkReference, "reference" },
		{ tkFunction, "function" },
		{ tkMemberFunction, "member_func" },
		{ tkMemberPointer, "member_pointer" },
		{ tkConstructor, "constructor" },
		{ tkDefaultArgsFunction, "default_args_function" },
	
		{ tkSharedPtr, "std::shared_ptr" },
		{ tkStdFunction, "std::function" },
		{ tkVector, "std::vector" },
		{ tkList, "std::list" },
		{ tkDeque, "std::deque" },
		{ tkStdArray, "std::array" },
		{ tkForwardList, "std::forward_list" },
		{ tkStack, "std::stack" },
		{ tkQueue, "std::queue" },
		{ tkPriorityQueue, "std::priority_queue" },
		{ tkMap, "std::map" },
		{ tkMultimap, "std::multipmap" },
		{ tkSet, "std::set" },
		{ tkMultiset, "std::multiset" },
		{ tkUnorderedMap, "std::unordered_map" },
		{ tkUnorderedMultimap, "std::unordered_multimap" },
		{ tkUnorderedSet, "std::unordered_set" },
		{ tkUnorderedMultiset, "std::unordered_multiset" },
		{ tkPair, "std::pair" },
		{ tkTuple, "std::tuple" },
		{ tkAny, "std::any" },
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
