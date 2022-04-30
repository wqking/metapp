# MetaStreaming interface

## Overview

`MetaStreaming` is a meta interface to support input/output streaming operators `<<` and `>>`.  
A `Variant` that meta type implementing `MetaStreaming` can be used in I/O streaming operators.  

## Implement MetaStreaming

Usually you don't need to construct `MetaStreaming` directly. You should inherit the declared meta type from `MetaStreamingBase`.  
For example, assume `MyClass` supports I/O streaming, then we can `DeclareMetaType` MyClass as below,  

```c++
// the header for MetaStreamingBase
#include "metapp/interfaces/bases/metastreamingbase.h"

template <>
struct metapp::DeclareMetaType <MyClass> : metapp::MetaStreamingBase <MyClass>
{
};
```
