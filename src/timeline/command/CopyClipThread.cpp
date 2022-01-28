#include "CopyClipThread.h"

#include <QDir>
#include <QProcess>

#include "base/utils/threadpools.h"
#include "baseskyundocommand.h"
#include "project/SEProject.h"

static CopyClipThread *s_thread = nullptr;
void CopyClipThread::copy(QUndoStack *stack, BaseClip *clip, double time) {
    if (s_thread && s_thread->isRunning()) {
        s_thread->quit();
        s_thread = nullptr;
    }
    s_thread = new CopyClipThread(stack, clip, time);
    s_thread->start();
    connect(s_thread, &QThread::finished, s_thread, &QObject::deleteLater);
}

CopyClipThread::CopyClipThread(QUndoStack *stack, BaseClip *clip, double resourceTime)
    : m_stack(stack),
      m_clip(clip),
      m_copyFrameTime(resourceTime) {
}

void CopyClipThread::run() {
    QString rootDir = SEProject::current()->rootPath();
    QDir dir(rootDir + "/res");
    if (!dir.exists()) {
        dir.mkpath(rootDir + "/res");
    }
    int imageCount = 1;
    QString imagePath = QFileInfo(dir, "image" + QString::number(imageCount) + ".jpg").filePath();
    while (QFile::exists(imagePath)) {
        imageCount++;
        imagePath = QFileInfo(dir, "image" + QString::number(imageCount) + ".jpg").filePath();
    }
    if (!skyOutputImage(m_clip->resourceBean().skyResouce(), imagePath, m_copyFrameTime)) {
        outputImage(m_clip->resourceBean().entryPath(), imagePath, m_copyFrameTime);
    }
    auto controller = SEProject::current()->dom()->timelineController();
    auto timelineModel = controller->timelineModel();
    QUndoCommand *command = new CopyClipImageCommand(timelineModel, m_clip, imagePath);
    m_stack->push(command);
    s_thread = nullptr;
}

bool CopyClipThread::skyOutputImage(SkyResource resource, QString imagePath, double time) {
    SkyTimeline *timeline = SEProject::current()->dom()->timeline();
    if (timeline) {
        SkyVideoDecoder decoder = timeline->openVideoDecoder(resource);
        if (decoder.isAvailable()) {
            SkyVideoFrame frame = decoder.retrieveVideo(time, SkyRetrieveMode_Accurate);
            QImage image(frame.getRGBA8Data(), frame.getWidth(), frame.getHeight(), QImage::Format_RGBA8888);
            if (!image.isNull()) {
                if (QFile::exists(imagePath)) {
                    QFile::remove(imagePath);
                }
                image.save(imagePath, "JPG");
                return true;
            }
        }
    }
    return false;
}

void CopyClipThread::outputImage(QString videoPath, QString imagePath, double time) {
    QProcess *m_process = new QProcess();
#ifdef Q_OS_WINDOWS
    QString program = QCoreApplication::applicationDirPath().append("/ffmpeg.exe");
#else
    QString program = QCoreApplication::applicationDirPath().append("/../..").append("/ffmpeg");
#endif
    m_process->setProgram(program);
    m_process->setArguments(QStringList() << "-y"
                                          << "-v"
                                          << "quiet"
                                          << "-ss" << QString::number(time) << "-i" << videoPath << "-frames:v"
                                          << "1" << imagePath);
    m_process->start();
    m_process->waitForFinished();
    m_process->close();
}
