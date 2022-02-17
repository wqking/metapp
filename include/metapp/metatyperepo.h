#ifndef METATYPEREPO_H_969872685611
#define METATYPEREPO_H_969872685611

#include "metapp/metatype.h"
#include "metapp/typelist.h"

#include <unordered_map>
#include <string>
#include <cstring>

namespace metapp {

struct StringEqual
{
	bool operator () (const char * a, const char * b) const {
		return a == b || strcmp(a, b) == 0;
	}
};

// http://isthe.com/chongo/tech/comp/fnv/#FNV-param
template <int size>
struct HashParameter
{
};

template <>
struct HashParameter <8>
{
	// 64 bits
	static constexpr size_t prime = (size_t)1099511628211ULL;
	static constexpr size_t offsetBasis = (size_t)14695981039346656037ULL;
	static constexpr size_t shift = 3;
	static constexpr size_t size = 8;
};

template <>
struct HashParameter <4>
{
	// 32 bits
	static constexpr size_t prime = 16777619U;
	static constexpr size_t offsetBasis = 2166136261U;
	static constexpr size_t shift = 2;
	static constexpr size_t size = 4;
};

struct StringHash
{
	size_t operator () (const char * s) const
	{
		const size_t length = strlen(s);
		const size_t count = (length >> HashParameter<sizeof(size_t)>::shift);
		size_t result = HashParameter<sizeof(size_t)>::offsetBasis;
		size_t i = 0;
		for(; i < count * HashParameter<sizeof(size_t)>::size; i += HashParameter<sizeof(size_t)>::size) {
			result ^= *(const size_t *)(s + i);
			result *= HashParameter<sizeof(size_t)>::prime;
		}
		for(; i < length; ++i) {
			result ^= (size_t)s[i];
			result *= HashParameter<sizeof(size_t)>::prime;
		}
		return result;
	}
};

class MetaTypeRepo
{
public:
	void registerMetaType(const MetaType * metaType);

	const MetaType * getMetaType(const TypeKind typeKind) const;

private:
	MetaTypeRepo();

	MetaTypeRepo(const MetaTypeRepo &) = delete;
	MetaTypeRepo(MetaTypeRepo &&) = delete;

private:
	std::unordered_map<TypeKind, const MetaType *> metaTypeKindMap;
	std::unordered_map<const char *, const MetaType *, StringHash, StringEqual> metaTypeNameMap;

	friend MetaTypeRepo * getMetaTypeRepo();
};

MetaTypeRepo * getMetaTypeRepo();

template <typename ...Args>
void registerMetaType();

} // namespace metapp

#include "metapp/implement/metatyperepo_impl.h"


#endif
