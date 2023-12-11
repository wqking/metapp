[//]: # (Auto generated file, don't modify this file.)

# MetaEnum interface
<!--begintoc-->
- [Overview](#mdtoc_e7c3d1bb)
- [Header](#mdtoc_6e72a8c1)
- [Get MetaEnum interface](#mdtoc_42ccb38f)
- [Implement MetaEnum](#mdtoc_247b079c)
- [Implemented built-in meta types](#mdtoc_ed7f0e2e)
- [MetaEnum constructor](#mdtoc_348cb714)
- [MetaEnum member functions for registering meta data](#mdtoc_31633ac1)
  - [registerValue](#mdtoc_9435388f)
- [MetaEnum member functions for retrieving meta data](#mdtoc_9b450e78)
  - [getByName](#mdtoc_90be2fd1)
  - [getByValue](#mdtoc_50e8212)
  - [getValueView](#mdtoc_ff547046)
- [Non-member utility functions](#mdtoc_e4e47ded)
<!--endtoc-->

<a id="mdtoc_e7c3d1bb"></a>
## Overview

`MetaEnum` is a meta interface to obtain C++ `enum` name and values.  

<a id="mdtoc_6e72a8c1"></a>
## Header

```c++
#include "metapp/interfaces/metaenum.h"
```

<a id="mdtoc_42ccb38f"></a>
## Get MetaEnum interface

We can call `MetaType::getMetaEnum()` to get the MetaEnum interface. If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<MyEnum>();
const metapp::MetaEnum * metaEnum = metaType->getMetaEnum();
```

<a id="mdtoc_247b079c"></a>
## Implement MetaEnum

```c++
// This is the enum we are going to reflect for.
enum class EnumAnimal {
  dog = 1,
  cat = 2,
  panda = 3
};

// We use metapp::DeclareMetaType to declare a type for EnumAnimal.
template <>
struct metapp::DeclareMetaType <EnumAnimal> : metapp::DeclareMetaTypeBase <EnumAnimal>
{
  // The static function getMetaEnum is where we implement MetaEnum.
  static const metapp::MetaEnum * getMetaEnum() {
    // Define a static metapp::MetaEnum object. Note it must be static.
    static const metapp::MetaEnum metaEnum([](metapp::MetaEnum & me) {
      // Register the values into the passed in MetaEnum
      me.registerValue("dog", EnumAnimal::dog);
      me.registerValue("cat", EnumAnimal::cat);
      me.registerValue("panda", EnumAnimal::panda);
    });
    // Return the MetaEnum object.
    return &metaEnum;
  }
};
```

```c++
// Let's use the MetaEnum
const auto metaType = metapp::getMetaType<EnumAnimal>();
const auto metaEnum = metaType->getMetaEnum();
ASSERT(metaEnum != nullptr);
ASSERT(metaEnum->getByName("dog").asEnumValue().get<EnumAnimal>() == EnumAnimal::dog);
ASSERT(metaEnum->getByValue(EnumAnimal::cat).getName() == "cat");
```

<a id="mdtoc_ed7f0e2e"></a>
## Implemented built-in meta types

None

<a id="mdtoc_348cb714"></a>
## MetaEnum constructor

```c++
template <typename FT>
explicit MetaEnum(FT callback);
```

`callback` is a callback function. MetaEnum invokes the callback in the constructor.
Since the interface is usually implemented as static variable inside static function, the "callback in constructor" mechanism
can guarantee thread safety.  

`callback` prototype,  
```c++
void callback(metapp::MetaEnum & me);
```
The MetaEnum instance under constructing is passed as the parameter. The callback should register all meta data to `me`.

<a id="mdtoc_31633ac1"></a>
## MetaEnum member functions for registering meta data

<a id="mdtoc_9435388f"></a>
#### registerValue

```c++
MetaItem & registerValue(const std::string & name, const Variant & value);
```

Register a enum `value` under `name`. `value` can be any type, not limited to enum value.  
The returned `MetaItem` can be used to add annotations to the meta data.  


<a id="mdtoc_9b450e78"></a>
## MetaEnum member functions for retrieving meta data

<a id="mdtoc_90be2fd1"></a>
#### getByName

```c++
const MetaItem & getByName(const std::string & name) const;
```

Get a MetaItem of `name`. If the name is not registered, an empty MetaItem is returned (MetaItem::isEmpty() is true).  

<a id="mdtoc_50e8212"></a>
#### getByValue

```c++
template <typename T>
const MetaItem & getByValue(const T value) const;
```

Get a MetaItem of `value`. If the value is not registered, an empty MetaItem is returned (MetaItem::isEmpty() is true).  
`value` must be an enumerate value that can cast to `long long`.

<a id="mdtoc_ff547046"></a>
#### getValueView

```c++
MetaItemView getValueView() const;
```

Returns a MetaItemView for all registered names and values.  

<a id="mdtoc_e4e47ded"></a>
## Non-member utility functions

Below free functions are shortcut functions to use the member functions in `MetaItem`.  

```c++
inline const MetaItem & enumGetByName(const Variant & var, const std::string & name)
{
  return getNonReferenceMetaType(var)->getMetaEnum()->getByName(name);
}

template <typename T>
inline const MetaItem & enumGetByValue(const Variant & var, const T value)
{
  return getNonReferenceMetaType(var)->getMetaEnum()->getByValue(value);
}

inline MetaItemView enumGetValueView(const Variant & var)
{
  return var.getMetaType()->getMetaEnum()->getValueView();
}
```

