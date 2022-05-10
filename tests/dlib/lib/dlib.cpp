#include "dlib.h"
#include "metapp/allmetatypes.h"

namespace {
LibData libData;
bool hasInitedLibData = false;

void initLibData()
{
	if(hasInitedLibData) {
		return;
	}
	hasInitedLibData = true;
	libData.strHello = "hello";
	libData.var5 = 5;
	libData.mtStrConstPtrVolatilePtr = metapp::getMetaType<std::string const * volatile *>();
}

} // namespace

const LibData * getLibData()
{
	initLibData();
	return &libData;
}
