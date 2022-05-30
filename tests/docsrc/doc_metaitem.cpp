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
# MetaItem

## Overview

`MetaItem` represents the meta data registered in `MetaRepo` and `MetaClass`.  
When registering any meta data in `MetaRepo` and `MetaClass`, the register function returns a `MetaItem`.  
When retrieving any meta data from `MetaRepo` and `MetaClass`, the returned data is either `MetaItem` or `MetaItemView`.  

## Header

desc*/

//code
#include "metapp/metaitem.h"
//code

/*desc
## MetaItem member functions

#### getType

```c++
MetaItem::Type getType() const;
```

Returns the type of the `MetaItem`. Each `MetaItem` has one type.  
`MetaItem::Type` is a `enum class`.

```c++
enum class MetaItem::Type {
	none,
	accessible,
	callable,
	variable,
	constructor,
	metaRepo,
	metaType,
	enumValue
};
```

The values,  

**MetaItem::Type::none**  
The item doesn't have any valid type. `isEmpty()` will return true.  
Don't check this type, use `isEmpty()` instead.

**MetaItem::Type::accessible**  
The item is an accessible that implements MetaAccessible meta interface.  

**MetaItem::Type::callable**  
The item is a callable that implements MetaCallable meta interface.  

**MetaItem::Type::variable**  
The item is a variable.  

**MetaItem::Type::constructor**  
The item is a constructor.

**MetaItem::Type::metaRepo**  
The item is a MetaRepo.  

**MetaItem::Type::metaType**  
The item is a MetaType.  

**MetaItem::Type::enumValue**  
The item is a value in `MetaEnum`.  

#### isEmpty

```c++
bool isEmpty() const;
```

Returns true if the item doesn't hold any valid data.  

#### getName

```c++
const std::string & getName() const
```

Returns the name. If the item doesn't have name, returns empty string.  
`MetaItem::Type::constructor` always has empty name.  

#### asXxx functions

```c++
const Variant & asAccessible() const;
const Variant & asCallable() const;
const Variant & asVariable() const;
const Variant & asConstructor() const;
const MetaType * asMetaType() const;
const MetaRepo * asMetaRepo() const;
const Variant & asEnumValue() const;
```

Returns the proper data.  
If the type doesn't match the function, exception `metapp::WrongMetaTypeException` is raised.  

#### getTarget

```c++
const Variant & getTarget() const;
```

Return the underlying Variant of the meta item. It doesn't check if the type matches, and it doesn't convert the data
to approperiate type such as `const MetaRepo *`.  

#### Implicitly type convert

```c++
operator const Variant & () const;
```

Convert the item to Variant implicitly. This is useful to pass the meta item where a Variant is required. For example,  
desc*/

ExampleFunc
{
	//code
	metapp::MetaRepo metaRepo;
	metaRepo.registerCallable("funcForMetaItem", std::function<int()>([]() {
		return 5;
	}));
	const metapp::MetaItem & callable = metaRepo.getCallable("funcForMetaItem");
	//desc The first argument in `metapp::callableInvoke` is `metapp::Variant`,
	//desc we can pass `metapp::MetaItem` to it the item will convert to `metapp::Variant`.
	const metapp::Variant result = metapp::callableInvoke(callable, nullptr);
	ASSERT(result.get<int>() == 5);

	//desc We can also get the Variant from the item explicitly
	const metapp::Variant result2 = metapp::callableInvoke(callable.asCallable(), nullptr);
	ASSERT(result2.get<int>() == 5);
	//code
}

/*desc
#### registerAnnotation

```c++
void registerAnnotation(const std::string & name, const Variant & value);
```

Register an annotation `value` under `name`.  
An annotation is any data. `metapp` doesn't use the data.  
The user can use the annotation for any purpose.
For example, if `metapp` is used in a property editor, an annotation may provide description,
or indicate a property should be hidden from the editor.

#### getAnnotation

```c++
const Variant & getAnnotation(const std::string & name) const;
```

Returns the annotaion for `name`. If `name` is not registered, an empty Variant is returned (Variant::isEmpty() is true).

#### getAllAnnotations

```c++
const std::map<std::string, Variant> & getAllAnnotations() const;
```

Returns all registered annotations in a `std::map`.

desc*/
