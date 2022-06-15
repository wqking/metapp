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
# Use MetaRepo to register meta types at running time

## Overview

`MetaRepo` is used to register and retrieve meta data and meta types at running time.
The registered meta types can be any C++ types, no matter if they are declared with [DeclareMetaType](declaremetatype.md) or not.  

metapp doesn't provide global `MetaRepo` object because metapp tries to avoid global data whenever possible.  
It's up to you to decide how to register the meta data. You can either define global `MetaRepo` object, or use local
stack based `MetaRepo`, or put `MetaRepo` in some global singleton, such as a global `Application` object that most GUI
program has.

## Header

```c++
#include "metapp/metarepo.h"
```

## Constructor and destructor

```c++
MetaRepo();
~MetaRepo();
```

Note: caution needs to be taken when constructing or destroying `MetaRepo` in multi-threading application.  
Please read [Exception and thread safety](exception_thread_safety.md) for details.

## MetaRepo member functions for registering meta data

#### registerAccessible

```c++
MetaItem & registerAccessible(const std::string & name, const Variant & field);
```

Register an accessible.
The parameter `name` is the accessible name. The accessible can be got from the MetaRepo by the name later.
If a accessible with the same name has already registered, `registerAccessible` doesn't register the new accessible
and returns the previous registered field.  
The parameter `accessible` is a Variant of MetaType that implements meta interface `MetaAccessible`.  
The returned `MetaItem` can be used to add annotations to the meta data.  

#### registerCallable

```c++
MetaItem & registerCallable(const std::string & name, const Variant & callable);
```
Register a callable.  
The parameter `name` is the callable name. metapp allows multiple methods be registered under the same name,, they are treated as overloaded methods.  
The parameter `callable` is a Variant of MetaType that implements meta interface `MetaCallable`. It can be a pointer to free function, or even `std::function`.  
The returned `MetaItem` can be used to add annotations to the meta data.  

#### registerVariable

```c++
MetaItem & registerVariable(const std::string & name, const Variant & variable);
```

Register a variable.  
The parameter `name` is the variable name.  
The parameter `variable` is a Variant of any value.  
The returned `MetaItem` can be used to add annotations to the meta data.  

The difference between `accessible` and `variable` is, an `accessible` must implement meta interface `MetaAccessible`, while
a `variable` can be any value. How to use a `variable` is up to the user.  
The best practice to decide when to use `accessible` or `variable` is, when a Variant will be got/set value via an accessible
such as a pointer to a variable, register it as `accessible`. If a Variant's value is not going to change, such as a constant,
register it as `variable`.  
Note: I'm not satisfied with the term `variable`. I've thought about constant, object, value, item, element, but none is satisfying.

#### registerType

```c++
template <typename T>
MetaItem & registerType(const std::string & name = ""); // #1
	return registerType(name, getMetaType<T>());
}
MetaItem & registerType(std::string name, const MetaType * metaType); // #2
```

Register a MetaType.  
The #1 form is equivalent to `registerType(name, getMetaType<T>())`;  
If the parameter `name` is empty, the function tries to get the name from built-in types. If the name is not found, then the name is not used and the MetaType can't be got by name.  
The returned `MetaItem` can be used to add annotations to the meta data.  

#### registerRepo, simulate namespace

```c++
MetaItem & registerRepo(const std::string & name, Variant repo = Variant());
```

Register a sub MetaRepo.  
If the parameter `repo` is empty, `registerRepo` creates a new MetaRepo and register it under the `name`.
The created `MetaRepo` can be got via `MetaItem::getTarget()` in the returned value.  
If the parameter `repo` is not empty, `repo` should be a value, reference, pointer, or smart pointer of `MetaRepo`.  
The returned `MetaItem` can be used to add annotations to the meta data.  
Note: registering a MetaRepo can simulate namespace. A MetaRepo can be treated as a namespace.  
Note: if the argument `repo` is a pointer or reference, the caller needs to ensure the `repo` is live while `this` repo is live.


## MetaRepo member functions for retrieving meta data

#### getAccessible

```c++
const MetaItem & getAccessible(const std::string & name) const;
```

Get an accessible of `name`. If the accessible is not registered, an empty MetaItem is returned (MetaItem::isEmpty() is true).  

#### getAccessibleView

```c++
MetaItemView getAccessibleView() const;
```

Returns a MetaItemView for all registered accessibles.  

#### getCallable

```c++
const MetaItem & getCallable(const std::string & name) const;
```

Get a callable of `name`. If the callable is not registered, an empty MetaItem is returned (MetaItem::isEmpty() is true).  

#### getCallableView

```c++
MetaItemView getCallableView() const;
```

Returns a MetaItemView for all registered callables.  

#### getVariable

```c++
const MetaItem & getVariable(const std::string & name) const;
```

Get a variable of `name`. If the variable is not registered, an empty MetaItem is returned (MetaItem::isEmpty() is true).  

#### getVariableView

```c++
MetaItemView getVariableView() const;
```

Returns a MetaItemView for all registered constants.  

#### getType by name

```c++
const MetaItem & getType(const std::string & name) const;
```

Get a MetaItem of `name`. If the type name is not registered, an empty MetaItem is returned (MetaItem::isEmpty() is true).  

#### getType by type kind

```c++
const MetaItem & getType(const TypeKind kind) const;
```

Get a MetaItem of `kind`. If the type kind is not registered, an empty MetaItem is returned (MetaItem::isEmpty() is true).  

#### getType by MetaType

```c++
const MetaItem & getType(const MetaType * metaType) const;
```

Get a MetaItem of `metaType`. If the meta type is not registered, an empty MetaItem is returned (MetaItem::isEmpty() is true).   
This function is useful to get the name of a registered meta type.

#### getTypeView

```c++
MetaItemView getTypeView() const;
```

Returns a MetaItemView for all registered types.  

#### getRepo

```c++
const MetaItem & getRepo(const std::string & name) const;
```

Get a registered sub MetaRepo by `name`. If the repo name is not registered, an empty MetaItem is returned (MetaItem::isEmpty() is true).  

#### getRepoView

```c++
MetaItemView getRepoView() const;
```

Returns a MetaItemView for all registered repos.  


## MetaRepo member functions for class hierarchy

Unlike some other reflection libraries that class hierarchy information is bound to the class type, metapp separates class hierarchy from MetaClass. The benefit is that class hierarchy information can be used without declaring MetaClass.  
We can register and get class hierarchy information in `MetaRepo`.  

#### enum MetaRepo::Relationship

```c++
enum class Relationship
{
	none,
	base,
	derived
};
```

Enum for relationship between two classes. The relationship can be got via member function `getRelationship()`.    
Given class A and B, `Relationship::none` means A and B are not derived from each other. `Relationship::base` means B is the base class of A, or to say, A derives from B. `Relationship::derived` means B derives from A, or to say, A is the base class of B.  

#### registerBase

```c++
template <typename Class, typename ...Bases>
void registerBase();
```

Register base classes of a class.  
Template parameter `Class` is the derived class, `Bases` are the base classes.  

#### getBases

```c++
template <typename Class>
BaseView getBases() const;

BaseView getBases(const MetaType * classMetaType) const;
```

Returns meta types of all base class for `Class`, or for `classMetaType`.  
The first templated form is similar to `getBases(metapp::getMetaType<remove all cv and reference of Class>())`.

#### getDeriveds

```c++
template <typename Class>
BaseView getDeriveds() const;

BaseView getDeriveds(const MetaType * classMetaType) const;
```

Returns meta types of all derived class for `Class`, or for `classMetaType`.  
The first templated form is similar to `getDeriveds(metapp::getMetaType<remove all cv and reference of Class>())`.

#### castToBase

```c++
template <typename Class>
void * castToBase(void * instance, const int baseIndex) const;

void * castToBase(void * instance, const MetaType * classMetaType, const int baseIndex) const;
```

Casts `instance` of `Class`, or of `classMetaType`, to its base class at `baseIndex` in the base list returned by `getBases`, then returns the casted pointer.  
The first templated form is similar to `castToBase(instance, metapp::getMetaType<remove all cv and reference of Class>(), baseIndex)`.

#### castToDerived

```c++
template <typename Class>
void * castToDerived(void * instance, const int derivedIndex) const;

void * castToDerived(void * instance, const MetaType * classMetaType, const int derivedIndex) const;
```

Casts `instance` of `Class`, or of `classMetaType`, to its derived class at `derivedIndex` in the derived list returned by `getDeriveds`, then returns the casted pointer.  
The first templated form is similar to `castToDerived(instance, metapp::getMetaType<remove all cv and reference of Class>(), derivedIndex)`.

#### cast

```c++
template <typename Class, typename ToClass>
void * cast(void * instance) const;

void * cast(void * instance, const MetaType * classMetaType, const MetaType * toMetaType) const;
```

Casts `instance` of `Class`, or of `classMetaType`, to `ToClass`, or `toMetaType`, then returns the casted pointer.  
The first templated form is similar to `cast(instance, metapp::getMetaType<remove all cv and reference of Class>(), metapp::getMetaType<remove all cv and reference of ToClass>())`.  

`Class` can be parent or ancient class of `ToClass`, or child or any depth grandson class of `ToClass`.  
If `Class` and `ToClass` doesn't have deriving relationship, `nullptr` is returned.  

#### getRelationship

```c++
template <typename Class, typename ToClass>
MetaRepo::Relationship getRelationship() const;

MetaRepo::Relationship getRelationship(const MetaType * classMetaType, const MetaType * toMetaType) const;
```

Get relationship between `Class`, or `classMetaType`, and `ToClass`, or `toMetaType`.  
The first templated form is similar to `getRelationship(metapp::getMetaType<remove all cv and reference of Class>(), metapp::getMetaType<remove all cv and reference of ToClass>())`.  

Enum `MetaRepo::Relationship` has 3 values,  
`MetaRepo::Relationship::none`: `Class` and `ToClass` doesn't derive from each other, they don't have relationship.  
`MetaRepo::Relationship::base`: `ToClass` is base class of `Class`, or to say, `Class` derives from `ToClass`.  
`MetaRepo::Relationship::derived`: `ToClass` derives from `Class`, or to say, `Class` is base class of `ToClass`.

#### isClassInHierarchy

```c++
template <typename Class>
bool isClassInHierarchy() const;

bool isClassInHierarchy(const MetaType * classMetaType) const;
```

Returns true if `Class`, or `classMetaType`, is registered as base or derived class.  
The first templated form is similar to `isClassInHierarchy(metapp::getMetaType<remove all cv and reference of Class>())`.  

#### traverseBases

```c++
template <typename FT>
bool traverseBases(const MetaType * classMetaType, FT && callback) const;
```

Traverses all base classes of `classMetaType`, and call `callback` on each meta type.  
It's guaranteed duplicated meta types are only passed to `callback` once.  
The first meta type is always `classMetaType`. That's to say, even if there is no any base classes, `callback` will still
be invoked with `classMetaType`.  
`callback` prototype is `bool callback(const MetaType * metaType)`. The argument `metaType` is the class current traversing at.
If `callback` returns false, the traversing stops.  
`traverseBases` returns true if all calls on `callback` returns true, returns false if all calls on `callback` returns false.

## Class MetaRepoList

`MetaRepoList` is a singleton class that holds all live `MetaRepo` instances.  
We can perform common functions on `MetaRepoList` which functions are usually from `MetaRepo`.  
Using `MetaRepoList` can release the dependence on a single global `MetaRepo` instance.  
To get the singleton, call function `metapp::getMetaRepoList`,

```c++
const MetaRepoList * getMetaRepoList();
```

When a `MetaRepo` is constructed, no matter it's on heap or on stack, it's added to the global `MetaRepoList` automatically.  
When a `MetaRepo` is destroyed, it's removed from the global `MetaRepoList` automatically.

## MetaRepoList member functions

#### begin() and end()

```c++
iterator begin() const;
iterator end() const;
```

Returns iterator to the begin or end of the list. These two functions allow to loop over the `MetaRepoList`.  
The `iterator` is forward iterator.  

#### findMetaRepoForHierarchy

```c++
const MetaRepo * findMetaRepoForHierarchy(const MetaType * classMetaType) const;
```

Returns a pointer to the `MetaRepo` in which the `classMetaType` is registered using `MetaRepo::registerBase`.  
If the `classMetaType` was not registered to any `MetaRepo`, nullptr is returned.  

#### traverseBases

```c++
template <typename FT>
bool traverseBases(const MetaType * classMetaType, FT && callback) const;
```

Similar to `MetaRepo::traverseBases`, this function finds the `MetaRepo` in which the `classMetaType`
is registered using `MetaRepo::registerBase`, then call `MetaRepo::traverseBases` on the found `MetaRepo`.  
If no `MetaRepo` is found, `callback` is invoked with `classMetaType` as if there is no any base classes.  
If `callback` returns false, the traversing stops.  
`traverseBases` returns true if all calls on `callback` returns true, returns false if all calls on `callback` returns false.

desc*/
