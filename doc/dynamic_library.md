[//]: # (Auto generated file, don't modify this file.)

# Use metapp in dynamic library

## Overview

We can use metapp in dynamic library. That's to say, we can expose meta data reflected in one dynamic library,
then use the meta data in the main program, or in another dynamic library.  

metapp is ready for use in dynamic library. The only thing we need to take care of is, we must give each meta type
a unique `typeKind`. When comparing meta types in different modules (different dynamic libraries or program),
metapp use `typeKind` to identify the meta type, so the `typeKind` must be unique.  
When we use metapp within a single module (a single program or a single dynamic library, no crossing dynamic libraries),
metapp compares meta types using a unique pointer, so whether `typeKind` is unique doesn't matter.  

To assign each meta type a unique `typeKind`, we need to [DeclareMetaType](declaremetatype.md)

## Note on performance

When checking `MetaType::equal` across modules, such as the main program (module A) call `Variant::get/cast` or invoke callable that
is from a dynamic library (module B), the performance can be much slower than doing the same action within a single module, especially
when the related `MetaType` has a lot of up types or deep level nested up types.  
The reason is, when checking `MetaType::equal` within a single module, the checking only requires up to two pairs of pointers
comparison. But when doing it across modules, the checking needs to traverse through all up types recursively and compare the type kind.

