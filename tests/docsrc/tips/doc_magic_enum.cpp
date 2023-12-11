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

#include "testutil.h"

/*desc
# Use magic_enum to register enum values automatically

## Overview

magic_enum is a third party library that provides static reflection for enums, work with any enum type without any macro
or boilerplate code.  
Using magic_enum can register enum values automatically.  
You can get magic_enum on https://github.com/Neargye/magic_enum

## Use magic_enum in metapp

Here we include the necessary headers
desc*/

//code
#include "metapp/allmetatypes.h"
#include "magic_enum/magic_enum.hpp"
// for enum_for_each
#include "magic_enum/magic_enum_utility.hpp"
//code

/*desc
This is the core function to register all values in an enum.
desc*/
//code
template <typename T>
void registerEnum(metapp::MetaEnum & me)
{
	magic_enum::enum_for_each<T>([&me] (auto value) {
		constexpr T v = value;
		me.registerValue(std::string(magic_enum::enum_name(v)), v);
	});
}
//code

/*desc
This is the sample code how to use it.
desc*/
//code
enum class EnumAnimal {
	dog = 1,
	cat = 2,
	panda = 3
};

template <>
struct metapp::DeclareMetaType <EnumAnimal> : metapp::DeclareMetaTypeBase <EnumAnimal>
{
	static constexpr metapp::TypeKind typeKind = metapp::tkUser + 1;

	static const metapp::MetaEnum * getMetaEnum() {
		static const metapp::MetaEnum metaEnum([](metapp::MetaEnum & me) {
			registerEnum<EnumAnimal>(me);
			
			// If we don't use magic_enum, we have to register values one by one
			//me.registerValue("dog", EnumAnimal::dog);
			//me.registerValue("cat", EnumAnimal::cat);
			//me.registerValue("panda", EnumAnimal::panda);
		});
		return &metaEnum;
	}
};

//code

ExampleFunc
{
	//code
	const auto metaType = metapp::getMetaType<EnumAnimal>();
	const auto metaEnum = metaType->getMetaEnum();
	ASSERT(metaEnum != nullptr);
	ASSERT(metaEnum->getByName("dog").asEnumValue().get<EnumAnimal>() == EnumAnimal::dog);
	ASSERT(metaEnum->getByValue(EnumAnimal::cat).getName() == "cat");
	//code
}


/*desc
## Why metapp doesn't include magic_enum by default?

Because magic_enum has limitations on the number of enum values and the compiler version.

desc*/
