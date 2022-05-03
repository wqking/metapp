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

#include "metapp/allmetatypes.h"

/*desc
# Registered items

## Overview

When getting meta information from MetaRepo such as calling `MetaRep::getCallable()`, the returned value is a registered item, such as `MetaItem`, or a `std::deque` of the registered items, such as `MetaItemList`.  

The registered items allow to,  
1. Get the meta data name.
2. Get the underlying target (for `MetaItem`, the target is the callable Variant).
3. Register and retrieve annotations.

All registered items inherit from `RegisteredAnnotation`. `RegisteredAnnotation` allow to register and retrieve annotations.  

All registered items have common member functions,  

#### getName

```c++
const std::string & getName() const
```

Returns the name. If the item doesn't have name, returns empty string.  
`MetaItem` doesn't have the `getName` function.  

#### getTarget

```c++
TargetType getTarget() const;
```

Returns the underlying data. `TargetType` is a placeholder here, it's different in different classes. For example, `TargetType` is `const Variant &` in `MetaItem`, which is the `callable`.  

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

## Class MetaItem

#### Header

```c++
#include "metapp/registration/registeredaccessible.h"
```

#### API

```c++
class MetaItem : public RegisteredAnnotation
{
public:
	const std::string & getName() const;
	const Variant & getTarget() const;
	bool isEmpty() const;
	operator const Variant & () const;
};

using MetaItemList = std::deque<MetaItem>;
```

`getTarget()` returns the accessible Variant.

## Class MetaItem

#### Header

```c++
#include "metapp/registration/registeredcallable.h"
```

#### API

```c++
class MetaItem : public RegisteredAnnotation
{
public:
	const std::string & getName() const;
	const Variant & getTarget() const;
	bool isEmpty() const;
	operator const Variant & () const;
};

using MetaItemList = std::deque<MetaItem>;
```

`getTarget()` returns the callable Variant.

## Class MetaItem

#### Header

```c++
#include "metapp/registration/registeredconstructor.h"
```

#### API

```c++
class MetaItem : public RegisteredAnnotation
{
public:
	const Variant & getTarget() const;
	bool isEmpty() const;
	operator const Variant & () const;
};

using MetaItemList = std::deque<MetaItem>;
```

`getTarget()` returns the callable (tkConstructor) Variant.  
Note: `MetaItem` doesn't have `getName()` function.  

## Class MetaItem

#### Header

```c++
#include "metapp/registration/registeredrepo.h"
```

#### API

```c++
class MetaItem : public RegisteredAnnotation
{
public:
	const std::string & getName() const;
	MetaRepo * getTarget() const;
	bool isEmpty() const;
	operator MetaRepo * () const;
};

using MetaItemList = std::deque<MetaItem>;
```

`getTarget()` returns the pointer to registered `MetaRepo`.  

## Class MetaItem

#### Header

```c++
#include "metapp/registration/registeredtype.h"
```

#### API

```c++
class MetaItem : public RegisteredAnnotation
{
public:
	const std::string & getName() const;
	const MetaType * getTarget() const;
	bool isEmpty() const;
	operator const MetaType * () const;
};

using MetaItemList = std::deque<MetaItem>;
```

`getTarget()` returns the pointer to registered `MetaType`.  

## Class MetaItem

#### Header

```c++
#include "metapp/registration/registeredenumvalue.h"
```

#### API

```c++
class MetaItem : public RegisteredAnnotation
{
public:
	using ValueType = long long;

	const std::string & getName() const;
	ValueType getTarget() const;
	bool isEmpty() const;
	operator ValueType () const;
};

using MetaItemList = std::deque<MetaItem>;
```

`getTarget()` returns the pointer to registered enum value as `ValueType (long long)`.  
`MetaItem` is not returned by `MetaRepo`, it's returned by `MetaEnum`.  

## Base class RegisteredAnnotation

`RegisteredAnnotation` is the base class of all registered items. Thant means when you get a registered item, you get a `RegisteredAnnotation`.  
`RegisteredAnnotation` allows to register and retrieve annotations. Annotation is the extra information bound to a meta data. An annotation is a name/value pair. Name is `std::string`, value is `metapp::Variant`. That means the value can be any data.  

#### API

```c++
class RegisteredAnnotation
{
public:
	void registerAnnotation(const std::string & name, const Variant & value);
	const Variant & getAnnotation(const std::string & name) const;
	const std::map<std::string, Variant> & getAllAnnotations() const;
};
```

For `getAnnotation`, if the annotation of `name` is not found, an empty Variant is returned (`Variant::isEmpty() is true`).

desc*/
