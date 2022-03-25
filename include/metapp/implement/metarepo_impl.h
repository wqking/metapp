// metapp library
// 
// Copyright (C) 2022 Wang Qi (wqking)
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

namespace metapp {

inline MetaRepo * getMetaRepo()
{
	static MetaRepo repo;
	return &repo;
}

inline MetaRepo::MetaRepo()
{
	registerBuiltinTypes();
}

inline MetaRepo * MetaRepo::addRepo(const std::string & name)
{
	std::unique_ptr<MetaRepo> repo(new MetaRepo());
	MetaRepo * result = repo.get();
	repoMap[name] = std::move(repo);
	return result;
}

inline std::vector<std::string> MetaRepo::getRepoNameList() const
{
	return internal_::getMapKeys(repoMap);
}

inline void MetaRepo::registerBuiltinTypes()
{
	registerType<void>();
	registerType<bool>();
	registerType<char>();
	registerType<wchar_t>();
	registerType<signed char>();
	registerType<unsigned char>();
	registerType<short>();
	registerType<unsigned short>();
	registerType<int>();
	registerType<unsigned int>();
	registerType<long>();
	registerType<unsigned long>();
	registerType<long long>();
	registerType<unsigned long long>();
	registerType<float>();
	registerType<double>();
	registerType<long double>();
	registerType<std::string>();
	registerType<std::wstring>();
}


} // namespace metapp

