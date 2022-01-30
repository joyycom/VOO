//
//  vn2_gesture.h
//  venus
//
//  Created by Yuda Mo on 2020/4/1.
//

#ifndef vn2_gesture_h
#define vn2_gesture_h

#include "vn2_core.h"
#include "vn2_common.h"

#ifdef cplusplus
extern "C" {
#endif

	/*
	 # VN2_Gesture_Create
	 # - handle(VN2Handle): handle of Gesture context
	 # - argc(int): argument counter
	 # - argv(const void *): argument vector
	 */
	VN2_VENUS_API VN2_Result VN2_Gesture_Create( VN2Handle * handle, const int argc, const void * argv[] );

	/*
	 # VN2_Gesture_Destory
	 # - handle: handle of Gesture context
	 */
	VN2_VENUS_API VN2_Result VN2_Gesture_Destory( VN2Handle * handle );

	/*
	 # VN2_Gesture_Apply_CPU
	 # - handle(VN2Handle): handle of Gesture context
	 # - input(VN2_Image *): input image
	 # - output(VN_GestureFrameDataArr *): output mask
	 */
	VN2_VENUS_API VN2_Result VN2_Gesture_Apply_CPU( VN2Handle handle, const void * input, void * output );

	/*
	 # VN2_Gesture_Apply_GPU
	 # - handle(VN2Handle): handle of Gesture context
	 # - input(VN2_Image *): input image, with texture is not NULL
	 # - output(VN_GestureFrameDataArr *): output mask, with output->texture is not NULL
	 */
	VN2_VENUS_API VN2_Result VN2_Gesture_Apply_GPU( VN2Handle handle, const void * input, void * output );

	/*
	 # VN2_Gesture_Set_Attr
	 # - handle(VN2Handle): handle of Gesture context
	 # - name(const char *): name of argument u want to set
	 # - value(const void *): value of argument u want to set
	 */
	VN2_VENUS_API VN2_Result VN2_Gesture_Set_Attr( VN2Handle handle, const char * name, const void * value );

	/*
	 # VN2_Gesture_Get_Attr
	 # - handle(VN2Handle): handle of Gesture context
	 # - name(const char *): name of argument u want to get
	 # - value(void *): value of argument u want to get
	 */
	VN2_VENUS_API VN2_Result VN2_Gesture_Get_Attr( VN2Handle handle, const char * name, void * value );


	/*
	 # VN2_Gesture_Process_Result
	 # - handle(VN2Handle): handle of Gesture context
	 # - output(VN_GestureFrameDataArr *): output gesture box data
	 # - mirror, if true, mirror gesture box
	 # - rotate, rotate gesture box clockwise, only support 0, 90, 180, 270 degrees
	 */
/// VN2_Process_Gesture_Result_Rotate (for Android)
/// @param handle Handle of Gesture context
/// @param output In/Output pointer of type VN_GestureFrameDataArr to store gesture points data
/// @param rotate Rotate gesture points and gesture box clockwise, only support 0, 90, 180, 270 degrees
	VN2_VENUS_API VN2_Result VN2_Process_Gesture_Result_Rotate(VN2Handle handle, void * output, int rotate);

/// VN2_Process_Gesture_Result_Mirror (for Android)
/// @param handle Handle of Gesture context
/// @param output In/Output pointer of type VN_GestureFrameDataArr to store gesture points data
/// mirror gesture points and boxes
	VN2_VENUS_API VN2_Result VN2_Process_Gesture_Result_Mirror(VN2Handle handle, void * output);

/// VN2_Process_Gesture_Result_FlipV (for Android)
/// @param handle Handle of Gesture context
/// @param output In/Output pointer of type VN_GestureFrameDataArr to store gesture points data
/// flip gesture points and boxes vertically
	VN2_VENUS_API VN2_Result VN2_Process_Gesture_Result_FlipV(VN2Handle handle, void * output);

#ifdef cplusplus
}
#endif

#endif /* vn2_gesture_h */
