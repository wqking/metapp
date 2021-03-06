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