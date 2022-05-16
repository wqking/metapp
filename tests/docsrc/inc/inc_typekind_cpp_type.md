|TypeKind              |Value     |C++ type                                                                                                      |
|----------------------|----------|--------------------------------------------------------------------------------------------------------------|
|tkVoid                |0         |void                                                                                                          |
|tkBool                |1         |bool                                                                                                          |
|tkChar                |2         |char                                                                                                          |
|tkWideChar            |3         |wchar_t                                                                                                       |
|tkChar8 (C++20)       |4         |None                                                                                                          |
|tkChar16              |5         |None                                                                                                          |
|tkChar32              |6         |None                                                                                                          |
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