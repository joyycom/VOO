//
//  vn2_docrect.h
//  venus
//
//  Created by fengwu on 2020/11/24.
//

#ifndef vn2_docrect_h
#define vn2_docrect_h

#include "vn2_core.h"
#include "vn2_common.h"

#ifdef __cplusplus
extern "C" {
#endif


/// VN2_Create_DocRect
/// @param handle Handle of DocRect context
/// @param argc  Argument counter
/// @param argv  Argument vector
VN2_VENUS_API VN2_Result VN2_Create_DocRect( VN2Handle * handle, const int argc, const void * argv[] );



/// VN2_Destory_DocRect
/// @param handle Handle of DocRect context
VN2_VENUS_API VN2_Result VN2_Destory_DocRect( VN2Handle* handle );


/// VN2_Apply_DocRect_CPU
/// @param handle Handle of DocRect context
/// @param input Input float pointer of a VN2_Image object
/// @param output Ouput float pointer of 4 point
VN2_VENUS_API VN2_Result VN2_Apply_DocRect_CPU( VN2Handle handle, const void * input, void * output );




/// VN_Seg_DocRect
/// @param handle Handle of DocRect context
/// @param name Name of argument u want to set
/// @param value Value argument u want to set
VN2_VENUS_API VN2_Result VN2_Set_DocRect( VN2Handle handle, const char * name, const void * value );


/// VN2_Get_DocRect
/// @param handle Handle of DocRect context
/// @param name Name of argument u want to get
/// @param value Value of argument u want to get
VN2_VENUS_API VN2_Result VN2_Get_DocRect( VN2Handle handle, const char * name, void * value );


#ifdef __cplusplus
}
#endif

#endif /* vn2_docrect_h */

