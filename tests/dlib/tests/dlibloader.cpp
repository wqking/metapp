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

#include "dlibloader.h"

#if _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

DlibLoader::DlibLoader()
	: libHandle(nullptr)
{
}

DlibLoader::~DlibLoader()
{
}

void DlibLoader::load(const std::string & fileName)
{
#if _WIN32
	libHandle = (void *)LoadLibrary((fileName + ".dll").c_str());
#else
	libHandle = (void *)dlopen((fileName + ".so").c_str(), RTLD_LAZY);
#endif
}

void DlibLoader::unload()
{
	if(libHandle != nullptr) {
#if _WIN32
		FreeLibrary((HMODULE)libHandle);
#else
		dlclose(libHandle);
#endif
	}
	libHandle = nullptr;
}

void * DlibLoader::getSymbol(const std::string & symbolName)
{
#if _WIN32
	return (void *)GetProcAddress((HMODULE)libHandle, symbolName.c_str());
#else
	return dlsym(libHandle, symbolName.c_str());
#endif
}

bool DlibLoader::hasLoaded() const
{
	return libHandle != nullptr;
}
