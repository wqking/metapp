# Exception and multi-threading safety in metapp

## Multi-threading safety

In general the threading safety is similar to native C++ values (such as an `int n`). Reading is multi-threading safe, writing is not multi-threading safe.  
"Not multi-threading safe" doesn't only mean the underlying data is wrong, but also may cause crash.

### Reading is multi-threading safe

Examples,

1. Get/cast from `metapp::Variant` is safe.
2. `metapp::getMetaType` is safe.
3. Getting meta information from `MetaRepo` or `MetaClass` is safe.
4. Invoking `MetaCallable` is safe.
5. Reading values from meta interfaces such as `MetaIndexable` is safe for types implemented by metapp. Whether it's safe for other types depends on how the interfaces are implemented.
6. Use `DeclareMetaType` to declare meta type, and implement meta interfaces.

### Writing is not multi-threading safe

Examples,

1. Assigning values to the same `Variant` from different threads is not safe.
2. Registering meta information to the same `MetaRepo` or `MetaClass` from different threads is not safe.

### Special notes on MetaRepo and MetaRepoList

It's not safe when constructing or destroying a `MetaRepo` and at the same time the other threads using any functions from `MetaRepoList`.  
metapp internal uses `MetaRepoList` implicitly, such as performing Variant casting, or getting meta information from `MetaClass`.  

The common use case is that an application constructs and fills all `MetaRepo` instances before the other parts start running, this will avoid the multi-threading problem. And most applications may have only one `MetaRepo` instance as global variable or member in a singleton data such as global application object.  
In brief, unless `MetaRepo` is abused, the multi-threading unsafety should not be a problem.  

Here is a deeper inside the multi-threading unsafety. In `MetaRepo` constructor and destructor, the `MetaRepo` adds it's raw pointer to the global `MetaRepoList`. `MetaRepoList` then adds the pointer to a linked list. All public functions will access the linked list. So constructing/destroying a `MetaRepo` will modify the linked list, and public functions in `MetaRepoList` will read the linked list, that's not multi-threading safe.  
It's possible to make it multi-threading safe with cost of performance. Since we should not abuse constructing/destroying `MetaRepo` so the problem is avoidable, we don't need to pay the cost to make it multi-threading safe.

## Exception safety and exceptions

### Exception safety

I try to make all operations to guarantee strong exception safety, which means the underlying data remains original value on exception is thrown.  

### metapp throws exceptions

metapp throws exceptions when an action can't be finished correctly, such as casting a Variant to an un-castable type.  

### Exceptions can be disabled (with risk)

If exceptions are disabled in C++ compiler settings, metapp will not throw any exceptions, it just uses objects without any valid data. For example, a failed casting will return an empty Variant. Another example, when invoking a callable, if any argument can't cast to the target type, an empty Variant will be passed to the callable and the underlying function will receive wrong argument.  

If you are going to disable exception, you must ensure the actions will not fail. For example, you must ensure all arguments can cast to the callable arguments types before invoking the callable.

## metapp exception classes reference

All exceptions are in header `metapp/exception.h`.  
All metapp exception classes inherit from `metapp::MetaException`. `MetaException` inherits from `std::runtime_error`.  

#### UnsupportedException

Thrown when,

- Streaming operators (<< or >>) are used on Variant which underlying type doesn't support streaming.  

#### BadCastException

Thrown when,

- `Variant::get` can't get the type properly. For example, a Variant holds an `int`, and gets `double` from it.
- `Variant::cast` can't cast the type properly.
- Invoking `MetaCallable` with un-castable arguments, `MetaCallable` uses `Variant::cast` to cast the arguments.
- The functions `cast`, `castToBase`, `castToDerived` in `MetaRepo` when the object instance can't be casted to the target class type.

#### IllegalArgumentException

Thrown when,

- Invoking `MetaCallable` with wrong number or arguments.
- Invoking a `MetaCallable` of overload functions, but it can't find proper function to invoke (no function matches the given arguments).

#### WrongMetaTypeException

Thrown when,

- `MetaClass::registerConstructor` but the constructor is not `MetaCallable`.
- `MetaRepo::registerCallable` but the callable is not `MetaCallable`.
- `MetaRepo::registerAccessible` but the accessible is not `MetaAccessible`.
- The functions `asXXX` in `MetaItem`, the underlying type doesn't match `XXX`.

#### OutOfRangeException

Thrown when,

- Getting/setting values in `MetaIndexable` which index is out of range.

#### UnwritableException

Thrown when,

- When setting value to an unwritable object. For example, setting value to a readonly accessible, setting value to `MetaIndexable` of a const object, etc.

#### NotConstructibleException

Thrown when,

- Constructing Variant or call `MetaType::construct/copyConstruct/placementConstruct/placementCopyConstruct`. If the object can't be constructed properly (such as there is no proper ctor), or the object can't be copied when it's requested to copy, or the object can't be moved when it's requested to move.

