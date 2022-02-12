#ifndef METATYPEDATA_H_969872685611
#define METATYPEDATA_H_969872685611

#include <memory>

namespace metapp {

constexpr size_t podSize = (sizeof(long long) > sizeof(long double) ? sizeof(long long) : sizeof(long double));

struct MetaTypeData
{
	uint8_t pod[podSize];
	std::shared_ptr<void> object;

	template <typename T>
	T & podAs() const {
		return *(T *)pod;
	}
};


} // namespace metapp

#endif
