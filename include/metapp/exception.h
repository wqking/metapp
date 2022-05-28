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

#ifndef METAPP_EXCEPTION_H_969872685611
#define METAPP_EXCEPTION_H_969872685611

#include "metapp/compiler.h"

#include <stdexcept>
#include <string>

#ifndef METAPP_DISABLE_EXCEPTION

#ifdef METAPP_COMPILER_VC
	#if defined(_CPPUNWIND ) && _CPPUNWIND 
		#define METAPP_EXCEPTION_ENABLED
	#endif
#else
	#if (defined(__EXCEPTIONS) && __EXCEPTIONS) || (defined(__cpp_exceptions) && __cpp_exceptions >= 199711)
		#define METAPP_EXCEPTION_ENABLED
	#endif
#endif

#endif

namespace metapp {

class MetaException : public std::runtime_error
{
private:
	using super = std::runtime_error;

public:
	using super::super;
};

class UnsupportedException : public MetaException
{
private:
	using super = MetaException;

public:
	UnsupportedException(const std::string & message = "Not supported")
		: super(message)
	{
	}
};

class BadCastException : public MetaException
{
private:
	using super = MetaException;

public:
	BadCastException(const std::string & message = "Bad cast")
		: super(message)
	{
	}
};

class IllegalArgumentException : public MetaException
{
private:
	using super = MetaException;

public:
	IllegalArgumentException(const std::string & message = "Illegal arguments")
		: super(message)
	{
	}
};

class WrongMetaTypeException : public MetaException
{
private:
	using super = MetaException;

public:
	WrongMetaTypeException(const std::string & message = "Wrong meta type")
		: super(message)
	{
	}
};

class OutOfRangeException : public MetaException
{
private:
	using super = MetaException;

public:
	OutOfRangeException(const std::string & message = "Index out of range")
		: super(message)
	{
	}
};

class UnwritableException : public MetaException
{
private:
	using super = MetaException;

public:
	UnwritableException(const std::string & message = "Unwritable")
		: super(message)
	{
	}
};

class NotConstructibleException : public MetaException
{
private:
	using super = MetaException;

public:
	NotConstructibleException(const std::string & message = "Not constructible")
		: super(message)
	{
	}
};

// When calling raiseException, the caller should put a "return" after the call,
// because if exception is disabled, no exception will be throw and the execute flow
// may coninue if there is no "return".

template <typename E, typename ...Args>
void raiseException(Args && ... args)
{
#ifdef METAPP_EXCEPTION_ENABLED
	throw E(std::forward<Args>(args)...);
#endif
}


} // namespace metapp

#ifdef METAPP_EXCEPTION_ENABLED
#undef METAPP_EXCEPTION_ENABLED
#endif


#endif
