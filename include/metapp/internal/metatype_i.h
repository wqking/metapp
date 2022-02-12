#include "metapp/typekind.h"
#include "metapp/typelist.h"
#include "metapp/metatypedata.h"

namespace metapp {

class MetaType;

namespace internal_ {

struct NoneUpType {};

template <typename T>
struct DeclareMetaTypeBase
{
	using UpType = NoneUpType;

	static constexpr QualifierKind qualifiers = 0;
	
	static void copy(const MetaTypeData & fromData, MetaTypeData & toData) {
		toData = fromData;
	}

	static bool canCast(const MetaType * /*toMetaType*/) {
		return false;
	}

	static void cast(const MetaTypeData & /*data*/ , const MetaType * /*toMetaType*/, void * /*toData*/) {
	}
};

template <typename T, typename U>
void podCast(const MetaTypeData & data, void * toData) {
	*(U *)toData = (U)(data.podAs<T>());
}


} // namespace internal_

} // namespace metapp
