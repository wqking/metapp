#include "varpp/common.h"
#include "varpp/typelist.h"

namespace varpp {

class MetaType;

namespace internal_ {

template <typename T>
struct DeclareMetaTypeBase
{
	using UpType = void;

	static constexpr ExtendType qualifiers = etNone;
	
	static void copy(const VariantData & fromData, VariantData & toData) {
		toData = fromData;
	}

	static bool canCast(const MetaType * /*toMetaType*/) {
		return false;
	}

	static void cast(const VariantData & /*data*/ , const MetaType * /*toMetaType*/, void * /*toData*/) {
	}
};

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
