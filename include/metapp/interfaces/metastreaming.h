#ifndef METAPP_METASTREAMING_H_969872685611
#define METAPP_METASTREAMING_H_969872685611

#include <iostream>

namespace metapp {

class Variant;

class MetaStreaming
{
public:
	MetaStreaming(
		void (*streamIn)(std::istream & stream, Variant & value),
		void (*streamOut)(std::ostream & stream, const Variant & value)
	)
		: streamIn(streamIn), streamOut(streamOut)
	{
	}

	void (*streamIn)(std::istream & stream, Variant & value);
	void (*streamOut)(std::ostream & stream, const Variant & value);
};


} // namespace metapp

#endif
