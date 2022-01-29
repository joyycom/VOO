#ifndef _VN2_CORE_H_
#define _VN2_CORE_H_

/************************************************************************/
/** Module export definition.                                             */
/************************************************************************/

// New macro definition
#if defined(_WIN32) || defined(WIN32)
#   ifdef VN2_VENUS_LIB
#       define VN2_VENUS_API __declspec(dllexport)
#   else
#       define VN2_VENUS_API
#   endif
#elif defined(ANDROID)
#   define VN2_VENUS_API __attribute__ ((visibility("default")))
#else
#   define VN2_VENUS_API
#endif

#endif // _VN2_CORE_H_
