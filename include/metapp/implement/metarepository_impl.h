namespace metapp {

namespace internal_ {

inline void registerBuiltinTypes(MetaRepository * repo)
{
	repo->addType<void>("void");
	repo->addType<bool>("bool");
	repo->addType<char>("char");
	repo->addType<wchar_t>("wchar_t");
	repo->addType<signed char>("signed char");
	repo->addType<unsigned char>("unsigned char");
	repo->addType<short>("short");
	repo->addType<unsigned short>("unsigned short");
	repo->addType<int>("int");
	repo->addType<unsigned int>("unsigned int");
	repo->addType<long>("long");
	repo->addType<unsigned long>("unsigned long");
	repo->addType<long long>("long long");
	repo->addType<unsigned long long>("unsigned long long");
	repo->addType<float>("float");
	repo->addType<double>("double");
	repo->addType<long double>("long double");
	repo->addType<std::string>("string");
	repo->addType<std::wstring>("wstring");
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


} // namespace metapp

