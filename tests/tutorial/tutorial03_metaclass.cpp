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

#include "tutorial.h"

#include "metapp/metatypes/metatypes.h"
#include "metapp/interfaces/metaclass.h"
#include "metapp/metarepo.h"

#include <iostream>

class MyBaseClass
{
public:
	MyBaseClass() : value(0) {
	}

	explicit MyBaseClass(const int n) : value(n) {
	}

	int getValue() const {
		return value;
	}
	
	void setValue(const int n) {
		value = n;
	}

	virtual std::string greeting() const {
		return "Hello, base";
	}

private:
	int value;
};

class MyAnotherBase
{
public:
	std::string getText() const {
		return "another";
	}

private:
	int anotherValue;
};

class MyDerivedClass : public MyBaseClass, public MyAnotherBase
{
public:
	std::string greeting() const override {
		return "Hi, derived";
	}

};

template <>
struct metapp::DeclareMetaType <MyBaseClass> : metapp::DeclareMetaTypeBase <MyBaseClass>
{
	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<MyBaseClass>(),
			[](metapp::MetaClass & mc) {
				mc.addConstructor(metapp::Constructor<MyBaseClass ()>());
				mc.addConstructor(metapp::Constructor<MyBaseClass (const int)>());
				mc.addField("value", metapp::Accessor<int>(&MyBaseClass::getValue, &MyBaseClass::setValue));
				mc.addMethod("greeting", &MyBaseClass::greeting);
			}
		);
		return &metaClass;
	}

};

template <>
struct metapp::DeclareMetaType <MyAnotherBase> : metapp::DeclareMetaTypeBase <MyAnotherBase>
{
	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<MyAnotherBase>(),
			[](metapp::MetaClass & mc) {
				mc.addMethod("getText", &MyAnotherBase::getText);
			}
		);
		return &metaClass;
	}

};

template <>
struct metapp::DeclareMetaType <MyDerivedClass> : metapp::DeclareMetaTypeBase <MyDerivedClass>
{
	static void setup() {
		metapp::getMetaRepo()->registerBase<MyDerivedClass, MyBaseClass, MyAnotherBase>();
	}

	static const metapp::MetaClass * getMetaClass() {
		static const metapp::MetaClass metaClass(
			metapp::getMetaType<MyDerivedClass>(),
			[](metapp::MetaClass & /*mc*/) {
			}
		);
		return &metaClass;
	}

};

void tutorialMetaClass()
{
	const metapp::MetaType * metaType = metapp::getMetaType<MyDerivedClass>();
	const metapp::MetaClass * metaClass = metaType->getMetaClass();
	MyDerivedClass obj;
	MyBaseClass * pBase = &obj;
	ASSERT(metapp::accessibleGet(metaClass->getField("value"), &obj).get<int>() == 0);
	ASSERT(metapp::accessibleGet(metaClass->getField("value"), pBase).get<int>() == 0);
}


RUN_TUTORIAL(tutorialMetaClass)
