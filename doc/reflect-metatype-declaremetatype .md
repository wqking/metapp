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

#### Function canCast

```c++
static bool canCast(const Variant & value, const MetaType * toMetaType)
```

Function `canCast` return true if the `value` can be type casted to `toMetaType`, false if it can't be casted.  
If the implementation can't cast the value, it should return the value of default implementation, `CommonDeclareMetaType<T>::canCast(value, toMetaType)`.  

**Example**  
```c++
template <>
struct metapp::DeclareMetaType <MyClass>
	: metapp::DeclareMetaTypeBase <MyClass>
{
	static bool canCast(const Variant & value, const MetaType * toMetaType)
	{
		if(can cast MyClass to toMetaType) {
			return true;
		}
		return CommonDeclareMetaType<MyClass>::canCast(value, toMetaType);
	}
}
```

#### Function cast

```c++
static Variant cast(const Variant & value, const MetaType * toMetaType);
```

Function `cast` return the casted Variant if the `value` can be type casted to `toMetaType`, otherwise returns empty `metapp::Variant()`.  
If the implementation can't cast the value, it should return the value of default implementation, `CommonDeclareMetaType<T>::cast(value, toMetaType)`.  

```c++
template <>
struct metapp::DeclareMetaType <MyClass>
	: metapp::DeclareMetaTypeBase <MyClass>
{
	static Variant cast(const Variant & value, const MetaType * toMetaType)
	{
		if(can cast MyClass to toMetaType) {
			return the value casted to toMetaType;
		}
		return CommonDeclareMetaType<MyClass>::cast(value, toMetaType);
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

