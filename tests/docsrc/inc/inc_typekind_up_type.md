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