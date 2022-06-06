[//]: # (Auto generated file, don't modify this file.)

# Tutorial for invoking callables
<!--begintoc-->
- [Include headers](#mdtoc_e1acc517)
- [Invoke non-member function](#mdtoc_ca04390c)
- [Invoke class member function](#mdtoc_6b6c86f9)
- [Invoke std::function](#mdtoc_da835b5a)
- [Use overloaded functions](#mdtoc_16c84820)
- [Use default arguments](#mdtoc_7001e1e)
- [Use variadic function](#mdtoc_79d66ab1)
<!--endtoc-->

`MetaCallable` is the core meta interface for invoking callables
and get type information from a callable.  
Use the member function `MetaType::getMetaCallable()` to get the meta interface.
If the meta type doesn't implement the interface, `nullptr` is returned.

<a id="mdtoc_e1acc517"></a>
## Include headers
Header for MetaCallable

```c++
#include "metapp/interfaces/metacallable.h"
```

Header for metapp::Variant

```c++
#include "metapp/variant.h"
```

To use all declared meta types, include this header

```c++
#include "metapp/allmetatypes.h"
```

<a id="mdtoc_ca04390c"></a>
## Invoke non-member function

Let's see how to invoke non-member (free) function.
func1 is the function we are going to invoke.

```c++
inline std::string func1(const int n)
{
  return std::to_string(n);
}
```

v is pointer to func1

```c++
metapp::Variant v(&func1);
```

Prepare the arguments array

```c++
metapp::Variant arguments[] { 5 };
```

Invoke the callable, the nullptr is the object instance, for free function, it's nullptr

```c++
metapp::Variant result = v.getMetaType()->getMetaCallable()->invoke(v, nullptr, arguments);
ASSERT(result.get<std::string>() == "5");
```

Or we can use metapp::callableInvoke to pass the arguments directly

```c++
result = metapp::callableInvoke(v, nullptr, 38);
ASSERT(result.get<std::string>() == "38");
```

<a id="mdtoc_6b6c86f9"></a>
## Invoke class member function

Now let's invoke class member function

```c++
struct MyClass {
  int value;

  int add(const int delta1, const int delta2) const {
    return value + delta1 + delta2;
  }
};

metapp::Variant v(&MyClass::add);
MyClass obj { 5 };
// The second argument is the pointer to obj, it's required when invoking member function
metapp::Variant result = metapp::callableInvoke(v, &obj, 3, 9);
ASSERT(result.get<int>() == 17);
```

<a id="mdtoc_da835b5a"></a>
## Invoke std::function

We can also invoke `std::function`

```c++
metapp::Variant v(std::function<std::string (const std::string &, const std::string &)>(
  [](const std::string & a, const std::string & b) -> std::string {
    return a + b;
  }
));
metapp::Variant result = metapp::callableInvoke(v, nullptr, "Hello ", "world");
ASSERT(result.get<const std::string &>() == "Hello world");
```

<a id="mdtoc_16c84820"></a>
## Use overloaded functions

This example shows how to use `metapp::OverloadedFunction` to construct overloaded function explicitly.  
Usually we don't need to do so. When registering callable in `MetaRepo` or `MetaClass`, the functions
registered under the same will be combined to a `metapp::OverloadedFunction` automatically.

```c++
metapp::Variant callable = metapp::OverloadedFunction();
metapp::OverloadedFunction & overloadedFunction = callable.get<metapp::OverloadedFunction &>();
overloadedFunction.addCallable(std::function<int ()>([]() {
  return 1;
  }));
overloadedFunction.addCallable(std::function<int (int)>([](const int n) {
  return n * 2;
  }));
overloadedFunction.addCallable(std::function<int (long)>([](const long n) {
  return (int)n * 3;
  }));

ASSERT(metapp::callableInvoke(callable, nullptr).get<int>() == 1);

ASSERT(metapp::callableInvoke(callable, nullptr, 5).get<int>() == 10);

ASSERT(metapp::callableInvoke(callable, nullptr, 5L).get<int>() == 15);
```

<a id="mdtoc_7001e1e"></a>
## Use default arguments

We also support default arguments.  
`myDefaultArgsFunc` is the function we are going to invoke with default arguments.  
The C++ function doesn't need to have default argument.

```c++
std::string myDefaultArgsFunc(const int a, const bool b, const std::string & c)
{
  return std::to_string(a) + (b ? "true" : "false") + c;
}
```

Create a metapp::DefaultArgsFunction<FT>, the FT is deduced by `metapp::createDefaultArgsFunction`.  
The first argument is the function, it can be any meta callable, such as free function, member function, constructor, etc.  
The second argument is a list of default arguments, it must be specified
in the right to left order in the function prototype

```c++
metapp::Variant v(metapp::createDefaultArgsFunction(&myDefaultArgsFunc, { "hello", true }));
```

```c++
// Invoke the function, with 2 default arguments
ASSERT(metapp::callableInvoke(v, nullptr, 5).get<const std::string &>() == "5truehello");

// Invoke the function, with 1 default arguments
ASSERT(metapp::callableInvoke(v, nullptr, 38, false).get<const std::string &>() == "38falsehello");

// Invoke the function, with no default arguments
ASSERT(metapp::callableInvoke(v, nullptr, 19, false, "GOOD").get<const std::string &>() == "19falseGOOD");
```

<a id="mdtoc_79d66ab1"></a>
## Use variadic function

We can also use variadic function.  
The function must accept two arguments, the first is a pointer to Variant, which contains
the arguments array. The second argument is the number of Variant in the array.

```c++
int myVariadicFunc(const metapp::ArgumentSpan & arguments)
{
  int total = 0;
  for(const auto & argument : arguments) {
    total += argument.cast<int>().get<int>();
  }
  return total;
}
```

```c++
metapp::Variant v(metapp::createVariadicFunction(&myVariadicFunc));

// Invoke the function with no arguments.
ASSERT(metapp::callableInvoke(v, nullptr).get<int>() == 0);

// Invoke the function with 2 arguments.
ASSERT(metapp::callableInvoke(v, nullptr, 2, 5).get<int>() == 7);

// Invoke the function with 10 arguments, the arguments can have different type,
// as long as the target function can process (cast) them.
ASSERT(metapp::callableInvoke(v, nullptr,
  10, 9.1, 8LL, 7, 6, 5, 4, 3, 2, 1).get<int>() == 55);
```
