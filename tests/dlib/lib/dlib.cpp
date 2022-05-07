#include "dlib.h"

namespace {
LibData libData;
bool hasInitedLibData = false;

void initLibData()
{
	if(hasInitedLibData) {
		return;
	}
	hasInitedLibData = true;
	libData.name = "hello";
}

} // namespace

const LibData * getLibData()
{
	initLibData();
	return &libData;
}
