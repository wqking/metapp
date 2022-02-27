namespace metapp {

namespace internal_ {

inline void registerBuiltinTypes(MetaRepository * repo)
{
	repo->addType<void>();
	repo->addType<bool>();
	repo->addType<char>();
	repo->addType<wchar_t>();
	repo->addType<signed char>();
	repo->addType<unsigned char>();
	repo->addType<short>();
	repo->addType<unsigned short>();
	repo->addType<int>();
	repo->addType<unsigned int>();
	repo->addType<long>();
	repo->addType<unsigned long>();
	repo->addType<long long>();
	repo->addType<unsigned long long>();
	repo->addType<float>();
	repo->addType<double>();
	repo->addType<long double>();
	repo->addType<std::string>();
	repo->addType<std::wstring>();
}

} // namespace

inline MetaRepository * getMetaRepository()
{
	struct RepoWrapper {
		RepoWrapper() : repo() {
			registerBuiltinTypes(&repo);
		}
		MetaRepository repo;
	};
	static RepoWrapper repoWrapper;
	return &repoWrapper.repo;
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



} // namespace metapp

