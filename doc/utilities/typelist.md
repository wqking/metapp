[//]: # (Auto generated file, don't modify this file.)

# TypeList reference

## Overview

`metapp::TypeList` is a template that holds a list of types.  
For any end user, you only need to know how to construct a `TypeList`.  

## Header

```c++
#include "metapp/utilities/typelist.h"
```

## Examples
TL1 is a type list of `int` and `char`.

```c++
using TL1 = metapp::TypeList<int, char>;
```

TL2 is a type list of `std::string`, `std::vector<int>`, `bool`, `long`, and `double`.

```c++
using TL2 = metapp::TypeList<std::string, std::vector<int>, bool, long, double>;
```

## Use cases

`TypeList` is used when declaring meta type. The `UpType` can be a `TypeList`, and implementing `cast` and `castFrom`
can use `TypeList`.  
[See document of DeclareMetaType for more details](../declaremetatype.md)