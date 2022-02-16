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

inline MetaTypeRepo::MetaTypeRepo()
	: metaTypeKindMap()
{
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

inline void MetaTypeRepo::registerMetaType(const MetaType * metaType)
{
	metaTypeKindMap[metaType->getTypeKind()] = metaType;
	const char * name = metaType->getName();
	if(*name != 0) {
		metaTypeNameMap[name] = metaType;
	}
}

inline const MetaType * MetaTypeRepo::getMetaType(const TypeKind typeKind) const
{
	auto it = metaTypeKindMap.find(typeKind);
	if(it != metaTypeKindMap.end()) {
		return it->second;
	}
	else {
		return nullptr;
	}
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

