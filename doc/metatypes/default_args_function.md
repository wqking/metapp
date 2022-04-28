# Meta type default arguments function (tkDefaultArgsFunction)

Template `metapp::DefaultArgsFunction` is used to create a defautl arguments function.  

```c++
template <typename FT>
struct DefaultArgsFunction
{
  DefaultArgsFunction(const Variant & func, std::vector<Variant> defaultArgs);
};
```
`FT` is the function prototype.  
The parameter `func` in the constructor is a callable Variant that implements `MetaCallable` interface.
It can be constructor, function pointer, member function pointer, etc. The C++ type of the callable doesn't
need to have default arguments. `DefaultArgsFunction` has no relationship to C++ default arguments function.  
The parameter `defaultArgs` is a vector of default arguments.  

`DefaultArgsFunction` is an adaptor to dedicate any invoking to `func`, and provide `func` any missing arguments from `defaultArgs`.

To deduce `FT`, we can use the non-member function to create `DefaultArgsFunction`,  

```c++
template <typename FT>
DefaultArgsFunction<FT> createDefaultArgsFunction(FT && func, std::vector<Variant> defaultArgs);
```

Now let's see how to use `DefaultArgsFunction`.

Assume we have this function

```c++
std::string docDefaultArgsFunc(const std::string & a, const int b)
{
  return a + std::to_string(b);
}
```

Then we can have,

```c++
metapp::Variant func(metapp::createDefaultArgsFunction(&docDefaultArgsFunc, { 5 }));
```

Call the function with 1 argument, 1 default argument

```c++
metapp::Variant result1 = metapp::callableInvoke(func, nullptr, "hello");
ASSERT(result1.get<const std::string &>() == "hello5");
```

Call the function with 2 arguments, no default argument

```c++
metapp::Variant result2 = metapp::callableInvoke(func, nullptr, "Good", 38);
ASSERT(result2.get<const std::string &>() == "Good38");
```
