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

inline void errorNoStreamIn()
{
	throw NotSupportedException("No << input streaming operator.");
}

inline void errorNoStreamOut()
{
	throw NotSupportedException("No >> output streaming operator.");
}

inline void errorBadCast()
{
	throw NotSupportedException("Bad cast");
}


} // namespace metapp

#endif
