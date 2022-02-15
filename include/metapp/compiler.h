#ifndef COMPILER_H_969872685611
#define COMPILER_H_969872685611

#if __cplusplus >= 201703L
	#define METAPP_SUPPORT_STANDARD_17 true
#else
	// Visual Studio 2017 RTW (15.0)
	#if defined(_MSC_VER) && _MSC_VER >= 1910
		#define METAPP_SUPPORT_STANDARD_17 true
	#else
		#define METAPP_SUPPORT_STANDARD_17 false
	#endif
#endif


#endif

