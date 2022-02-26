#ifndef UTIL_I_H_969872685611
#define UTIL_I_H_969872685611

#include <vector>

namespace metapp {

namespace internal_ {

template <typename Map>
auto getMapKeys(const Map & map) -> std::vector<typename Map::key_type>
{
	std::vector<typename Map::key_type> result;
	for(auto it = std::begin(map); it != std::end(map); ++it) {
		result.push_back(it->first);
	}
	return result;
}

} // namespace internal_

} // namespace metapp

#endif
