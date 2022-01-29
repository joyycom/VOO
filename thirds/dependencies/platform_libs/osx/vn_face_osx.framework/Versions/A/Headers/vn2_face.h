//
//  vn2_face.h
//  venus
//
//  Created by Yuda Mo on 2020/3/19.
//

#ifndef vn2_face_h
#define vn2_face_h

#include "vn2_core.h"
#include "vn2_common.h"

#ifdef __cplusplus
extern "C" {
#endif


/// VN2_Create_Face
/// @param handle Handle of Face context
/// @param argc  Argument counter
/// @param argv  Argument vector
VN2_VENUS_API VN2_Result VN2_Create_Face( VN2Handle * handle, const int argc, const void * argv[] );


/// VN2_Destory_Face
/// @param handle Handle of Face context
VN2_VENUS_API VN2_Result VN2_Destory_Face( VN2Handle* handle );


/// VN2_Apply_Face_CPU
/// @param handle Handle of Face context
/// @param input Input pointer of type VN2_Image
/// @param output Ouput pointer of type VN2_FaceFrameDataArr
VN2_VENUS_API VN2_Result VN2_Apply_Face_CPU( VN2Handle handle, const void * input, void * output );


/// VN2_Apply_Face_GPU
/// @param handle Handle of Face context
/// @param input Input pointer of type VN2_Image
/// @param output Ouput pointer of type VN2_FaceFrameDataArr
VN2_VENUS_API VN2_Result VN2_Apply_Face_GPU( VN2Handle handle, const void * input, void * output );


/// VN_Seg_Face
/// @param handle Handle of Face context
/// @param name Name of argument u want to set
/// @param value Value argument u want to set
VN2_VENUS_API VN2_Result VN2_Set_Face( VN2Handle handle, const char * name, const void * value );


/// VN2_Get_Face
/// @param handle Handle of Face context
/// @param name Name of argument u want to get
/// @param value Value of argument u want to get
VN2_VENUS_API VN2_Result VN2_Get_Face( VN2Handle handle, const char * name, void * value );


/// VN2_Face_Process_Result (for Android)
/// @param handle Handle of Face context
/// @param output In/Output pointer of type VN2_FaceFrameDataArr to store face points data
/// @param rotate Rotate face points and face box clockwise, only support 0, 90, 180, 270 degrees
VN2_VENUS_API VN2_Result VN2_Process_Face_Result_Rotate(VN2Handle handle, void * output, int rotate);

/// VN2_Face_Process_Result (for Android)
/// @param handle Handle of Face context
/// @param output In/Output pointer of type VN2_FaceFrameDataArr to store face points data
/// mirror face points and boxes
VN2_VENUS_API VN2_Result VN2_Process_Face_Result_Mirror(VN2Handle handle, void * output);

/// VN2_Face_Process_Result (for Android)
/// @param handle Handle of Face context
/// @param output In/Output pointer of type VN2_FaceFrameDataArr to store face points data
/// flip face points and boxes vertically
VN2_VENUS_API VN2_Result VN2_Process_Face_Result_FlipV(VN2Handle handle, void * output);


#ifdef __cplusplus
}
#endif

#endif /* vn2_face_h */

