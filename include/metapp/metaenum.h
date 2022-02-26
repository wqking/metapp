#ifndef METAENUM_H_969872685611
#define METAENUM_H_969872685611

#include "metapp/internal/util_i.h"

#include <vector>
#include <map>

namespace metapp {

class MetaEnum
{
public:
	using ValueType = long long;

public:
	template <typename FT>
	MetaEnum(FT callback)
	{
		callback(*this);
	}

	void addValue(const std::string & name, const ValueType value) {
		nameValueMap[name] = value;
	}

	std::vector<std::string> getNameList() const {
		return internal_::getMapKeys(nameValueMap);
	}

private:
	std::map<std::string, ValueType> nameValueMap;
};


} // namespace metapp

#endif
