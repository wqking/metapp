#ifndef METATYPEREPO_H_969872685611
#define METATYPEREPO_H_969872685611

#include "metapp/metatype.h"
#include "metapp/typelist.h"

#include <unordered_map>
#include <string>

namespace metapp {

class MetaTypeRepo
{
public:
	void registerMetaType(const MetaType * metaType) {
		metaTypeMap[metaType->getTypeKind()] = metaType;
	}

	const MetaType * getMetaType(const TypeKind typeKind) const {
		auto it = metaTypeMap.find(typeKind);
		if(it != metaTypeMap.end()) {
			return it->second;
		}
		else {
			return nullptr;
		}
	}

private:
	MetaTypeRepo();

	MetaTypeRepo(const MetaTypeRepo &) = delete;
	MetaTypeRepo(MetaTypeRepo &&) = delete;

private:
	std::unordered_map<TypeKind, const MetaType *> metaTypeMap;

	friend MetaTypeRepo * getMetaTypeRepo();
};

namespace internal_ {

template <typename T>
struct RegisterMetaType;

template <typename Arg0, typename ...Args>
struct RegisterMetaType <TypeList<Arg0, Args...> >
{
	static void registerMetaType(MetaTypeRepo * metaTypeRepo)
	{
		metaTypeRepo->registerMetaType(getMetaType<Arg0>());
		RegisterMetaType<TypeList<Args...> >::registerMetaType(metaTypeRepo);
	}
};

template <>
struct RegisterMetaType <TypeList<> >
{
	static void registerMetaType(MetaTypeRepo * /*metaTypeRepo*/)
	{
	}
};

} // namespace internal_

MetaTypeRepo::MetaTypeRepo() : metaTypeMap() {
	internal_::RegisterMetaType<TypeList<
		void,
		std::string,
		std::wstring,
		bool,
		char, wchar_t,
		signed char, unsigned char,
		short, unsigned short,
		int, unsigned int,
		long, unsigned long,
		long long, unsigned long long,
		float, double, long double
	> >::registerMetaType(this);
}

inline MetaTypeRepo * getMetaTypeRepo()
{
	static MetaTypeRepo metaTypeRepo;
	return &metaTypeRepo;
}

template <typename ...Args>
void registerMetaType()
{
	internal_::RegisterMetaType<TypeList<Args...> >::registerMetaType(getMetaTypeRepo());
}


} // namespace metapp

#endif
