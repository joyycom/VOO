#include "FfmpegProcessWraper.h"
#include <QDebug>
#include <QThreadPool>
#include <QTime>

FfmpegProcessWraper::FfmpegProcessWraper(QString cmd, QObject *parent) : QObject(parent), m_ffmpegCMD(cmd) {
}

void FfmpegProcessWraper::cancel() {
    qDebug() << "FfmpegProcessWraper cancel";
    QProcess *process = m_process;
    if (process != nullptr && process->state() != QProcess::NotRunning) {
        m_process->write("q");
        m_process->closeWriteChannel();
    }
    isCanceled = true;
}

FfmpegProcessWraper::~FfmpegProcessWraper() {
    cancel();
}

class FfmpegTask : public QRunnable {
public:
    FfmpegProcessWraper *m_wraper;

    FfmpegTask(FfmpegProcessWraper *wraper) {
        m_wraper = wraper;
        setAutoDelete(true);
    }

public:
    void run() override {
        m_wraper->sync();
    }
};

FfmpegProcessWraper *FfmpegProcessWraper::start(QString cmd, QObject *parent) {
    FfmpegProcessWraper *wraper = new FfmpegProcessWraper(cmd, parent);
    FfmpegTask *task = new FfmpegTask(wraper);
    QThreadPool::globalInstance()->start(task);
    return wraper;
}

void FfmpegProcessWraper::async() {
    FfmpegTask *task = new FfmpegTask(this);
    QThreadPool::globalInstance()->start(task);
}

void FfmpegProcessWraper::sync() {
    QTime begin = QTime::currentTime();

    emit notifyStateChanged(Start);

    m_process = new QProcess();
    m_process->start(m_ffmpegCMD);
    qDebug() << "FfmpegProcessWraper Start Run CMD:" << m_ffmpegCMD;
    m_process->waitForStarted();
    m_process->setReadChannel(QProcess::StandardError);
    double during = -1;
    double fps = 30;
    QRegExp duringReg("Duration:\\s(\\d{2}):(\\d{2}):(\\d{2}).(\\d{2})");
    QRegExp timeReg("time=(\\d{2}):(\\d{2}):(\\d{2}).(\\d{2})");
    QRegExp fpsReg(",\\s*(\\d+.?\\d*?)\\s*fps,");
    QRegExp frameReg("frame=\\s*(\\d+)");

    emit notifyStateChanged(Progress);
    while (m_process->state() != QProcess::NotRunning) {
        m_process->waitForReadyRead();
        QString output = QString::fromLocal8Bit(m_process->readAllStandardError());
        if (during < 0) {
            if (duringReg.indexIn(output) > -1) {
                during = duringReg.cap(1).toInt() * 3600 + duringReg.cap(2).toInt() * 60 + duringReg.cap(3).toInt() +
                         duringReg.cap(4).toInt() / 100.0;
                qDebug() << "Video During:" << duringReg.cap(0) << " Time:" << during;
            }
        }

        if (fps < 0) {
            if (fpsReg.indexIn(output) > -1) {
                fps = fpsReg.cap(1).toDouble();
                qDebug() << "Video FPS:" << fpsReg.cap(1) << " Time:" << fps;
            }
        }

        if (during > 0 && fps > 0 && frameReg.indexIn(output) > -1) {
            int frame = frameReg.cap(1).toInt();
            double progress1 = frame / (fps * during);
            if (timeReg.indexIn(output) > -1) {
                double time = timeReg.cap(1).toInt() * 3600 + timeReg.cap(2).toInt() * 60 + timeReg.cap(3).toInt() +
                              timeReg.cap(4).toInt() / 100.0;
                double progress2 = time / during;
                emit notifyProgressChanged(std::max(progress1, progress2));
            } else {
                emit notifyProgressChanged(progress1);
            }
        }

        qDebug() << output;
    }

    m_process->waitForFinished();
    qDebug() << "Output:" << QString::fromLocal8Bit(m_process->readAllStandardError());
    QProcess::ExitStatus exitStatus = m_process->exitStatus();
    qDebug() << "FfmpegProcessWraper Done Spend:" << begin.msecsTo(QTime::currentTime()) << " Exit:" << exitStatus;
    delete m_process;
    m_process = nullptr;
    if (exitStatus == QProcess::CrashExit) {
        emit notifyStateChanged(Error);
    } else {
        emit notifyStateChanged(isCanceled ? Canceled : Success);
    }
}
