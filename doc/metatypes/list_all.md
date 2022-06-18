[//]: # (Auto generated file, don't modify this file.)

# List of all built-in meta types
<!--begintoc-->
- [List of type kinds, equivalent C++ types and implemented meta interfaces](#mdtoc_2abe77cd)
- [List of cast-abilities](#mdtoc_7f714096)
- [List of UpTypes](#mdtoc_99a0987)
<!--endtoc-->

<a id="mdtoc_2abe77cd"></a>
## List of type kinds, equivalent C++ types and implemented meta interfaces

|TypeKind              |Value     |C++ type                                                                                                                                                                                                                              |Implemented meta interfaces           |
|----------------------|----------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|--------------------------------------|
|tkVoid                |0         |void                                                                                                                                                                                                                                  |None                                  |
|tkBool                |1         |bool                                                                                                                                                                                                                                  |None                                  |
|tkChar                |2         |char                                                                                                                                                                                                                                  |None                                  |
|tkWideChar            |3         |wchar_t                                                                                                                                                                                                                               |None                                  |
|tkChar8 (C++20)       |4         |char8_t                                                                                                                                                                                                                               |None                                  |
|tkChar16              |5         |char16_t                                                                                                                                                                                                                              |None                                  |
|tkChar32              |6         |char32_t                                                                                                                                                                                                                              |None                                  |
|tkSignedChar          |7         |signed char                                                                                                                                                                                                                           |None                                  |
|tkUnsignedChar        |8         |unsigned char                                                                                                                                                                                                                         |None                                  |
|tkShort               |9         |short                                                                                                                                                                                                                                 |None                                  |
|tkUnsignedShort       |10        |unsigned short                                                                                                                                                                                                                        |None                                  |
|tkInt                 |11        |int                                                                                                                                                                                                                                   |None                                  |
|tkUnsignedInt         |12        |unsigned int                                                                                                                                                                                                                          |None                                  |
|tkLong                |13        |long                                                                                                                                                                                                                                  |None                                  |
|tkUnsignedLong        |14        |unsigned long                                                                                                                                                                                                                         |None                                  |
|tkLongLong            |15        |long long                                                                                                                                                                                                                             |None                                  |
|tkUnsignedLongLong    |16        |unsigned long long                                                                                                                                                                                                                    |None                                  |
|tkFloat               |17        |float                                                                                                                                                                                                                                 |None                                  |
|tkDouble              |18        |double                                                                                                                                                                                                                                |None                                  |
|tkLongDouble          |19        |long double                                                                                                                                                                                                                           |None                                  |
|tkObject              |40        |None                                                                                                                                                                                                                                  |None                                  |
|tkPointer             |41        |T *                                                                                                                                                                                                                                   |MetaAccessible                        |
|tkReference           |42        |T &                                                                                                                                                                                                                                   |None                                  |
|tkFunction            |43        |// function pointer<br />R (*)(T1, T2, T3,...)<br />// function type<br />R (T1, T2, T3,...)                                                                                                                                          |MetaCallable                          |
|tkMemberFunction      |44        |// member function<br />R (C::*)(T1, T2, T3,...)                                                                                                                                                                                      |MetaCallable                          |
|tkMemberPointer       |45        |// member data<br />T C::*                                                                                                                                                                                                            |MetaAccessible                        |
|tkConstructor         |46        |// Class constructor.<br />metapp::Constructor                                                                                                                                                                                        |MetaCallable<br />MetaMember          |
|tkOverloadedFunction  |47        |// Overload functions<br />metapp::OverloadedFunction                                                                                                                                                                                 |MetaCallable                          |
|tkDefaultArgsFunction |48        |// Function with default arguments<br />metapp::DefaultArgsFunction                                                                                                                                                                   |MetaCallable                          |
|tkVariadicFunction    |49        |// Variadic function<br />metapp::VariadicFunction                                                                                                                                                                                    |MetaCallable                          |
|tkArray               |50        |T[]                                                                                                                                                                                                                                   |MetaIndexable                         |
|tkEnum                |51        |// C++ enum, or scoped enum.<br />enum A {};<br />enum class B {};                                                                                                                                                                    |None                                  |
|tkAccessor            |52        |metapp::Accessor                                                                                                                                                                                                                      |MetaAccessible                        |
|tkVariant             |53        |metapp::Variant                                                                                                                                                                                                                       |None                                  |
|tkMetaType            |54        |metapp::MetaType                                                                                                                                                                                                                      |None                                  |
|tkMetaRepo            |55        |metapp::MetaRepo                                                                                                                                                                                                                      |None                                  |
|tkStdString           |100       |std::string                                                                                                                                                                                                                           |None                                  |
|tkStdWideString       |101       |std::wstring                                                                                                                                                                                                                          |None                                  |
|tkStdSharedPtr        |102       |std::shared_ptr<T>                                                                                                                                                                                                                    |MetaAccessible<br />MetaPointerWrapper|
|tkStdUniquePtr        |103       |std::unique_ptr<T>                                                                                                                                                                                                                    |MetaAccessible<br />MetaPointerWrapper|
|tkStdWeakPtr          |104       |std::weak_ptr<T>                                                                                                                                                                                                                      |None                                  |
|tkStdFunction         |105       |std::function                                                                                                                                                                                                                         |MetaCallable                          |
|tkStdVector           |106       |std::vector<T, Allocator>                                                                                                                                                                                                             |MetaIndexable<br />MetaIterable       |
|tkStdList             |107       |std::list<T, Allocator>                                                                                                                                                                                                               |MetaIndexable<br />MetaIterable       |
|tkStdDeque            |108       |std::deque<T, Allocator>                                                                                                                                                                                                              |MetaIndexable<br />MetaIterable       |
|tkStdArray            |109       |std::array<T, length>                                                                                                                                                                                                                 |MetaIndexable<br />MetaIterable       |
|tkStdForwardList      |110       |std::forward_list<T, Allocator>                                                                                                                                                                                                       |MetaIterable                          |
|tkStdStack            |111       |std::stack<T, Container>                                                                                                                                                                                                              |None                                  |
|tkStdQueue            |112       |std::queue<T, Container>                                                                                                                                                                                                              |None                                  |
|tkStdPriorityQueue    |113       |std::priority_queue<T, Container>                                                                                                                                                                                                     |None                                  |
|tkStdMap              |114       |std::map<Key, T, Compare, Allocator>                                                                                                                                                                                                  |MetaIterable<br />MetaMappable        |
|tkStdMultimap         |115       |std::multimap<<br />&nbsp;&nbsp;&nbsp;&nbsp;    Key,<br />&nbsp;&nbsp;&nbsp;&nbsp;    T,<br />&nbsp;&nbsp;&nbsp;&nbsp;    Compare,<br />&nbsp;&nbsp;&nbsp;&nbsp;    Allocator<br />>                                                  |MetaIterable<br />MetaMappable        |
|tkStdSet              |116       |std::set<Key, Compare, Allocator>                                                                                                                                                                                                     |MetaIterable                          |
|tkStdMultiset         |117       |std::multiset<<br />&nbsp;&nbsp;&nbsp;&nbsp;    Key,<br />&nbsp;&nbsp;&nbsp;&nbsp;    Compare,<br />&nbsp;&nbsp;&nbsp;&nbsp;    Allocator<br />>                                                                                      |MetaIterable                          |
|tkStdUnorderedMap     |118       |std::unordered_map<<br />&nbsp;&nbsp;&nbsp;&nbsp;    Key,<br />&nbsp;&nbsp;&nbsp;&nbsp;    T,<br />&nbsp;&nbsp;&nbsp;&nbsp;    Hash,<br />&nbsp;&nbsp;&nbsp;&nbsp;    KeyEqual,<br />&nbsp;&nbsp;&nbsp;&nbsp;    Allocator<br />>     |MetaIterable<br />MetaMappable        |
|tkStdUnorderedMultimap|119       |std::unordered_multimap<<br />&nbsp;&nbsp;&nbsp;&nbsp;    Key,<br />&nbsp;&nbsp;&nbsp;&nbsp;    T,<br />&nbsp;&nbsp;&nbsp;&nbsp;    Hash,<br />&nbsp;&nbsp;&nbsp;&nbsp;    KeyEqual,<br />&nbsp;&nbsp;&nbsp;&nbsp;    Allocator<br />>|MetaIterable<br />MetaMappable        |
|tkStdUnorderedSet     |120       |std::unordered_set<<br />&nbsp;&nbsp;&nbsp;&nbsp;    Key,<br />&nbsp;&nbsp;&nbsp;&nbsp;    Hash,<br />&nbsp;&nbsp;&nbsp;&nbsp;    KeyEqual,<br />&nbsp;&nbsp;&nbsp;&nbsp;    Allocator<br />>                                         |MetaIterable                          |
|tkStdUnorderedMultiset|121       |std::unordered_multiset<<br />&nbsp;&nbsp;&nbsp;&nbsp;    Key,<br />&nbsp;&nbsp;&nbsp;&nbsp;    Hash,<br />&nbsp;&nbsp;&nbsp;&nbsp;    KeyEqual,<br />&nbsp;&nbsp;&nbsp;&nbsp;    Allocator<br />>                                    |MetaIterable                          |
|tkStdPair             |122       |std::pair<T1, T2>                                                                                                                                                                                                                     |MetaIndexable                         |
|tkStdTuple            |123       |std::tuple<Types...>                                                                                                                                                                                                                  |MetaIndexable<br />MetaIterable       |
|tkStdAny              |124       |std::any                                                                                                                                                                                                                              |None                                  |
|tkStdVariant          |125       |std::variant<Types...>                                                                                                                                                                                                                |None                                  |
|tkUser                |1024      |The start value of user defined meta type kinds.                                                                                                                                                                                      |None                                  |

<a id="mdtoc_7f714096"></a>
## List of cast-abilities

|TypeKind              |Cast to                                                                                           |
|----------------------|--------------------------------------------------------------------------------------------------|
|tkVoid                |None                                                                                              |
|tkBool                |All arithmetic types.                                                                             |
|tkChar                |Ditto                                                                                             |
|tkWideChar            |Ditto                                                                                             |
|tkChar8 (C++20)       |Ditto                                                                                             |
|tkChar16              |Ditto                                                                                             |
|tkChar32              |Ditto                                                                                             |
|tkSignedChar          |Ditto                                                                                             |
|tkUnsignedChar        |Ditto                                                                                             |
|tkShort               |Ditto                                                                                             |
|tkUnsignedShort       |Ditto                                                                                             |
|tkInt                 |Ditto                                                                                             |
|tkUnsignedInt         |Ditto                                                                                             |
|tkLong                |Ditto                                                                                             |
|tkUnsignedLong        |Ditto                                                                                             |
|tkLongLong            |Ditto                                                                                             |
|tkUnsignedLongLong    |Ditto                                                                                             |
|tkFloat               |Ditto                                                                                             |
|tkDouble              |Ditto                                                                                             |
|tkLongDouble          |Ditto                                                                                             |
|tkObject              |None                                                                                              |
|tkPointer             |Cast to pointer.<br />char * can cast to std::string.<br />wchar_t * can cast to std::wstring.    |
|tkReference           |Cast to reference.                                                                                |
|tkFunction            |None                                                                                              |
|tkMemberFunction      |None                                                                                              |
|tkMemberPointer       |None                                                                                              |
|tkConstructor         |None                                                                                              |
|tkOverloadedFunction  |None                                                                                              |
|tkDefaultArgsFunction |None                                                                                              |
|tkVariadicFunction    |None                                                                                              |
|tkArray               |char[] can cast to std::string.<br />wchar_t[] can cast to std::wstring.<br />T[] can cast to T *.|
|tkEnum                |None                                                                                              |
|tkAccessor            |None                                                                                              |
|tkVariant             |Use the cast rules of Variant                                                                     |
|tkMetaType            |None                                                                                              |
|tkMetaRepo            |None                                                                                              |
|tkStdString           |None                                                                                              |
|tkStdWideString       |None                                                                                              |
|tkStdSharedPtr        |std::weak_ptr<T>                                                                                  |
|tkStdUniquePtr        |None                                                                                              |
|tkStdWeakPtr          |std::shared_ptr<T>                                                                                |
|tkStdFunction         |Any MetaCallable can cast to std::function, as long as the argument count matches.                |
|tkStdVector           |None                                                                                              |
|tkStdList             |None                                                                                              |
|tkStdDeque            |None                                                                                              |
|tkStdArray            |None                                                                                              |
|tkStdForwardList      |None                                                                                              |
|tkStdStack            |None                                                                                              |
|tkStdQueue            |None                                                                                              |
|tkStdPriorityQueue    |None                                                                                              |
|tkStdMap              |None                                                                                              |
|tkStdMultimap         |None                                                                                              |
|tkStdSet              |None                                                                                              |
|tkStdMultiset         |None                                                                                              |
|tkStdUnorderedMap     |None                                                                                              |
|tkStdUnorderedMultimap|None                                                                                              |
|tkStdUnorderedSet     |None                                                                                              |
|tkStdUnorderedMultiset|None                                                                                              |
|tkStdPair             |None                                                                                              |
|tkStdTuple            |None                                                                                              |
|tkStdAny              |None                                                                                              |
|tkStdVariant          |None                                                                                              |
|tkUser                |None                                                                                              |

<a id="mdtoc_99a0987"></a>
## List of UpTypes

|TypeKind              |UpCount               |UpTypes                                                                                                                                 |
|----------------------|----------------------|----------------------------------------------------------------------------------------------------------------------------------------|
|tkVoid                |0                     |None                                                                                                                                    |
|tkBool                |0                     |None                                                                                                                                    |
|tkChar                |0                     |None                                                                                                                                    |
|tkWideChar            |0                     |None                                                                                                                                    |
|tkChar8 (C++20)       |0                     |None                                                                                                                                    |
|tkChar16              |0                     |None                                                                                                                                    |
|tkChar32              |0                     |None                                                                                                                                    |
|tkSignedChar          |0                     |None                                                                                                                                    |
|tkUnsignedChar        |0                     |None                                                                                                                                    |
|tkShort               |0                     |None                                                                                                                                    |
|tkUnsignedShort       |0                     |None                                                                                                                                    |
|tkInt                 |0                     |None                                                                                                                                    |
|tkUnsignedInt         |0                     |None                                                                                                                                    |
|tkLong                |0                     |None                                                                                                                                    |
|tkUnsignedLong        |0                     |None                                                                                                                                    |
|tkLongLong            |0                     |None                                                                                                                                    |
|tkUnsignedLongLong    |0                     |None                                                                                                                                    |
|tkFloat               |0                     |None                                                                                                                                    |
|tkDouble              |0                     |None                                                                                                                                    |
|tkLongDouble          |0                     |None                                                                                                                                    |
|tkObject              |0                     |None                                                                                                                                    |
|tkPointer             |1                     |T                                                                                                                                       |
|tkReference           |1                     |T                                                                                                                                       |
|tkFunction            |1+ParameterCount      |Up0: the return type<br />Up1: the first parameter<br />Up2: the second parameter<br />UpN: the Nth parameter                           |
|tkMemberFunction      |2+ParameterCount      |Up0: the class type<br />Up1: the return type<br />Up2: the first parameter<br />Up3: the second parameter<br />UpN: the N-1th parameter|
|tkMemberPointer       |2                     |Up0: the class type<br />Up1: the value type                                                                                            |
|tkConstructor         |1+ParameterCount      |Up0: the class type<br />Up1: the first parameter<br />Up2: the second parameter<br />UpN: the Nth parameter                            |
|tkOverloadedFunction  |0                     |None                                                                                                                                    |
|tkDefaultArgsFunction |as the underlying type|None                                                                                                                                    |
|tkVariadicFunction    |0                     |None                                                                                                                                    |
|tkArray               |1                     |the type removed by one extent                                                                                                          |
|tkEnum                |1                     |the underlying type                                                                                                                     |
|tkAccessor            |1                     |the value type                                                                                                                          |
|tkVariant             |0                     |None                                                                                                                                    |
|tkMetaType            |0                     |None                                                                                                                                    |
|tkMetaRepo            |0                     |None                                                                                                                                    |
|tkStdString           |0                     |None                                                                                                                                    |
|tkStdWideString       |0                     |None                                                                                                                                    |
|tkStdSharedPtr        |1                     |T                                                                                                                                       |
|tkStdUniquePtr        |1                     |T                                                                                                                                       |
|tkStdWeakPtr          |1                     |T                                                                                                                                       |
|tkStdFunction         |1+ParameterCount      |Up0: the return type<br />Up1: the first parameter<br />Up2: the second parameter<br />UpN: the Nth parameter                           |
|tkStdVector           |1                     |T                                                                                                                                       |
|tkStdList             |1                     |T                                                                                                                                       |
|tkStdDeque            |1                     |T                                                                                                                                       |
|tkStdArray            |1                     |T                                                                                                                                       |
|tkStdForwardList      |1                     |T                                                                                                                                       |
|tkStdStack            |0                     |None                                                                                                                                    |
|tkStdQueue            |0                     |None                                                                                                                                    |
|tkStdPriorityQueue    |0                     |None                                                                                                                                    |
|tkStdMap              |2                     |Up0: Key<br />Up1: T                                                                                                                    |
|tkStdMultimap         |2                     |Up0: Key<br />Up1: T                                                                                                                    |
|tkStdSet              |1                     |Key                                                                                                                                     |
|tkStdMultiset         |1                     |Key                                                                                                                                     |
|tkStdUnorderedMap     |2                     |Up0: Key<br />Up1: T                                                                                                                    |
|tkStdUnorderedMultimap|2                     |Up0: Key<br />Up1: T                                                                                                                    |
|tkStdUnorderedSet     |1                     |Key                                                                                                                                     |
|tkStdUnorderedMultiset|1                     |Key                                                                                                                                     |
|tkStdPair             |1                     |Up0: T1<br />Up1: T2                                                                                                                    |
|tkStdTuple            |sizeof...(Types)      |Up0: first type in Types<br />Up1: second type in Types<br />UpN: Nth type in Types                                                     |
|tkStdAny              |0                     |None                                                                                                                                    |
|tkStdVariant          |sizeof...(Types)      |Up0: first type in Types<br />Up1: second type in Types<br />UpN: Nth type in Types                                                     |
|tkUser                |0                     |None                                                                                                                                    |

