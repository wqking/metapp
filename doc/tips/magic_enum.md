[//]: # (Auto generated file, don't modify this file.)

# Use magic_enum to register enum values automatically

## Overview

magic_enum is a third party library that provides static reflection for enums, work with any enum type without any macro
or boilerplate code.  
Using magic_enum can register enum values automatically.  
You can get magic_enum on https://github.com/Neargye/magic_enum

## Use magic_enum in metapp

Here we include the necessary headers

```c++
#include "metapp/allmetatypes.h"
#include "magic_enum/magic_enum.hpp"
// for enum_for_each
#include "magic_enum/magic_enum_utility.hpp"
```

This is the core function to register all values in an enum.

```c++
template <typename T>
void registerEnum(metapp::MetaEnum & me)
{
  magic_enum::enum_for_each<T>([&me] (auto value) {
    constexpr T v = value;
    me.registerValue(std::string(magic_enum::enum_name(v)), v);
  });
}
```

This is the sample code how to use it.

```c++
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
```

```c++
const auto metaType = metapp::getMetaType<EnumAnimal>();
const auto metaEnum = metaType->getMetaEnum();
ASSERT(metaEnum != nullptr);
ASSERT(metaEnum->getByName("dog").asEnumValue().get<EnumAnimal>() == EnumAnimal::dog);
ASSERT(metaEnum->getByValue(EnumAnimal::cat).getName() == "cat");
```

## Why metapp doesn't include magic_enum by default?

Because magic_enum has limitations on the number of enum values and the compiler version.

