# Registered items

## Overview

When getting meta information from MetaRepo such as calling `MetaRep::getCallable()`, the returned value is a registered item, such as `RegisteredCallable`, or a `std::deque` of the registered items, such as `RegisteredCallableList`.  

The registered items allow to,  
1. Get the meta data name.
2. Get the underlying target (for `RegisteredCallable`, the target is the callable Variant).
3. Register and retrieve annotations.

All registered items inherit from `RegisteredAnnotation`. `RegisteredAnnotation` allow to register and retrieve annotations.  

All registered items have common member functions,  

#### getName

```c++
const std::string & getName() const
```

Returns the name. If the item doesn't have name, returns empty string.  
`RegisteredConstructor` doesn't have the `getName` function.  

#### getTarget

```c++
TargetType getTarget() const;
```

Returns the underlying data. `TargetType` is a placeholder here, it's different in different classes. For example, `TargetType` is `const Variant &` in `RegisteredCallable`, which is the `callable`.  

#### Implicitly type convert

```c++
operator TargetType () const
```

Implicitly casts the item to `TargetType`.  

#### isEmpty

```c++
bool isEmpty() const;
```

Returns true if the item is empty, that means the item doesn't contain any valid data.  
`MetaRepo` may return empty item if the meta data is not found in the repo.  

## Class RegisteredAccessible

#### Header

```c++
#include "metapp/registration/registeredaccessible.h"
```

#### API

```c++
class RegisteredAccessible : public RegisteredAnnotation
{
public:
	const std::string & getName() const;
	const Variant & getTarget() const;
	bool isEmpty() const;
	operator const Variant & () const;
};

using RegisteredAccessibleList = std::deque<RegisteredAccessible>;
```

`getTarget()` returns the accessible Variant.

## Class RegisteredCallable

#### Header

```c++
#include "metapp/registration/registeredcallable.h"
```

#### API

```c++
class RegisteredCallable : public RegisteredAnnotation
{
public:
	const std::string & getName() const;
	const Variant & getTarget() const;
	bool isEmpty() const;
	operator const Variant & () const;
};

using RegisteredCallableList = std::deque<RegisteredCallable>;
```

`getTarget()` returns the callable Variant.

## Class RegisteredConstructor

#### Header

```c++
#include "metapp/registration/registeredconstructor.h"
```

#### API

```c++
class RegisteredConstructor : public RegisteredAnnotation
{
public:
	const Variant & getTarget() const;
	bool isEmpty() const;
	operator const Variant & () const;
};

using RegisteredConstructorList = std::deque<RegisteredConstructor>;
```

`getTarget()` returns the callable (tkConstructor) Variant.  
Note: `RegisteredConstructor` doesn't have `getName()` function.  

## Class RegisteredRepo

#### Header

```c++
#include "metapp/registration/registeredrepo.h"
```

#### API

```c++
class RegisteredRepo : public RegisteredAnnotation
{
public:
	const std::string & getName() const;
	MetaRepo * getTarget() const;
	bool isEmpty() const;
	operator MetaRepo * () const;
};

using RegisteredRepoList = std::deque<RegisteredRepo>;
```

`getTarget()` returns the pointer to registered `MetaRepo`.  

## Class RegisteredType

#### Header

```c++
#include "metapp/registration/registeredtype.h"
```

#### API

```c++
class RegisteredType : public RegisteredAnnotation
{
public:
	const std::string & getName() const;
	const MetaType * getTarget() const;
	bool isEmpty() const;
	operator const MetaType * () const;
};

using RegisteredTypeList = std::deque<RegisteredType>;
```

`getTarget()` returns the pointer to registered `MetaType`.  

## Class RegisteredEnumValue

#### Header

```c++
#include "metapp/registration/registeredenumvalue.h"
```

#### API

```c++
class RegisteredEnumValue : public RegisteredAnnotation
{
public:
	using ValueType = long long;

	const std::string & getName() const;
	ValueType getTarget() const;
	bool isEmpty() const;
	operator ValueType () const;
};

using RegisteredEnumValueList = std::deque<RegisteredEnumValue>;
```

`getTarget()` returns the pointer to registered enum value as `ValueType (long long)`.  
`RegisteredEnumValue` is not returned by `MetaRepo`, it's returned by `MetaEnum`.  

## Base class RegisteredAnnotation

`RegisteredAnnotation` is the base class of all registered items. Thant means when you get a registered item, you get a `RegisteredAnnotation`.  
`RegisteredAnnotation` allows to register and retrieve annotations. Annotation is the extra information bound to a meta data. An annotation is a name/value pair. Name is `std::string`, value is `metapp::Variant`. That means the value can be any data.  

#### API

```c++
class RegisteredAnnotation
{
public:
	void addAnnotation(const std::string & name, const Variant & value);
	const Variant & getAnnotation(const std::string & name) const;
	const std::map<std::string, Variant> & getAllAnnotations() const;
};
```

For `getAnnotation`, if the annotation of `name` is not found, an empty Variant is returned (`Variant::isEmpty() is true`).
