#ifndef METAARRAY_H_969872685611
#define METAARRAY_H_969872685611

namespace metapp {

class MetaArray
{
public:
	explicit MetaArray(const int length) : length(length) {}

	int getLength() const {
		return length;
	}

private:
	const int length;
};


} // namespace metapp

#endif
