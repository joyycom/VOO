#ifndef REWINDVIDEOCOMMAND_H
#define REWINDVIDEOCOMMAND_H

#include "baseskyundocommand.h"

#include <base/utils/FfmpegProcessWraper.h>

class RewindVideoCommand : public BaseSkyUndoCommand {
public:
    RewindVideoCommand(TimelineModel *timelineModel, QModelIndex index);

    void undo() override;

    void redo() override;

    void cancelRewindVideoClip();

    int id() const override {
        return RewindVideoCmd;
    };

private:
    void rewindVideoClip();

    void performRewindVideo(QString sourcePath, QString rewindPath);

    void exchangeVideoResource(QString rewindPath);

    void onFfmpegRewindVideoSuccess(QString rewindPath);

    void onFfmpegRewindVideoFailed(QString rewindPath);

    QModelIndex m_index;
    BaseClip *m_clip;
    FfmpegProcessWraper *m_ffmpegWraper = nullptr;
};

#endif // REWINDVIDEOCOMMAND_H
