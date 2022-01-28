#ifndef FFMPEGPROCESSWRAPER_H
#define FFMPEGPROCESSWRAPER_H

#include <QObject>
#include <QProcess>
#include <QRunnable>

class FfmpegProcessWraper : public QObject {
    Q_OBJECT
public:
    explicit FfmpegProcessWraper(QString cmd, QObject *parent = nullptr);

    ~FfmpegProcessWraper();

    enum ProcessState { IDLE, Start, Progress, Success, Canceled, Error };

    void sync();

    void async();

    void cancel();

    static FfmpegProcessWraper *start(QString cmd, QObject *parent = nullptr);

signals:
    void notifyProgressChanged(double progress);
    void notifyStateChanged(ProcessState state);

private:
    void updateState(ProcessState state);
    bool isCanceled = false;
    QProcess *m_process = nullptr;
    QString m_ffmpegCMD = "";
};

#endif // FFMPEGPROCESSWRAPER_H
