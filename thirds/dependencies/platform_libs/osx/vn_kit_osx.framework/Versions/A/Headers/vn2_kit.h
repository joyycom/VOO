#ifndef _VN2_KIT_H_
#define _VN2_KIT_H_

#include "vn2_core.h"
#include "vn2_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @brief VN2_SetLogTag
/// @param i_tag Log tag
VN2_VENUS_API VN2_Result VN2_SetLogTag(const char *i_tag);

/// @brief VN2_SetLogCallback
/// @param i_callback LOGCALLBACK function to set
typedef int (*VN2LOGCALLBACK) (const char* i_log);
VN2_VENUS_API VN2_Result VN2_SetLogCallback(VN2LOGCALLBACK i_callback);

/// @brief VN2_SetLogLevel
/// @param i_level in log level
VN2_VENUS_API VN2_Result VN2_SetLogLevel(VN2UInt32 i_level);

/// @brief VN2_GetLogLevel
/// @param o_level out log level
VN2_VENUS_API VN2_Result VN2_GetLogLevel(VN2UInt32* o_level);

/// @brief VN2_Init
/// @param i_contextID GL context id
VN2_VENUS_API VN2_Result VN2_Init(VN2Handle* i_contextID);

/// @brief VN2_Uninit
/// @param i_contextID GL context id
VN2_VENUS_API VN2_Result VN2_Uninit(VN2Handle i_contextID);

/// @brief VN2_CheckGLVersion
VN2_VENUS_API VN2_Result VN2_CheckGLVersion();

/// @brief VN2_Get_VNImage_Bytes
VN2_VENUS_API VN2_Result VN2_Get_VNImage_Bytes(const void * i_img, unsigned int * o_bytes);


/// VN_Create_VNImage_From_PixelBuffer
/// @param i_cvpixelbuffer CVPixelBufferRef pointer
/// @param o_vnimage VN2_Image pointer
/// @param i_gpu_only Only get GPU data to VN2_Image
VN2_VENUS_API VN2_Result VN2_Create_VNImage_From_PixelBuffer(const void * i_cvpixelbuffer,  void * o_vnimage, const bool i_gpu_only);


/// VN_Free_VNImage
/// @param i_cvpixelbuffer CVPixelBufferRef pointer
/// @param o_vnimage VN2_Image pointer
/// @param i_gpu_only Only get GPU data to VN2_Image
VN2_VENUS_API VN2_Result VN2_Free_VNImage(const void * i_cvpixelbuffer, void * io_vnimage, const bool i_gpu_only);

#ifdef _OPENMP
VN2_VENUS_API VN2_Result VN2_Get_Num_Procs(int* num_procs);
VN2_VENUS_API VN2_Result VN2_Set_NUM_Threads(int num_threads);
#endif


#ifdef __cplusplus
}
#endif

#endif // _VN2_KIT_H_

