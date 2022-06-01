[//]: # (Auto generated file, don't modify this file.)

# MetaStreamable interface

## Overview

`MetaStreamable` is a meta interface to support input/output streaming operators `<<` and `>>`.  
A `Variant` that meta type implementing `MetaStreamable` can be used in I/O streaming operators.  

## Implement MetaStreamable

Usually you don't need to construct `MetaStreamable` directly. You should inherit the declared meta type from `MetaStreamableBase`.  
For example, assume `MyClass` supports I/O streaming, then we can `DeclareMetaType` MyClass as below,  

```c++
// the header for MetaStreamableBase
#include "metapp/interfaces/bases/metastreamablebase.h"

template <>
struct metapp::DeclareMetaType <MyClass> : metapp::MetaStreamableBase <MyClass>
{
};
```
