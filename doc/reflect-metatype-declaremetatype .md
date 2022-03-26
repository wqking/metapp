# Use DeclareMetaType to reflect meta types at compile time

## Overview

Even though metapp works on any C++ type which are not known to metapp, it's useful to provide metapp more information on a certain type. The template `DeclareMetaType` is to provide such information.  
To provide more information of a type to metapp, specialize template `metapp::DeclareMetaType` with the type. The specialization must be in the global namespace. The specialization should inherit from `metapp::DeclareMetaTypeBase`.   

Function `metapp::getMetaType` converts `metapp::DeclareMetaType` to non-template class `metapp::MetaType`.  

For example,  

```c++
// Assume the type is MyClass
class MyClass { blah blah };

template <>
struct metapp::DeclareMetaType <MyClass>
	: metapp::DeclareMetaTypeBase <MyClass>
{
}
```

The members in DeclareMetaType that will be used by metapp are static constants, type definition, or static functions. All members are optional. If any member is missed, metapp will use default implementation.  

## Members in DeclareMetaType

#### Constant typeKind
```c++
template <>
struct metapp::DeclareMetaType <MyClass>
	: metapp::DeclareMetaTypeBase <MyClass>
{
	static constexpr metapp::TypeKind typeKind = metapp::tkUser + 1;
}
```

The default value of `typeKind` is `metapp::tkObject`.  

Each `MetaType` always has one `TypeKind`. TypeKind is a 16 bit integer that represents the meta type. For any user defined TypeKind, the value must be equal or greater than `metapp::tkUser`.  

#### Function cast

```c++
static bool cast(Variant * result, const Variant & value, const MetaType * toMetaType);
```

Function `cast` return true if the `value` can be type casted to `toMetaType`, otherwise returns false.  
If `result` is not nullptr and if the `value` can be casted, set `result` with the casted value.  
If the implementation can't cast the value, it should return the value of default implementation, `CommonDeclareMetaType<T>::cast(result, value, toMetaType)`.  

```c++
template <>
struct metapp::DeclareMetaType <MyClass>
	: metapp::DeclareMetaTypeBase <MyClass>
{
	static bool cast(Variant * result, const Variant & value, const MetaType * toMetaType)
	{
		if(can cast MyClass to toMetaType) {
			*result = casted value;
			return true;
		}
		return CommonDeclareMetaType<MyClass>::cast(result, value, toMetaType);
	}
}
```

#### Function setup
```c++
static void setup();
```

Function `setup` is invoked on the first time when the MetaType is initialized. It will be called only once for one MetaType.  
`setup` is useful when the `DeclareMetaType` needs to do initialize work. One use case is to register inheritance relationship.  

**Example** 
struct A {};
struct B {};
struct C : A, B {};

template <>
struct metapp::DeclareMetaType <C>
	: metapp::DeclareMetaTypeBase <C>
{
	static void setup() {
		metapp::getMetaRepo()->registerBase<C, A, B>();
	}
}
```


## Members in DeclareMetaType, usually don't need to be re-implemented

#### Type UpType
```c++
template <>
struct metapp::DeclareMetaType <MyClass>
	: metapp::DeclareMetaTypeBase <MyClass>
{
	using UpType = TheUpTypeOfMyClass;
}
```

