#include "metapp/typekind.h"
#include "metapp/typelist.h"
#include "metapp/metatypedata.h"

namespace metapp {

class MetaType;
bool probablySame(const MetaType * fromMetaType, const MetaType * toMetaType, const bool strictMode = false);
const void * getDataAddress(const MetaType * metaType, const MetaTypeData & data);
template <typename T>
const MetaType * getMetaType();

namespace internal_ {

struct NoneUpType {};

template <typename T>
struct DeclareMetaTypeBase
{
	using UpType = NoneUpType;

	static constexpr TypeFlags typeFlags = 0;
	
	static void copy(const MetaTypeData & fromData, MetaTypeData & toData) {
		toData = fromData;
	}

	static bool canCast(const MetaType * toMetaType) {
		return probablySame(getMetaType<T>(), toMetaType, true);
	}

	static void cast(const MetaTypeData & data , const MetaType * /*toMetaType*/ , void * toData) {
		const void * value = getDataAddress(getMetaType<T>(), data);
		using U = typename std::remove_reference<T>::type;
		doCast((const U *)value, (U *)toData);
	}

private:
	template <typename U>
	static void doCast(const U * /*value*/, U * /*toData*/, typename std::enable_if<std::is_void<U>::value>::type * = nullptr) {
	}

	template <typename U>
	static void doCast(const U * value , U * toData, typename std::enable_if<! std::is_void<U>::value>::type * = nullptr) {
		*toData = *value;
	}
};

template <typename T, typename U>
void podCast(const MetaTypeData & data, void * toData) {
	*(U *)toData = (U)(data.podAs<T>());
}


} // namespace internal_

} // namespace metapp
