# Meta type constructor (tkConstructor)

Template `metapp::Constructor` is used to create a constructor.  

```c++
template <typename T>
struct Constructor
{
};
```
`T` is the constructor function prototype, the return type must be the class.

Assume we have `DcClass`,  

```c++
class DcClass
{
public:
  DcClass(const std::string & a, const int b) {}

  std::string greeting(const std::string & message) {
    return "Hello, " + message;
  }
};
```

Then we can have,

```c++
metapp::Variant ctor = metapp::Constructor<DcClass (const std::string &, const int)>();
```

Then we can call the constructor, a constructor is a callable which always allocates an object
and returns the instance pointer.  

```c++
metapp::Variant instance(metapp::callableInvoke(ctor, nullptr, "hello", 5));
```

Now `instance` contains a pointer to DcClass, we can use it to invoke any functions in DcClass, for example,

```c++
ASSERT(instance.get<DcClass *>()->greeting("metapp") == "Hello, metapp");
```

Since `instance` only contains a pointer, so the `Variant` doesn't manage the instance lifetime,
and it doesn't free the instance. We must delete the instance explicitly.  

```c++
delete instance.get<DcClass *>();
```

Explicitly deleting is not elegant nor exception safe. We can use `Variant::takeFrom` to convert
the instance pointer to a `Variant` managed object.  

```c++
metapp::Variant object = metapp::Variant::takeFrom(metapp::callableInvoke(ctor, nullptr, "hello", 5));
```

Now `object` contains a DcClass object (not pointer), and when `object` is freed,
the DcClass object is freed too, so we don't need to delete the object.  
To call `greeting()`, we can write,  

```c++
ASSERT(object.get<DcClass &>().greeting("world") == "Hello, world");
```