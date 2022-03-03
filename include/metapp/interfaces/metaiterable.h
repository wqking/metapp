#ifndef METAPP_METAITERABLE_H_969872685611
#define METAPP_METAITERABLE_H_969872685611

#include <functional>

namespace metapp {

class Variant;

class MetaIterable
{
public:
	using Callback = std::function<bool (const Variant &)>;
	explicit MetaIterable(
			void (*forEach)(const Variant & value, Callback callback)
		) : forEach(forEach)
	{
	}

	void (*forEach)(const Variant & value, Callback callback);
};


} // namespace metapp

#endif
