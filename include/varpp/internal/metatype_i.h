#include "varpp/common.h"
#include "varpp/typelist.h"

namespace varpp {

struct MetaTypeData;

namespace internal_ {

template <typename T>
struct MetaTypeBase
{
	static constexpr ExtendType extendType = etNone;

	static void copy(const VariantData & fromData, VariantData & toData) {
		toData = fromData;
	}

	static bool canCast(const MetaTypeData * /*toMetaTypeData*/) {
		return false;
	}

	static void cast(const VariantData & /*data*/ , const MetaTypeData * /*toMetaTypeData*/, void * /*toData*/) {
	}
};

inline void emptyCopy(const VariantData & /*fromData*/, VariantData & /*toData*/)
{
}

template <typename T, typename U>
void podCast(const VariantData & data, void * toData) {
	*(U *)toData = (U)(data.podAs<T>());
}

using FundamentalTypeList = TypeList<
	bool,
	char, wchar_t,
	signed char, unsigned char,
	short, unsigned short,
	int, unsigned int,
	long, unsigned long,
	long long, unsigned long long,
	float, double, long double
>;


} // namespace internal_

} // namespace varpp
