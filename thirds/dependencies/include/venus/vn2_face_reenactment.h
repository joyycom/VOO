//
//  vn2_face_reenactment.h
//  venus
//
//  Created by fengwu on 2021/05/18.
//

#ifndef vn2_face_reenactment_h
#define vn2_face_reenactment_h

#include "vn2_core.h"
#include "vn2_common.h"

#ifdef __cplusplus
extern "C" {
#endif


/// VN2_Create_FaceReenactment
/// @param handle Handle of FaceReenactment context
/// @param argc  Argument counter
/// @param argv  Argument vector
VN2_VENUS_API VN2_Result VN2_Create_FaceReenactment( VN2Handle * handle, const int argc, const void * argv[] );


/// VN2_Destory_FaceReenactment
/// @param handle Handle of FaceReenactment context
VN2_VENUS_API VN2_Result VN2_Destroy_FaceReenactment( VN2Handle* handle );


/// VN2_Apply_FaceReenactment_CPU
/// @param handle Handle of FaceReenactment context
/// @param input int value to specify which frame to generate face image
/// @param output Ouput pointer of a VN2_Image object
VN2_VENUS_API VN2_Result VN2_Apply_FaceReenactment_CPU( VN2Handle handle, const void * input, void * output );


/// VN2_Apply_FaceReenactment_GPU
/// @param handle Handle of FaceReenactment context
/// @param input Input float pointer of a VN2_Image object
/// @param output Ouput pointer of a VN2_Image object
VN2_VENUS_API VN2_Result VN2_Apply_FaceReenactment_GPU( VN2Handle handle, const void * input, void * output );


/// VN_Seg_FaceReenactment
/// @param handle Handle of FaceReenactment context
/// @param name Name of argument u want to set
/// @param value Value argument u want to set
VN2_VENUS_API VN2_Result VN2_Set_FaceReenactment( VN2Handle handle, const char * name, const void * value );


/// VN2_Get_FaceReenactment
/// @param handle Handle of FaceReenactment context
/// @param name Name of argument u want to get
/// @param value Value of argument u want to get
VN2_VENUS_API VN2_Result VN2_Get_FaceReenactment( VN2Handle handle, const char * name, void * value );


#ifdef __cplusplus
}
#endif

#endif /* vn2_face_reenactment_h */

