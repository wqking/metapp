// accessorpp library
// Copyright (C) 2022 Wang Qi (wqking)
// Github: https://github.com/wqking/accessorpp
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ACCESSORPP_COMPILER_H_873589173319
#define ACCESSORPP_COMPILER_H_873589173319

#if defined(_MSC_VER)
#define ACCESSORPP_COMPILER_VC
#endif
// When using clang in Visual Studio, both _MSC_VER and __clang__ can be defined
#if defined(__clang__)
#define ACCESSORPP_COMPILER_CLANG
#elif defined(__GNUC__)
#define ACCESSORPP_COMPILER_GCC
#else
#define ACCESSORPP_COMPILER_UNKNOWN
#endif

#if __cplusplus >= 201703L
	#define ACCESSORPP_SUPPORT_STANDARD_17
#else
	// Visual Studio 2017 RTW (15.0)
	#if defined(_MSC_VER) && _MSC_VER >= 1910
		// the code is inspired by vcruntime.h
		#if defined(_MSVC_LANG) && (_MSVC_LANG > 201402L)
			#define ACCESSORPP_SUPPORT_STANDARD_17
		#endif
	#endif
#endif


#endif

