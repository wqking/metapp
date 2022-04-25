# Meta type constructor (tkConstructor)

Template `metapp::Constructor` is used to create a constructor.  

```c++
template <typename T>
struct Constructor
{
};
```
`T` is the constructor function prototype, the return type must be the class.

Assume we have `MyClass`,  

```c++
class MyClass
{
public:
	MyClass(const std::string & a, const int b) {}

	void sayHello() {}
};
```

Then we can have,

```c++
metapp::Variant ctor = metapp::Constructor<MyClass (const std::string &, const int)>();
```

Then we can call the constructor, a constructor is a callable which always allocates an object and returns the instance pointer.  

```c++
metapp::Variant instance(metapp::callableInvoke(ctor, nullptr, "hello", 5));
```

Now `instance` contains a pointer to MyClass, we can use it to invoke any functions in MyClass such as `instance.get<MyClass *>()->sayHello();`.

Since `instance` only contains a pointer, so the `Variant` doesn't manage the instance lifetime, and it doesn't free the instance. We must delete the instance explicitly.  

```c++
delete instance.get<MyClass *>();
```

Explicitly deleting is not elegant and not exception safe. We can use `Variant::takeFrom` to convert the instance pointer a `Variant` managed object.  

```c++
metapp::Variant object = metapp::Variant::takeFrom(metapp::callableInvoke(ctor, nullptr, "hello", 5));
```

Now `object` contains a MyClass object (not pointer), and when `object` is freed, the MyClass object is freed too, so we don't need to delete the object.  
To call `sayHello()`, we can write `object.get<MyClass &>().sayHello()`.  
