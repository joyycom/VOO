//
//  vn2_vad.h
//  venus
//
//  Created by Yuda Mo on 2020/10/19.
//

#ifndef vn2_vad_h
#define vn2_vad_h

#include "vn2_core.h"
#include "vn2_common.h"

#ifdef __cplusplus
extern "C" {
#endif


/// VN2_Create_VAD
/// @param handle Handle of VAD context
/// @param argc  Argument counter
/// @param argv  Argument vector
VN2_VENUS_API VN2_Result VN2_Create_VAD( VN2Handle * handle, const int argc, const void * argv[] );

/// VN2_Create_VAD_DataStream
/// @param handle Handle of VAD context
/// @param modelnum: the number of models
/// @param modelDatas: model datas with data stream format.
/// @param modelLength: the lengths of each model (Bytes).
VN2_VENUS_API VN2_Result VN2_Create_VAD_DataStream( VN2Handle * handle, const int modelnum, const char** modelDatas, const long* modelLengths );


/// VN2_Destory_VAD
/// @param handle Handle of VAD context
VN2_VENUS_API VN2_Result VN2_Destory_VAD( VN2Handle* handle );


/// VN2_Apply_VAD_CPU
/// @param handle Handle of VAD context
/// @param input Input float pointer of audio data
/// @param output Ouput float pointer of audio data
VN2_VENUS_API VN2_Result VN2_Apply_VAD_CPU( VN2Handle handle, const void * input, void * output );


/// VN2_ReSet_VAD_RNN_State_CPU
/// @param handle Handle of VAD context
VN2_VENUS_API VN2_Result VN2_ReSet_VAD_RNN_State_CPU( VN2Handle handle );


/// VN_Seg_VAD
/// @param handle Handle of VAD context
/// @param name Name of argument u want to set
/// @param value Value argument u want to set
VN2_VENUS_API VN2_Result VN2_Set_VAD( VN2Handle handle, const char * name, const void * value );


/// VN2_Get_VAD
/// @param handle Handle of VAD context
/// @param name Name of argument u want to get
/// @param value Value of argument u want to get
VN2_VENUS_API VN2_Result VN2_Get_VAD( VN2Handle handle, const char * name, void * value );


#ifdef __cplusplus
}
#endif

#endif /* vn2_vad_h */

