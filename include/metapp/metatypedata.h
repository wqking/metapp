#ifndef METATYPEDATA_H_969872685611
#define METATYPEDATA_H_969872685611

#include <memory>
#include <array>

namespace metapp {

constexpr size_t podSize = (sizeof(long long) > sizeof(long double) ? sizeof(long long) : sizeof(long double));

struct MetaTypeData
{
	std::array<uint8_t, podSize> pod;
	std::shared_ptr<void> object;

	template <typename T>
	T & podAs() const {
		return *(T *)(pod.data());
	}
};


} // namespace metapp

#endif
