# List of all built-in meta types

## How to use the build-in meta types

To use the meta types, below header must be included.  

```c++
#include "metapp/metatypes/metatypes.h"
```

## List of type kinds and equivalent C++ types

| TypeKind               | Value | C++ type                                                                                                 |
|------------------------|-------|----------------------------------------------------------------------------------------------------------|
| tkVoid                 | 0     | void                                                                                                     |
| tkBool                 | 1     | bool                                                                                                     |
| tkChar                 | 2     | char                                                                                                     |
| tkWideChar             | 3     | wchar_t                                                                                                  |
| tkSignedChar           | 4     | signed char                                                                                              |
| tkUnsignedChar         | 5     | unsigned char                                                                                            |
| tkShort                | 6     | short                                                                                                    |
| tkUnsignedShort        | 7     | unsigned short                                                                                           |
| tkInt                  | 8     | int                                                                                                      |
| tkUnsignedInt          | 9     | unsigned int                                                                                             |
| tkLong                 | 10    | long                                                                                                     |
| tkUnsignedLong         | 11    | unsigned long                                                                                            |
| tkLongLong             | 12    | long long                                                                                                |
| tkUnsignedLongLong     | 13    | unsigned long long                                                                                       |
| tkFloat                | 14    | float                                                                                                    |
| tkDouble               | 15    | double                                                                                                   |
| tkLongDouble           | 16    | long double                                                                                              |
| tkObject               | 30    |                                                                                                          |
| tkPointer              | 31    | T *                                                                                                      |
| tkReference            | 32    | T &                                                                                                      |
| tkFunction             | 33    |                                                                                                          |
| tkMemberFunction       | 34    |                                                                                                          |
| tkMemberPointer        | 35    |                                                                                                          |
| tkConstructor          | 36    |                                                                                                          |
| tkDefaultArgsFunction  | 37    |                                                                                                          |
| tkVariadicFunction     | 38    |                                                                                                          |
| tkArray                | 39    |                                                                                                          |
| tkEnum                 | 40    |                                                                                                          |
| tkAccessor             | 41    |                                                                                                          |
| tkVariant              | 42    | metapp::Variant                                                                                          |
| tkStdString            | 70    | std::string                                                                                              |
| tkStdWideString        | 71    | std::wstring                                                                                             |
| tkStdSharedPtr         | 72    | std::shared_ptr<T>                                                                                       |
| tkStdUniquePtr         | 73    | std::unique_ptr<T>                                                                                       |
| tkStdFunction          | 74    | std::function                                                                                            |
| tkStdVector            | 75    | std::vector<T, Allocator>                                                                                |
| tkStdList              | 76    | std::list<T, Allocator>                                                                                  |
| tkStdDeque             | 77    | std::deque<T, Allocator>                                                                                 |
| tkStdArray             | 78    | std::array<T, length>                                                                                    |
| tkStdForwardList       | 79    | std::forward_list<T, Allocator>                                                                          |
| tkStdStack             | 80    | std::stack<T, Container>                                                                                 |
| tkStdQueue             | 81    | std::queue<T, Container>                                                                                 |
| tkStdPriorityQueue     | 82    | std::priority_queue<T, Container>                                                                        |
| tkStdMap               | 83    | std::map<Key, T, Compare, Allocator>                                                                     |
| tkStdMultimap          | 84    | std::multimap<<br/>    Key,<br/>    T,<br/>    Compare,<br/>    Allocator<br/>>                          |
| tkStdSet               | 85    | std::set<Key, Compare, Allocator>                                                                        |
| tkStdMultiset          | 86    | std::multiset<<br/>    Key,<br/>    Compare,<br/>    Allocator<br/>>                                     |
| tkStdUnorderedMap      | 87    | std::unordered_map<<br/>    Key,<br/>    T,<br/>    Hash,<br/>    KeyEqual,<br/>    Allocator<br/>>      |
| tkStdUnorderedMultimap | 88    | std::unordered_multimap<<br/>    Key,<br/>    T,<br/>    Hash,<br/>    KeyEqual,<br/>    Allocator<br/>> |
| tkStdUnorderedSet      | 89    | std::unordered_set<<br/>    Key,<br/>    Hash,<br/>    KeyEqual,<br/>    Allocator<br/>>                 |
| tkStdUnorderedMultiset | 90    | std::unordered_multiset<<br/>    Key,<br/>    Hash,<br/>    KeyEqual,<br/>    Allocator<br/>>            |
| tkStdPair              | 91    | std::pair<T1, T2>                                                                                        |
| tkStdTuple             | 92    | std::tuple<Types...>                                                                                     |
| tkStdAny               | 93    | std::any                                                                                                 |
| tkStdVariant           | 94    | std::variant<Types...>                                                                                   |
| tkUser                 | 1024  |

## List of implemented meta interfaces

| TypeKind               | Implemented meta interfaces    |
|------------------------|--------------------------------|
| tkVoid                 |                                |
| tkBool                 |                                |
| tkChar                 |                                |
| tkWideChar             |                                |
| tkSignedChar           |                                |
| tkUnsignedChar         |                                |
| tkShort                |                                |
| tkUnsignedShort        |                                |
| tkInt                  |                                |
| tkUnsignedInt          |                                |
| tkLong                 |                                |
| tkUnsignedLong         |                                |
| tkLongLong             |                                |
| tkUnsignedLongLong     |                                |
| tkFloat                |                                |
| tkDouble               |                                |
| tkLongDouble           |                                |
| tkObject               |                                |
| tkPointer              | MetaAccessible                 |
| tkReference            |                                |
| tkFunction             | MetaCallable                   |
| tkMemberFunction       | MetaCallable<br/>MetaMember    |
| tkMemberPointer        | MetaMember                     |
| tkConstructor          | MetaCallable<br/>MetaMember    |
| tkDefaultArgsFunction  | MetaCallable                   |
| tkVariadicFunction     | MetaCallable                   |
| tkArray                | MetaIndexable                  |
| tkEnum                 |                                |
| tkAccessor             | MetaAccessible                 |
| tkVariant              |                                |
| tkStdString            |                                |
| tkStdWideString        |                                |
| tkStdSharedPtr         |                                |
| tkStdUniquePtr         |                                |
| tkStdFunction          | MetaCallable                   |
| tkStdVector            | MetaIndexable<br/>MetaIterable |
| tkStdList              | MetaIndexable<br/>MetaIterable |
| tkStdDeque             | MetaIndexable<br/>MetaIterable |
| tkStdArray             | MetaIndexable<br/>MetaIterable |
| tkStdForwardList       | MetaIterable                   |
| tkStdStack             |                                |
| tkStdQueue             |                                |
| tkStdPriorityQueue     |                                |
| tkStdMap               | MetaIterable<br/>MetaMap       |
| tkStdMultimap          | MetaIterable<br/>MetaMap       |
| tkStdSet               | MetaIterable                   |
| tkStdMultiset          | MetaIterable                   |
| tkStdUnorderedMap      | MetaIterable<br/>MetaMap       |
| tkStdUnorderedMultimap | MetaIterable<br/>MetaMap       |
| tkStdUnorderedSet      | MetaIterable                   |
| tkStdUnorderedMultiset | MetaIterable                   |
| tkStdPair              | MetaIndexable                  |
| tkStdTuple             | MetaIndexable<br/>MetaIterable |
| tkStdAny               |                                |
| tkStdVariant           |                                |
| tkUser                 |

## List of UpTypes

| TypeKind               | UpCount                | UpTypes                                                                                                                              |
|------------------------|------------------------|--------------------------------------------------------------------------------------------------------------------------------------|
| tkVoid                 | 0                      |                                                                                                                                      |
| tkBool                 | 0                      |                                                                                                                                      |
| tkChar                 | 0                      |                                                                                                                                      |
| tkWideChar             | 0                      |                                                                                                                                      |
| tkSignedChar           | 0                      |                                                                                                                                      |
| tkUnsignedChar         | 0                      |                                                                                                                                      |
| tkShort                | 0                      |                                                                                                                                      |
| tkUnsignedShort        | 0                      |                                                                                                                                      |
| tkInt                  | 0                      |                                                                                                                                      |
| tkUnsignedInt          | 0                      |                                                                                                                                      |
| tkLong                 | 0                      |                                                                                                                                      |
| tkUnsignedLong         | 0                      |                                                                                                                                      |
| tkLongLong             | 0                      |                                                                                                                                      |
| tkUnsignedLongLong     | 0                      |                                                                                                                                      |
| tkFloat                | 0                      |                                                                                                                                      |
| tkDouble               | 0                      |                                                                                                                                      |
| tkLongDouble           | 0                      |                                                                                                                                      |
| tkObject               |                        |                                                                                                                                      |
| tkPointer              | 1                      | T                                                                                                                                    |
| tkReference            | 1                      | T                                                                                                                                    |
| tkFunction             | 1+ParameterCount       | Up0: the return type<br/>Up1: the first parameter<br/>Up2: the second parameter<br/>UpN: the Nth parameter                           |
| tkMemberFunction       | 2+ParameterCount       | Up0: the class type<br/>Up1: the return type<br/>Up2: the first parameter<br/>Up3: the second parameter<br/>UpN: the N-1th parameter |
| tkMemberPointer        | 2                      | Up0: the class type<br/>Up1: the value type                                                                                          |
| tkConstructor          | 1+ParameterCount       | Up0: the class type<br/>Up1: the first parameter<br/>Up2: the second parameter<br/>UpN: the Nth parameter                            |
| tkDefaultArgsFunction  | as the underlying type |                                                                                                                                      |
| tkVariadicFunction     | 0                      |                                                                                                                                      |
| tkArray                | 1                      | the type removed by one extent                                                                                                       |
| tkEnum                 | 1                      | the underlying type                                                                                                                  |
| tkAccessor             | 1                      | the value type                                                                                                                       |
| tkVariant              | 0                      |                                                                                                                                      |
| tkStdString            | 0                      |                                                                                                                                      |
| tkStdWideString        | 0                      |                                                                                                                                      |
| tkStdSharedPtr         | 1                      | T                                                                                                                                    |
| tkStdUniquePtr         | 1                      | T                                                                                                                                    |
| tkStdFunction          | 1+ParameterCount       | Up0: the return type<br/>Up1: the first parameter<br/>Up2: the second parameter<br/>UpN: the Nth parameter                           |
| tkStdVector            | 1                      | T                                                                                                                                    |
| tkStdList              | 1                      | T                                                                                                                                    |
| tkStdDeque             | 1                      | T                                                                                                                                    |
| tkStdArray             | 1                      | T                                                                                                                                    |
| tkStdForwardList       | 1                      | T                                                                                                                                    |
| tkStdStack             | 0                      |                                                                                                                                      |
| tkStdQueue             | 0                      |                                                                                                                                      |
| tkStdPriorityQueue     | 0                      |                                                                                                                                      |
| tkStdMap               | 2                      | Up0: Key<br/>Up1: T                                                                                                                  |
| tkStdMultimap          | 2                      | Up0: Key<br/>Up1: T                                                                                                                  |
| tkStdSet               | 1                      | Key                                                                                                                                  |
| tkStdMultiset          | 1                      | Key                                                                                                                                  |
| tkStdUnorderedMap      | 2                      | Up0: Key<br/>Up1: T                                                                                                                  |
| tkStdUnorderedMultimap | 2                      | Up0: Key<br/>Up1: T                                                                                                                  |
| tkStdUnorderedSet      | 1                      | Key                                                                                                                                  |
| tkStdUnorderedMultiset | 1                      | Key                                                                                                                                  |
| tkStdPair              | 1                      | Up0: T1<br/>Up1: T2                                                                                                                  |
| tkStdTuple             | sizeof...(Types)       | Up0: first type in Types<br/>Up1: second type in Types<br/>UpN: Nth type in Types                                                    |
| tkStdAny               | 0                      |                                                                                                                                      |
| tkStdVariant           | sizeof...(Types)       | Up0: first type in Types<br/>Up1: second type in Types<br/>UpN: Nth type in Types                                                    |
| tkUser                 |
