#ifndef METAPP_UTIL_I_H_969872685611
#define METAPP_UTIL_I_H_969872685611

#include <vector>

namespace metapp {

namespace internal_ {

template <typename Map>
auto getMapKeys(const Map & map)
	-> std::vector<typename Map::key_type>
{
	std::vector<typename Map::key_type> result;
	for(auto it = std::begin(map); it != std::end(map); ++it) {
		result.push_back(it->first);
	}
	return result;
}

template <typename Map>
auto getPointerFromMap(const Map & map, const std::string & name)
	-> const typename Map::mapped_type *
{
	auto it = map.find(name);
	if(it == map.end()) {
		return nullptr;
	}
	return &it->second;
}

template <typename Map>
auto getValueFromMap(
		const Map & map,
		const std::string & name,
		const typename Map::mapped_type & defaultValue = typename Map::mapped_type()
	)
	-> typename Map::mapped_type
{
	auto it = map.find(name);
	if(it == map.end()) {
		return defaultValue;
	}
	return it->second;
}


} // namespace internal_

} // namespace metapp

#endif
