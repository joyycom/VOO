//
//  vn2_audiodenoise.h
//  venus
//
//  Created by Yuda Mo on 2020/10/19.
//

#ifndef vn2_audiodenoise_h
#define vn2_audiodenoise_h

#include "vn2_core.h"
#include "vn2_common.h"

#ifdef __cplusplus
extern "C" {
#endif


/// VN2_Create_AudioDenoise
/// @param handle Handle of AudioDenoise context
/// @param argc  Argument counter
/// @param argv  Argument vector
VN2_VENUS_API VN2_Result VN2_Create_AudioDenoise( VN2Handle * handle, const int argc, const void * argv[] );

/// VN2_Create_AudioDenoise_DataStream
/// @param handle Handle of AudioDenoise context
/// @param modelnum: the number of models
/// @param modelDatas: model datas with data stream format.
/// @param modelLength: the lengths of each model (Bytes).
VN2_VENUS_API VN2_Result VN2_Create_AudioDenoise_DataStream( VN2Handle * handle, const int modelnum, const char** modelDatas, const long* modelLengths );

/// VN2_Destory_AudioDenoise
/// @param handle Handle of AudioDenoise context
VN2_VENUS_API VN2_Result VN2_Destory_AudioDenoise( VN2Handle* handle );


/// VN2_Apply_AudioDenoise_CPU
/// @param handle Handle of AudioDenoise context
/// @param input Input pointer of audio data
/// @param output Ouput pointer of audio data
VN2_VENUS_API VN2_Result VN2_Apply_AudioDenoise_CPU( VN2Handle handle, const void * input, void * output );


/// VN2_ReSet_AudioDenoise_RNN_State_CPU
/// @param handle Handle of AudioDenoise context
VN2_VENUS_API VN2_Result VN2_ReSet_AudioDenoise_RNN_State_CPU( VN2Handle handle );


/// VN_Seg_AudioDenoise
/// @param handle Handle of AudioDenoise context
/// @param name Name of argument u want to set
/// @param value Value argument u want to set
VN2_VENUS_API VN2_Result VN2_Set_AudioDenoise( VN2Handle handle, const char * name, const void * value );


/// VN2_Get_AudioDenoise
/// @param handle Handle of AudioDenoise context
/// @param name Name of argument u want to get
/// @param value Value of argument u want to get
VN2_VENUS_API VN2_Result VN2_Get_AudioDenoise( VN2Handle handle, const char * name, void * value );


#ifdef __cplusplus
}
#endif

#endif /* vn2_audiodenoise_h */

