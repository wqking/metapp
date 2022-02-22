#ifndef METACLASS_I_H_969872685611
#define METACLASS_I_H_969872685611

namespace metapp {

namespace internal_ {

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

template <typename From, typename To>
void * classCast(void * pointer)
{
	return static_cast<To *>((From *)pointer);
}

struct BaseDerived
{
	const MetaType * metaType;
	void * (*cast)(void * pointer);
};


} // namespace internal_

} // namespace metapp

#endif
