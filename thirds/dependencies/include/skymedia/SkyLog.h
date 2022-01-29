#pragma once

#include "SkyDefs.h"
#include <stdarg.h>

SKYMEDIA_NAMESPACE_BEGIN

typedef enum {
    SkyLogLevel_Verbose = 0,
    SkyLogLevel_Debug = 1,
    SkyLogLevel_Info = 2,
    SkyLogLevel_Warn = 3,
    SkyLogLevel_Error = 4,
} SkyLogLevel;

SKYMEDIA_API void SkyLogWrite(SkyLogLevel level, const char *format, ...) SKY_FORMAT_PRINTF(2, 3);
SKYMEDIA_API void SkyLogWriteV(SkyLogLevel level, const char *format, va_list vlist);

#define SKY_LOG_V(format, ...) \
    SKYMEDIA_NAMESPACE::SkyLogWrite(SKYMEDIA_NAMESPACE::SkyLogLevel_Verbose, format, ##__VA_ARGS__)
#define SKY_LOG_D(format, ...) \
    SKYMEDIA_NAMESPACE::SkyLogWrite(SKYMEDIA_NAMESPACE::SkyLogLevel_Debug, format, ##__VA_ARGS__)
#define SKY_LOG_I(format, ...) \
    SKYMEDIA_NAMESPACE::SkyLogWrite(SKYMEDIA_NAMESPACE::SkyLogLevel_Info, format, ##__VA_ARGS__)
#define SKY_LOG_W(format, ...) \
    SKYMEDIA_NAMESPACE::SkyLogWrite(SKYMEDIA_NAMESPACE::SkyLogLevel_Warn, format, ##__VA_ARGS__)
#define SKY_LOG_E(format, ...) \
    SKYMEDIA_NAMESPACE::SkyLogWrite(SKYMEDIA_NAMESPACE::SkyLogLevel_Error, format, ##__VA_ARGS__)

SKYMEDIA_NAMESPACE_END
