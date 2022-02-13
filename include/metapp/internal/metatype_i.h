#include "metapp/typekind.h"
#include "metapp/typelist.h"
#include "metapp/metatypedata.h"

namespace metapp {

class MetaType;
bool probablySame(const MetaType * fromMetaType, const MetaType * toMetaType, const bool strictMode = false);
template <typename T>
const MetaType * getMetaType();

namespace internal_ {

template <typename T, typename U>
void podCast(const MetaTypeData & data, void * toData) {
	*(U *)toData = (U)(data.podAs<T>());
}


} // namespace internal_

} // namespace metapp
