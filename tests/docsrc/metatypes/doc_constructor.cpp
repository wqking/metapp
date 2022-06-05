// metapp library
// 
// Copyright (C) 2022 Wang Qi (wqking)
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "testutil.h"

#include "metapp/allmetatypes.h"

/*desc
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
desc*/

//code
class DcClass
{
public:
	DcClass(const std::string &, const int) {}

	std::string greeting(const std::string & message) {
		return "Hello, " + message;
	}
};
//code

TEST_CASE("docConstructor")
{
	//code
	//desc Then we can have,
	metapp::Variant ctor = metapp::Constructor<DcClass (const std::string &, const int)>();

	//descThen we can call the constructor, a constructor is a callable which always allocates an object
	//desc and returns the instance pointer.  

	metapp::Variant instance(metapp::callableInvoke(ctor, nullptr, "hello", 5));

	//desc Now `instance` contains a pointer to DcClass, we can use it to invoke any functions in DcClass, for example,
	ASSERT(instance.get<DcClass *>()->greeting("metapp") == "Hello, metapp");
	//desc Since `instance` only contains a pointer, so the `Variant` doesn't manage the instance lifetime,
	//desc and it doesn't free the instance. We must delete the instance explicitly.  
	delete instance.get<DcClass *>();

	//desc Explicitly deleting is not elegant nor exception safe. We can use `Variant::takeFrom` to convert
	//desc the instance pointer to a `Variant` managed object.  

	metapp::Variant object = metapp::Variant::takeFrom(metapp::callableInvoke(ctor, nullptr, "hello", 5));

	//desc Now `object` contains a DcClass object (not pointer), and when `object` is freed,
	//desc the DcClass object is freed too, so we don't need to delete the object.  
	//desc To call `greeting()`, we can write,  
	ASSERT(object.get<DcClass &>().greeting("world") == "Hello, world");
	//code
}

/*desc
One may wonder why invoking a constructor doesn't return a Variant that manages the memory of the created instance. The reason is,
if the constructor returns Variant that owns the instance, then there is no way to take away the ownership from the Variant,
then the created object can't be managed by other mechanism such as a std::unique_ptr.
desc*/
