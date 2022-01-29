#ifndef vn2_pose_h
#define vn2_pose_h

#include "vn2_common.h"
#include "vn2_core.h"

#ifdef __cplusplus
extern "C" {
#endif


/// VN2_Create_Pose
/// @param handle Handle of Pose context
/// @param argc  Argument counter
/// @param argv  Argument vector
VN2_VENUS_API VN2_Result VN2_Create_Pose( VN2Handle * handle, const int argc, const void * argv[] );


/// VN2_Destory_Pose
/// @param handle Handle of Pose context
VN2_VENUS_API VN2_Result VN2_Destory_Pose( VN2Handle* handle );


/// VN2_Apply_Pose_CPU
/// @param handle Handle of Pose context
/// @param input Input pointer of type VN2_Image
/// @param output Ouput pointer of type VN_PoseFrameDataArr
VN2_VENUS_API VN2_Result VN2_Apply_Pose_CPU( VN2Handle handle, const void * input, void * output );


/// VN2_Apply_Pose_GPU
/// @param handle Handle of Pose context
/// @param input Input pointer of type VN2_Image
/// @param output Ouput pointer of type VN_PoseFrameDataArr
VN2_VENUS_API VN2_Result VN2_Apply_Pose_GPU( VN2Handle handle, const void * input, void * output );


/// VN_Seg_Pose_Attr
/// @param handle Handle of Pose context
/// @param name Name of argument u want to set
/// @param value Value argument u want to set
VN2_VENUS_API VN2_Result VN2_Set_Pose_Attr( VN2Handle handle, const char * name, const void * value );


/// VN2_Get_Pose_Attr
/// @param handle Handle of Pose context
/// @param name Name of argument u want to get
/// @param value Value of argument u want to get
VN2_VENUS_API VN2_Result VN2_Get_Pose_Attr( VN2Handle handle, const char * name, void * value );


/// VN2_Pose_Process_Result (for Android)
/// @param handle Handle of Pose context
/// @param output In/Output pointer of type VN_PoseFrameDataArr to store pose points data
/// @param mirror If mirror pose points and boxes
/// @param rotate Rotate pose points and pose box clockwise, only support 0, 90, 180, 270 degrees
VN2_VENUS_API VN2_Result VN2_Pose_Process_Result(VN2Handle handle, void * output, bool mirror, int rotate);


#ifdef __cplusplus
}
#endif

#endif /* vn2_pose_h */


