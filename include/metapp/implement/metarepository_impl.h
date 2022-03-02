namespace metapp {

inline MetaRepository * getMetaRepository()
{
	static MetaRepository repo;
	return &repo;
}

inline MetaRepository::MetaRepository()
{
	registerBuiltinTypes();
}

inline MetaRepository * MetaRepository::addRepository(const std::string & name)
{
	std::unique_ptr<MetaRepository> repo(new MetaRepository());
	MetaRepository * result = repo.get();
	repositoryMap[name] = std::move(repo);
	return result;
}

inline std::vector<std::string> MetaRepository::getRepositionNameList() const
{
	return internal_::getMapKeys(repositoryMap);
}

inline void MetaRepository::registerBuiltinTypes()
{
	addType<void>();
	addType<bool>();
	addType<char>();
	addType<wchar_t>();
	addType<signed char>();
	addType<unsigned char>();
	addType<short>();
	addType<unsigned short>();
	addType<int>();
	addType<unsigned int>();
	addType<long>();
	addType<unsigned long>();
	addType<long long>();
	addType<unsigned long long>();
	addType<float>();
	addType<double>();
	addType<long double>();
	addType<std::string>();
	addType<std::wstring>();
}


} // namespace metapp

