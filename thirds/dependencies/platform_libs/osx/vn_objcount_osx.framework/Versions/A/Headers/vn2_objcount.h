//
//  vn2_objcount.h
//  venus
//
//  Created by fengwu on 2020/11/24.
//

#ifndef vn2_objcount_h
#define vn2_objcount_h

#include "vn2_core.h"
#include "vn2_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/// VN2_Create_ObjCount
/// @param handle Handle of ObjCount context
/// @param argc  Argument counter
/// @param argv  Argument vector
VN2_VENUS_API VN2_Result VN2_Create_ObjCount( VN2Handle * handle, const int argc, const void * argv[] );

/// VN2_Destory_ObjCount
/// @param handle Handle of ObjCount context
VN2_VENUS_API VN2_Result VN2_Destory_ObjCount( VN2Handle* handle );

/// VN2_Apply_ObjCount_CPU
/// @param handle Handle of ObjCount context
/// @param input Input pointer of a VN2_Image object
/// @param output Ouput pointer of a VN2_ObjCountDataArr object
VN2_VENUS_API VN2_Result VN2_Apply_ObjCount_CPU( VN2Handle handle, const void * input, void * output );

VN2_VENUS_API VN2_Result VN2_Process_ObjCount_Result_Rotate(VN2Handle handle, void * output, int rotate);

VN2_VENUS_API VN2_Result VN2_Process_ObjCount_Result_Mirror(VN2Handle handle, void * output);

VN2_VENUS_API VN2_Result VN2_Process_ObjCount_Result_FlipV(VN2Handle handle, void * output);

/// VN_Set_ObjCount
/// @param handle Handle of ObjCount context
/// @param name Name of argument u want to set
/// @param value Value argument u want to set
VN2_VENUS_API VN2_Result VN2_Set_ObjCount( VN2Handle handle, const char * name, const void * value );

/// VN2_Get_ObjCount
/// @param handle Handle of ObjCount context
/// @param name Name of argument u want to get
/// @param value Value of argument u want to get
VN2_VENUS_API VN2_Result VN2_Get_ObjCount( VN2Handle handle, const char * name, void * value );


#ifdef __cplusplus
}
#endif

#endif /* vn2_objcount_h */

