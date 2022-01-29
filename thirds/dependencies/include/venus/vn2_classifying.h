//
//  vn2_classifying.h
//  venus
//
//  Created by fengwu on 2021/01/27.
//

#ifndef vn2_classifying_h
#define vn2_classifying_h

#include "vn2_core.h"
#include "vn2_common.h"

#ifdef __cplusplus
extern "C" {
#endif


/// VN2_Create_Classifying
/// @param handle Handle of Classifying context
/// @param argc  Argument counter
/// @param argv  Argument vector
VN2_VENUS_API VN2_Result VN2_Create_Classifying( VN2Handle * handle, const int argc, const void * argv[] );


/// VN2_Destory_Classifying
/// @param handle Handle of Classifying context
VN2_VENUS_API VN2_Result VN2_Destroy_Classifying( VN2Handle* handle );


/// VN2_Apply_Classifying_CPU
/// @param handle Handle of Classifying context
/// @param input Input float pointer of a VN2_Image object
/// @param face_data Input float pointer of a VN2_FaceFrameDataArr object
/// @param output Ouput pointer of a VN2_MultiClsTopNAccArr object
//VN2_VENUS_API VN2_Result VN2_Apply_Classifying_CPU( VN2Handle handle, const void * input, void * output );
VN2_VENUS_API VN2_Result VN2_Apply_Classifying_CPU( VN2Handle handle, const void * input, const void* face_data, void * output);

/// VN2_Apply_VideoLabel_CPU
/// @param handle Handle of Classifying context
/// @param input Input pointer of a VN2_Image object, indicate an array of VN2_Image object.
/// @param frames Input pointer of a int object, used to indicate the number of VN2_image object in input
/// @param output Ouput pointer of a VN2_MultiClsTopNAccArr object
VN2_VENUS_API VN2_Result VN2_Apply_VideoLabel_CPU( VN2Handle handle, const void * input, const void* frames, void * output);

/// VN2_Apply_Classifying_GPU
/// @param handle Handle of Classifying context
/// @param input Input float pointer of a VN2_Image object
/// @param output Ouput pointer of a VN2_Image object
VN2_VENUS_API VN2_Result VN2_Apply_Classifying_GPU( VN2Handle handle, const void * input, void * output );


/// VN_Seg_Classifying
/// @param handle Handle of Classifying context
/// @param name Name of argument u want to set
/// @param value Value argument u want to set
VN2_VENUS_API VN2_Result VN2_Set_Classifying( VN2Handle handle, const char * name, const void * value );


/// VN2_Get_Classifying
/// @param handle Handle of Classifying context
/// @param name Name of argument u want to get
/// @param value Value of argument u want to get
VN2_VENUS_API VN2_Result VN2_Get_Classifying( VN2Handle handle, const char * name, void * value );


#ifdef __cplusplus
}
#endif

#endif /* vn2_classifying_h */

