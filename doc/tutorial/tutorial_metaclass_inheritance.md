[//]: # (Auto generated file, don't modify this file.)

# Tutorial for using class inheritance

## Define the C++ classes to reflect for

Here is the class hierarchy we are going to reflect for.

```c++
struct Parent1
{
  // make some data to make derived class has `this` pointer at different address
  char p1[4];
};

struct Parent2
{
  char p2[16];
};

struct Son1 : Parent1
{
  char s1[5];
};

struct Son2 : Parent1, Parent2
{
  char s2[2];
};

struct Grandson1 : Son1
{
  char g1[35];
};

struct Grandson2 : Son1, Parent2
{
  char g2[50];
};
```

If we declare meta type (DeclareMetaType) for any classes, we can register the base classes in `setup` function.  
`setup` is a static function in DeclareMetaType. It will be called when the meta type is used,
and it's guaranteed to be called only once even in multi threading.  
`setup` is a good place to register the base classes.

```c++
template <>
struct metapp::DeclareMetaType <Son1> : metapp::DeclareMetaTypeBase <Son1>
{
  static void setup() {
    metapp::getMetaRepo()->registerBase<Son1, Parent1>();
  }
};
```

If we don't declare any meta type, or we don't want to use `setup`,
we can register the base classes anywhere in the code.

```c++
metapp::getMetaRepo()->registerBase<Son2, Parent1, Parent2>();
metapp::getMetaRepo()->registerBase<Grandson1, Son1>();
metapp::getMetaRepo()->registerBase<Grandson2, Son1, Parent2>();
```

Now let's retrieve the hierarchy information.

```c++
metapp::BaseView baseBaseViewSon1 = metapp::getMetaRepo()->getBases(metapp::getMetaType<Son1>());
ASSERT(baseBaseViewSon1.size() == 1);
ASSERT(baseBaseViewSon1.at(0) == metapp::getMetaType<Parent1>());

metapp::BaseView baseBaseViewGrandson2 = metapp::getMetaRepo()->getBases<Grandson2>();
ASSERT(baseBaseViewGrandson2.size() == 2);
ASSERT(baseBaseViewGrandson2.at(0) == metapp::getMetaType<Son1>());
ASSERT(baseBaseViewGrandson2.at(1) == metapp::getMetaType<Parent2>());

Son2 son2;
ASSERT(metapp::getMetaRepo()->castToBase<Son2>(&son2, 0) == static_cast<Parent1 *>(&son2));
ASSERT(metapp::getMetaRepo()->castToBase<Son2>(&son2, 1) == static_cast<Parent2 *>(&son2));

Son1 son1;
ASSERT(metapp::getMetaRepo()->castToDerived<Son1>(&son1, 0) == static_cast<Grandson1 *>(&son1));
```