#ifndef METAPP_EXCEPTION_H_969872685611
#define METAPP_EXCEPTION_H_969872685611

#include "metapp/compiler.h"

#ifdef METAPP_COMPILER_VC
	#if defined(_CPPUNWIND ) && _CPPUNWIND 
		#define METAPP_EXCEPTION_ENABLED
	#endif
#else
	#if (defined(__EXCEPTIONS) && __EXCEPTIONS) || (defined(__cpp_exceptions) && __cpp_exceptions >= 199711)
		#define METAPP_EXCEPTION_ENABLED
	#endif
#endif

#include <stdexcept>

namespace metapp {

class MetaException : public std::runtime_error
{
private:
	using super = std::runtime_error;

public:
	using super::super;
};

class NotSupportedException : public MetaException
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

#ifdef METAPP_EXCEPTION_ENABLED

inline void errorNotSupported(const std::string & message = "Not supported")
{
	throw NotSupportedException(message);
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

#else

inline void errorNotSupported(const std::string & /*message*/ = "Not supported")
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

#endif


} // namespace metapp

#ifdef METAPP_EXCEPTION_ENABLED
#undef METAPP_EXCEPTION_ENABLED
#endif


#endif
