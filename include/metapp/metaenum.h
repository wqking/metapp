#ifndef METAENUM_H_969872685611
#define METAENUM_H_969872685611

#include <vector>
#include <map>

namespace metapp {

class MetaEnum
{
public:
	template <typename FT>
	MetaEnum(FT callback)
	{
		callback(*this);
	}

private:
};


} // namespace metapp

#endif
