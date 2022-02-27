#ifndef EXCEPTION_H_969872685611
#define EXCEPTION_H_969872685611

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


} // namespace metapp

#endif
