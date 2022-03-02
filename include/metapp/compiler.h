#ifndef METAPP_COMPILER_H_969872685611
#define METAPP_COMPILER_H_969872685611

#if defined(_MSC_VER)
#define METAPP_COMPILER_VC
#elif defined(__clang__)
#define METAPP_COMPILER_CLANG
#elif defined(__GNUC__)
#define METAPP_COMPILER_GCC
#else
#define METAPP_COMPILER_UNKNOWN
#endif

#if __cplusplus >= 201703L
	#define METAPP_SUPPORT_STANDARD_17
	#define METAPP_NOEXCEPT_BELONGS_TO_FUNCTION_TYPE
#else
	// Visual Studio 2017 RTW (15.0)
	#if defined(_MSC_VER) && _MSC_VER >= 1910
		// the code is inspired by vcruntime.h
		#if defined(_MSVC_LANG) && (_MSVC_LANG > 201402L)
			#define METAPP_SUPPORT_STANDARD_17
		#endif
	#endif
#endif


#endif

