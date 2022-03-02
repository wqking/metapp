#ifndef METAPP_METACLASS_I_H_969872685611
#define METAPP_METACLASS_I_H_969872685611

namespace metapp {

namespace internal_ {

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
