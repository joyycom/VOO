//
//  vn2_segment.h
//  venus
//
//  Created by fengwu on 2021/01/27.
//

#ifndef vn2_segment_h
#define vn2_segment_h

#include "vn2_core.h"
#include "vn2_common.h"

#ifdef __cplusplus
extern "C" {
#endif


/// VN2_Create_Segment
/// @param handle Handle of Segment context
/// @param argc  Argument counter
/// @param argv  Argument vector
VN2_VENUS_API VN2_Result VN2_Create_Segment( VN2Handle * handle, const int argc, const void * argv[] );


/// VN2_Destory_Segment
/// @param handle Handle of Segment context
VN2_VENUS_API VN2_Result VN2_Destroy_Segment( VN2Handle* handle );


/// VN2_Apply_Segment_CPU
/// @param handle Handle of Segment context
/// @param input Input float pointer of a VN2_Image object
/// @param output Ouput pointer of a VN2_Image object
VN2_VENUS_API VN2_Result VN2_Apply_Segment_CPU( VN2Handle handle, const void * input, void * output );
VN2_VENUS_API VN2_Result VN2_Apply_HeadSegment_CPU( VN2Handle handle, const void * input, const void* face_data, void * output );


/// VN2_Apply_Segment_GPU
/// @param handle Handle of Segment context
/// @param input Input float pointer of a VN2_Image object
/// @param output Ouput pointer of a VN2_Image object
VN2_VENUS_API VN2_Result VN2_Apply_Segment_GPU( VN2Handle handle, const void * input, void * output );


/// VN_Seg_Segment
/// @param handle Handle of Segment context
/// @param name Name of argument u want to set
/// @param value Value argument u want to set
VN2_VENUS_API VN2_Result VN2_Set_Segment( VN2Handle handle, const char * name, const void * value );


/// VN2_Get_Segment
/// @param handle Handle of Segment context
/// @param name Name of argument u want to get
/// @param value Value of argument u want to get
VN2_VENUS_API VN2_Result VN2_Get_Segment( VN2Handle handle, const char * name, void * value );


#ifdef __cplusplus
}
#endif

#endif /* vn2_segment_h */

