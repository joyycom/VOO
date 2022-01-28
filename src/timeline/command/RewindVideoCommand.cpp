#include "RewindVideoCommand.h"

#include <base/utils/AppPath.h>
#include <project/SEProject.h>
#include <timeline/models/videoclip.h>

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDebug>
#include <QFileInfo>

RewindVideoCommand::RewindVideoCommand(TimelineModel *timelineModel, QModelIndex index)
    : BaseSkyUndoCommand(timelineModel),
      m_index(index) {
}

void RewindVideoCommand::redo() {
    rewindVideoClip();
}

void RewindVideoCommand::undo() {
    rewindVideoClip();
}

void RewindVideoCommand::cancelRewindVideoClip() {
    if (m_ffmpegWraper != nullptr) {
        m_ffmpegWraper->cancel();
    }
    qDebug() << " cancelRewindVideoClip :" << m_ffmpegWraper;
}

void RewindVideoCommand::rewindVideoClip() {
    m_clip = m_timelineModel->data(m_index);
    if (m_clip == nullptr || m_clip->clipType() != VideoClipType) {
        setObsolete(true);
        return;
    }
    SkyResourceBean bean = m_clip->resourceBean();
    QString rewindVideoPath = bean.rewindVideoPath();
    if (rewindVideoPath.isEmpty() || QFileInfo(rewindVideoPath).size() < 1024) {
        QString md5 = QCryptographicHash::hash(bean.entryPath().toUtf8(), QCryptographicHash::Md5).toHex();
        rewindVideoPath = AppPath::tempPath(md5 + ".mp4");
    }
    QFileInfo rewindFile(rewindVideoPath);
    if (rewindFile.exists()) {
        SkyResource resource(rewindVideoPath.toUtf8().data());
        if (resource.isAvailable() && resource.hasVideoStream()) {
            exchangeVideoResource(rewindVideoPath);
            return;
        }
    }
    performRewindVideo(bean.entryPath(), rewindVideoPath);
}

void RewindVideoCommand::performRewindVideo(QString sourcePath, QString rewindPath) {
#ifdef Q_OS_WINDOWS
    QString program = "ffmpeg.exe";
#else
    QString program = QCoreApplication::applicationDirPath().append("/../..").append("/ffmpeg");
#endif

    QFile targetFile(rewindPath);
    if (targetFile.exists()) {
        targetFile.remove();
    }

    QString targetTmpFilePath = rewindPath + "_tmp.mp4";
    QString cmd = QString("%1 -i %2 -vf reverse -af areverse -y %3").arg(program).arg(sourcePath).arg(targetTmpFilePath);
    m_ffmpegWraper = FfmpegProcessWraper::start(cmd);
    QObject::connect(m_ffmpegWraper, &FfmpegProcessWraper::notifyProgressChanged, [=](const double progress) {
        SEProject::current()->dom()->editPageSource()->notifyProgressUpdate(progress);
    });

    QObject::connect(m_ffmpegWraper,
                     &FfmpegProcessWraper::notifyStateChanged,
                     [this, rewindPath](const FfmpegProcessWraper::ProcessState state) {
                         qDebug() << "performRewindVideo notifyStateChanged:" << state;
                         if (state == FfmpegProcessWraper::Success) {
                             onFfmpegRewindVideoSuccess(rewindPath);
                             SEProject::current()->dom()->editPageSource()->notifyHideProgressDialog();
                         } else if (state == FfmpegProcessWraper::Canceled || state == FfmpegProcessWraper::Error) {
                             onFfmpegRewindVideoFailed(rewindPath);
                             SEProject::current()->dom()->editPageSource()->notifyHideProgressDialog();
                         } else if (state == FfmpegProcessWraper::Start) {
                             SEProject::current()->dom()->editPageSource()->notifyShowProgressDialog();
                         }
                     });
}

void RewindVideoCommand::onFfmpegRewindVideoSuccess(QString rewindPath) {
    QFile tmpFile(rewindPath + "_tmp.mp4");
    if (tmpFile.exists()) {
        QFile targetFile(rewindPath);
        if (tmpFile.rename(targetFile.fileName())) {
            exchangeVideoResource(rewindPath);
            return;
        }
    }
    onFfmpegRewindVideoFailed(rewindPath);
}

void RewindVideoCommand::onFfmpegRewindVideoFailed(QString rewindPath) {
    QFile tmpFile(rewindPath + "_tmp.mp4");
    if (tmpFile.exists()) {
        tmpFile.remove();
    }
    setObsolete(true);
    qInfo() << "onFfmpegRewindVideoFailed ";
}

void RewindVideoCommand::exchangeVideoResource(QString rewindPath) {
    bool result = m_timelineModel->rewindVideoClip(m_index, rewindPath);
    setObsolete(!result);
}
