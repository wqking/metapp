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

#ifndef METAPP_REGISTEREDCONSTRUCTOR_H_969872685611
#define METAPP_REGISTEREDCONSTRUCTOR_H_969872685611

#include "metapp/registration/registeredmethod.h"

namespace metapp {

class RegisteredConstructor : public internal_::RegisteredMethodBase, public Annotation
{
private:
	using super = internal_::RegisteredMethodBase;

public:
	explicit RegisteredConstructor(const Variant & method)
		: super(method)
	{
	}

	const Variant & getConstructor() const {
		return super::doGetMethod();
	}

};

using RegisteredConstructorList = std::vector<std::reference_wrapper<const RegisteredConstructor> >;

} // namespace metapp


#endif
