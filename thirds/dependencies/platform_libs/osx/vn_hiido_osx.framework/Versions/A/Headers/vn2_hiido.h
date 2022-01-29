#ifndef _VN_HIIDO_H_
#define _VN_HIIDO_H_

#include "vn2_core.h"
#include "vn2_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/// VN2_UploadInfoToHiiDo
/// @param argc number of string infos
/// @param argv string infos array
VN2_VENUS_API VN2_Result VN2_UploadInfoToHiiDo(const int argc, const char *argv[]);

#ifdef __cplusplus
}
#endif

#endif // _VN2_HIIDO_H_


