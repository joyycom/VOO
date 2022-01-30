#pragma once

#include "SkyDefs.h"

SKYMEDIA_NAMESPACE_BEGIN

typedef enum {
    SkyError_Success = 0,
    SkyError_Cancel = 1,
    SkyError_NativeIsNull = 2,

    SkyError_ExportCancel = 1,
    SkyError_ExportNativeIsNull = 2,
    SkyError_ExportAllocOutputContext = 100,
    SkyError_ExportOpenVideoStream = 101,
    SkyError_ExportOpenAudioStream = 102,
    SkyError_ExportAvioOpen = 103,
    SkyError_ExportWriteHeader = 104,
} SkyError;

SKYMEDIA_NAMESPACE_END
