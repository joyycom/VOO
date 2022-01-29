#pragma once

#include "SkyDefs.h"
#include "SkyTimeline.h"

SKYMEDIA_NAMESPACE_BEGIN

typedef enum {
    SkyExportTaskState_Idle = 0,
    SkyExportTaskState_Exporting = 1,
    SkyExportTaskState_Pause = 2,
    SkyExportTaskState_Success = 3,
    SkyExportTaskState_Error = 4,
} SkyExportTaskState;

class SKYMEDIA_API SkyExportTask final {
public:
    SkyExportTask(const SkyExportTask &) = delete;
    SkyExportTask &operator=(const SkyExportTask &) = delete;

    SkyExportTask(SkyTimeline *timeline, const char *filePath, const SkyEncodingParams &params);
    ~SkyExportTask();

    SkyExportTaskState getState() const;
    SkyExportTaskState exportVideo(const SkyEncodingCallback &callback);
    SkyExportTaskState exportNextFrame(const SkyEncodingCallback &callback);
    bool pause();

private:
    void *m_impl;
};

SKYMEDIA_NAMESPACE_END
