#ifndef METAPP_METAENUM_H_969872685611
#define METAPP_METAENUM_H_969872685611

#include "metapp/implement/internal/util_i.h"

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

	template <typename T>
	void addValue(const std::string & name, const T value) {
		nameValueMap[name] = static_cast<ValueType>(value);
	}

	std::vector<std::string> getNameList() const {
		return internal_::getMapKeys(nameValueMap);
	}

	ValueType getValue(const std::string & name) const {
		return internal_::getValueFromMap(nameValueMap, name);
	}

private:
	std::map<std::string, ValueType> nameValueMap;
};


} // namespace metapp

#endif
