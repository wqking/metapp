#ifndef METAPP_METAITERABLE_H_969872685611
#define METAPP_METAITERABLE_H_969872685611

namespace metapp {

class MetaIterable
{
public:
	explicit MetaIterable(const int length) : length(length) {}

	int getLength() const {
		return length;
	}

private:
	const int length;
};


} // namespace metapp

#endif
