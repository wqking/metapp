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

#ifndef DLIB_H_969872685611
#define DLIB_H_969872685611

#include "dlibdata.h"

#include "metapp/compiler.h"

#include <string>

#ifdef _WINDLL
#ifdef __cplusplus
#define DLIB_API extern "C" __declspec(dllexport)
#else
#define DLIB_API __declspec(dllexport)
#endif
#else
#ifdef __cplusplus
#define DLIB_API extern "C"
#else
#define DLIB_API
#endif
#endif

DLIB_API const LibData * getLibData();

#endif

