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
	using super::super;
};

class BadCastException : public MetaException
{
private:
	using super = MetaException;

public:
	using super::super;
};

class IllegalArgumentException : public MetaException
{
private:
	using super = MetaException;

public:
	using super::super;
};

class WrongMetaTypeException : public MetaException
{
private:
	using super = MetaException;

public:
	using super::super;
};

class InvalidIndexException : public MetaException
{
private:
	using super = MetaException;

public:
	using super::super;
};

class UnwritableException : public MetaException
{
private:
	using super = MetaException;

public:
	using super::super;
};

class NotConstructibleException : public MetaException
{
private:
	using super = MetaException;

public:
	using super::super;
};

// When calling errorXXX function, the caller should put a "return" after the call,
// because if exception is disabled, no exception will be throw and the execute flow
// may coninue if there is no "return".

#ifdef METAPP_EXCEPTION_ENABLED

inline void errorUnsupported(const std::string & message = "Not supported")
{
	throw UnsupportedException(message);
}

inline void errorBadCast(const std::string & message = "Bad cast")
{
	throw BadCastException(message);
}

inline void errorIllegalArgument(const std::string & message = "Illegal arguments")
{
	throw IllegalArgumentException(message);
}

inline void errorWrongMetaType(const std::string & message = "Wrong meta type")
{
	throw WrongMetaTypeException(message);
}

inline void errorInvalidIndex(const std::string & message = "Invalid index")
{
	throw InvalidIndexException(message);
}

inline void errorUnwritable(const std::string & message = "Unwritable")
{
	throw UnwritableException(message);
}

inline void errorNotConstructible(const std::string & message = "Not constructible")
{
	throw NotConstructibleException(message);
}

#else

inline void errorUnsupported(const std::string & /*message*/ = "Not supported")
{
}

inline void errorBadCast(const std::string & /*message*/ = "Bad cast")
{
}

inline void errorIllegalArgument(const std::string & /*message*/ = "Illegal arguments")
{
}

inline void errorWrongMetaType(const std::string & /*message*/ = "Wrong meta type")
{
}

inline void errorInvalidIndex(const std::string & /*message*/ = "Invalid index")
{
}

inline void errorUnwritable(const std::string & /*message*/ = "Unwritable")
{
}

inline void errorNotConstructible(const std::string & /*message*/ = "Not constructible")
{
}

#endif


} // namespace metapp

#ifdef METAPP_EXCEPTION_ENABLED
#undef METAPP_EXCEPTION_ENABLED
#endif


#endif
