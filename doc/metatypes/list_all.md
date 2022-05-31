[//]: # (Auto generated file, don't modify this file.)

# List of all built-in meta types
<!--begintoc-->
* [List of type kinds and equivalent C++ types](#a2_1)
* [List of implemented meta interfaces](#a2_2)
* [List of cast-abilities](#a2_3)
* [List of UpTypes](#a2_4)
<!--endtoc-->

<a id="a2_1"></a>
## List of type kinds and equivalent C++ types

|TypeKind              |Value     |C++ type                                                                                                      |
|----------------------|----------|--------------------------------------------------------------------------------------------------------------|
|tkVoid                |0         |void                                                                                                          |
|tkBool                |1         |bool                                                                                                          |
|tkChar                |2         |char                                                                                                          |
|tkWideChar            |3         |wchar_t                                                                                                       |
|tkChar8 (C++20)       |4         |char8_t                                                                                                       |
|tkChar16              |5         |char16_t                                                                                                      |
|tkChar32              |6         |char32_t                                                                                                      |
|tkSignedChar          |7         |signed char                                                                                                   |
|tkUnsignedChar        |8         |unsigned char                                                                                                 |
|tkShort               |9         |short                                                                                                         |
|tkUnsignedShort       |10        |unsigned short                                                                                                |
|tkInt                 |11        |int                                                                                                           |
|tkUnsignedInt         |12        |unsigned int                                                                                                  |
|tkLong                |13        |long                                                                                                          |
|tkUnsignedLong        |14        |unsigned long                                                                                                 |
|tkLongLong            |15        |long long                                                                                                     |
|tkUnsignedLongLong    |16        |unsigned long long                                                                                            |
|tkFloat               |17        |float                                                                                                         |
|tkDouble              |18        |double                                                                                                        |
|tkLongDouble          |19        |long double                                                                                                   |
|tkObject              |40        |None                                                                                                          |
|tkPointer             |41        |T *                                                                                                           |
|tkReference           |42        |T &                                                                                                           |
|tkFunction            |43        |None                                                                                                          |
|tkMemberFunction      |44        |None                                                                                                          |
|tkMemberPointer       |45        |None                                                                                                          |
|tkConstructor         |46        |None                                                                                                          |
|tkOverloadedFunction  |47        |None                                                                                                          |
|tkDefaultArgsFunction |48        |None                                                                                                          |
|tkVariadicFunction    |49        |None                                                                                                          |
|tkArray               |50        |None                                                                                                          |
|tkEnum                |51        |None                                                                                                          |
|tkAccessor            |52        |None                                                                                                          |
|tkVariant             |53        |metapp::Variant                                                                                               |
|tkMetaType            |54        |None                                                                                                          |
|tkMetaRepo            |55        |None                                                                                                          |
|tkStdString           |100       |std::string                                                                                                   |
|tkStdWideString       |101       |std::wstring                                                                                                  |
|tkStdSharedPtr        |102       |std::shared_ptr<T>                                                                                            |
|tkStdUniquePtr        |103       |std::unique_ptr<T>                                                                                            |
|tkStdWeakPtr          |104       |std::weak_ptr<T>                                                                                              |
|tkStdFunction         |105       |std::function                                                                                                 |
|tkStdVector           |106       |std::vector<T, Allocator>                                                                                     |
|tkStdList             |107       |std::list<T, Allocator>                                                                                       |
|tkStdDeque            |108       |std::deque<T, Allocator>                                                                                      |
|tkStdArray            |109       |std::array<T, length>                                                                                         |
|tkStdForwardList      |110       |std::forward_list<T, Allocator>                                                                               |
|tkStdStack            |111       |std::stack<T, Container>                                                                                      |
|tkStdQueue            |112       |std::queue<T, Container>                                                                                      |
|tkStdPriorityQueue    |113       |std::priority_queue<T, Container>                                                                             |
|tkStdMap              |114       |std::map<Key, T, Compare, Allocator>                                                                          |
|tkStdMultimap         |115       |std::multimap<<br />    Key,<br />    T,<br />    Compare,<br />    Allocator<br />>                          |
|tkStdSet              |116       |std::set<Key, Compare, Allocator>                                                                             |
|tkStdMultiset         |117       |std::multiset<<br />    Key,<br />    Compare,<br />    Allocator<br />>                                      |
|tkStdUnorderedMap     |118       |std::unordered_map<<br />    Key,<br />    T,<br />    Hash,<br />    KeyEqual,<br />    Allocator<br />>     |
|tkStdUnorderedMultimap|119       |std::unordered_multimap<<br />    Key,<br />    T,<br />    Hash,<br />    KeyEqual,<br />    Allocator<br />>|
|tkStdUnorderedSet     |120       |std::unordered_set<<br />    Key,<br />    Hash,<br />    KeyEqual,<br />    Allocator<br />>                 |
|tkStdUnorderedMultiset|121       |std::unordered_multiset<<br />    Key,<br />    Hash,<br />    KeyEqual,<br />    Allocator<br />>            |
|tkStdPair             |122       |std::pair<T1, T2>                                                                                             |
|tkStdTuple            |123       |std::tuple<Types...>                                                                                          |
|tkStdAny              |124       |std::any                                                                                                      |
|tkStdVariant          |125       |std::variant<Types...>                                                                                        |
|tkUser                |1024      |None                                                                                                          |

<a id="a2_2"></a>
## List of implemented meta interfaces

|TypeKind              |Implemented meta interfaces           |
|----------------------|--------------------------------------|
|tkVoid                |None                                  |
|tkBool                |None                                  |
|tkChar                |None                                  |
|tkWideChar            |None                                  |
|tkChar8 (C++20)       |None                                  |
|tkChar16              |None                                  |
|tkChar32              |None                                  |
|tkSignedChar          |None                                  |
|tkUnsignedChar        |None                                  |
|tkShort               |None                                  |
|tkUnsignedShort       |None                                  |
|tkInt                 |None                                  |
|tkUnsignedInt         |None                                  |
|tkLong                |None                                  |
|tkUnsignedLong        |None                                  |
|tkLongLong            |None                                  |
|tkUnsignedLongLong    |None                                  |
|tkFloat               |None                                  |
|tkDouble              |None                                  |
|tkLongDouble          |None                                  |
|tkObject              |None                                  |
|tkPointer             |MetaAccessible                        |
|tkReference           |None                                  |
|tkFunction            |MetaCallable                          |
|tkMemberFunction      |MetaCallable                          |
|tkMemberPointer       |MetaAccessible                        |
|tkConstructor         |MetaCallable<br />MetaMember          |
|tkOverloadedFunction  |MetaCallable                          |
|tkDefaultArgsFunction |MetaCallable                          |
|tkVariadicFunction    |MetaCallable                          |
|tkArray               |MetaIndexable                         |
|tkEnum                |None                                  |
|tkAccessor            |MetaAccessible                        |
|tkVariant             |None                                  |
|tkMetaType            |None                                  |
|tkMetaRepo            |None                                  |
|tkStdString           |None                                  |
|tkStdWideString       |None                                  |
|tkStdSharedPtr        |MetaAccessible<br />MetaPointerWrapper|
|tkStdUniquePtr        |MetaAccessible<br />MetaPointerWrapper|
|tkStdWeakPtr          |None                                  |
|tkStdFunction         |MetaCallable                          |
|tkStdVector           |MetaIndexable<br />MetaIterable       |
|tkStdList             |MetaIndexable<br />MetaIterable       |
|tkStdDeque            |MetaIndexable<br />MetaIterable       |
|tkStdArray            |MetaIndexable<br />MetaIterable       |
|tkStdForwardList      |MetaIterable                          |
|tkStdStack            |None                                  |
|tkStdQueue            |None                                  |
|tkStdPriorityQueue    |None                                  |
|tkStdMap              |MetaIterable<br />MetaMappable        |
|tkStdMultimap         |MetaIterable<br />MetaMappable        |
|tkStdSet              |MetaIterable                          |
|tkStdMultiset         |MetaIterable                          |
|tkStdUnorderedMap     |MetaIterable<br />MetaMappable        |
|tkStdUnorderedMultimap|MetaIterable<br />MetaMappable        |
|tkStdUnorderedSet     |MetaIterable                          |
|tkStdUnorderedMultiset|MetaIterable                          |
|tkStdPair             |MetaIndexable                         |
|tkStdTuple            |MetaIndexable<br />MetaIterable       |
|tkStdAny              |None                                  |
|tkStdVariant          |None                                  |
|tkUser                |None                                  |

<a id="a2_3"></a>
## List of cast-abilities

|TypeKind              |Cast to                                                                                    |
|----------------------|-------------------------------------------------------------------------------------------|
|tkVoid                |None                                                                                       |
|tkBool                |All arithmetic types.<br />Any types that can cast arithmetic types to.                    |
|tkChar                |Ditto                                                                                      |
|tkWideChar            |Ditto                                                                                      |
|tkChar8 (C++20)       |Ditto                                                                                      |
|tkChar16              |Ditto                                                                                      |
|tkChar32              |Ditto                                                                                      |
|tkSignedChar          |Ditto                                                                                      |
|tkUnsignedChar        |Ditto                                                                                      |
|tkShort               |Ditto                                                                                      |
|tkUnsignedShort       |Ditto                                                                                      |
|tkInt                 |Ditto                                                                                      |
|tkUnsignedInt         |Ditto                                                                                      |
|tkLong                |Ditto                                                                                      |
|tkUnsignedLong        |Ditto                                                                                      |
|tkLongLong            |Ditto                                                                                      |
|tkUnsignedLongLong    |Ditto                                                                                      |
|tkFloat               |Ditto                                                                                      |
|tkDouble              |Ditto                                                                                      |
|tkLongDouble          |Ditto                                                                                      |
|tkObject              |None                                                                                       |
|tkPointer             |Any pointers.<br />char * can cast to std::string.<br />wchar_t * can cast to std::wstring.|
|tkReference           |None                                                                                       |
|tkFunction            |None                                                                                       |
|tkMemberFunction      |None                                                                                       |
|tkMemberPointer       |None                                                                                       |
|tkConstructor         |None                                                                                       |
|tkOverloadedFunction  |None                                                                                       |
|tkDefaultArgsFunction |None                                                                                       |
|tkVariadicFunction    |None                                                                                       |
|tkArray               |None                                                                                       |
|tkEnum                |None                                                                                       |
|tkAccessor            |None                                                                                       |
|tkVariant             |Use the cast rules of Variant                                                              |
|tkMetaType            |None                                                                                       |
|tkMetaRepo            |None                                                                                       |
|tkStdString           |char *                                                                                     |
|tkStdWideString       |wchar_t *                                                                                  |
|tkStdSharedPtr        |T *<br />std::weak_ptr<T>                                                                  |
|tkStdUniquePtr        |T *                                                                                        |
|tkStdWeakPtr          |std::shared_ptr<T>                                                                         |
|tkStdFunction         |Any MetaCallable can cast to std::function, as long as the argument count matches.         |
|tkStdVector           |None                                                                                       |
|tkStdList             |None                                                                                       |
|tkStdDeque            |None                                                                                       |
|tkStdArray            |None                                                                                       |
|tkStdForwardList      |None                                                                                       |
|tkStdStack            |None                                                                                       |
|tkStdQueue            |None                                                                                       |
|tkStdPriorityQueue    |None                                                                                       |
|tkStdMap              |None                                                                                       |
|tkStdMultimap         |None                                                                                       |
|tkStdSet              |None                                                                                       |
|tkStdMultiset         |None                                                                                       |
|tkStdUnorderedMap     |None                                                                                       |
|tkStdUnorderedMultimap|None                                                                                       |
|tkStdUnorderedSet     |None                                                                                       |
|tkStdUnorderedMultiset|None                                                                                       |
|tkStdPair             |None                                                                                       |
|tkStdTuple            |None                                                                                       |
|tkStdAny              |None                                                                                       |
|tkStdVariant          |None                                                                                       |
|tkUser                |None                                                                                       |

<a id="a2_4"></a>
## List of UpTypes

|TypeKind              |UpCount               |UpTypes                                                                                                                                 |
|----------------------|----------------------|----------------------------------------------------------------------------------------------------------------------------------------|
|tkVoid                |0                     |None                                                                                                                                    |
|tkBool                |0                     |None                                                                                                                                    |
|tkChar                |0                     |None                                                                                                                                    |
|tkWideChar            |0                     |None                                                                                                                                    |
|tkChar8 (C++20)       |None                  |None                                                                                                                                    |
|tkChar16              |None                  |None                                                                                                                                    |
|tkChar32              |None                  |None                                                                                                                                    |
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
|tkObject              |None                  |None                                                                                                                                    |
|tkPointer             |1                     |T                                                                                                                                       |
|tkReference           |1                     |T                                                                                                                                       |
|tkFunction            |1+ParameterCount      |Up0: the return type<br />Up1: the first parameter<br />Up2: the second parameter<br />UpN: the Nth parameter                           |
|tkMemberFunction      |2+ParameterCount      |Up0: the class type<br />Up1: the return type<br />Up2: the first parameter<br />Up3: the second parameter<br />UpN: the N-1th parameter|
|tkMemberPointer       |2                     |Up0: the class type<br />Up1: the value type                                                                                            |
|tkConstructor         |1+ParameterCount      |Up0: the class type<br />Up1: the first parameter<br />Up2: the second parameter<br />UpN: the Nth parameter                            |
|tkOverloadedFunction  |None                  |None                                                                                                                                    |
|tkDefaultArgsFunction |as the underlying type|None                                                                                                                                    |
|tkVariadicFunction    |0                     |None                                                                                                                                    |
|tkArray               |1                     |the type removed by one extent                                                                                                          |
|tkEnum                |1                     |the underlying type                                                                                                                     |
|tkAccessor            |1                     |the value type                                                                                                                          |
|tkVariant             |0                     |None                                                                                                                                    |
|tkMetaType            |None                  |None                                                                                                                                    |
|tkMetaRepo            |None                  |None                                                                                                                                    |
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
|tkUser                |None                  |None                                                                                                                                    |
