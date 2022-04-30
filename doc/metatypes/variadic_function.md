# Meta type variadic function (tkVariadicFunction)

Template `metapp::VariadicFunction` is used to create a variadic function.  

```c++
template <typename FT>
struct VariadicFunction
{
  explicit VariadicFunction(const Variant & func);
};
```
`FT` is the function prototype.  
The parameter `func` in the constructor is a callable Variant that implements `MetaCallable` interface.
It can be constructor, function pointer, member function pointer, etc.  
The callable must have the prototype as,  

```c++
ResultType callable(const metapp::Variant * arguments, const size_t argumentCount)
```

The function can have any result type.  
Parameter `arguments` is a pointer to Variant. It contains the arguments array.    
Parameter `argumentCount` is the number of Variant in the array.  

The arguments are passed from lef to right order.

`VariadicFunction` is an adaptor to dedicate any invoking to `func`, and convert the arguments to the `arguments` array.

To deduce `FT`, we can use the non-member function to create `createVariadicFunction`,  

```c++
template <typename FT>
VariadicFunction<FT> createVariadicFunction(FT && func);
```

Now let's see how to use `VariadicFunction`.

Assume we have this function

```c++
int sum(const metapp::Variant * arguments, const size_t argumentCount)
{
  int total = 0;
  for(size_t i = 0; i < argumentCount; ++i) {
    total += arguments[i].cast<int>().get<int>();
  }
  return total;
}
```

Create the VariadicFunction Variant

```c++
metapp::Variant func = (metapp::createVariadicFunction(&sum));
```

Invoke func without arguments

```c++
ASSERT(metapp::callableInvoke(func, nullptr).get<int>() == 0);
```

Invoke func with 2 arguments

```c++
ASSERT(metapp::callableInvoke(func, nullptr, 6, 3).get<int>() == 9);
```

Invoke the function with 10 arguments, the arguments can have different type,
as long as the target function can process (cast) them.

```c++
ASSERT(metapp::callableInvoke(func, nullptr,
  10, 9.1, 8LL, 7, 6, 5, 4, 3, 2, 1).get<int>() == 55);
```