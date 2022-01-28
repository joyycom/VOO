#ifndef AUDIOWAVDATAMGR_H
#define AUDIOWAVDATAMGR_H

#include <QByteArray>
#include <QMap>
#include <QObject>
#include <QSet>
#include <QVector>

class AudioWavData : public QObject {
    Q_OBJECT
public:
    AudioWavData(QString path, QObject *parent = nullptr);

    std::vector<int8_t> *wavData();

    int sample();

    double during();

    void loadAudioWavData();

signals:
    void loadAudioDataFinish();

private:
    QString m_resoucePath;
    std::vector<int8_t> *m_wavData = nullptr;
    int m_sample;
    double m_during;
};

class AudioWavDataMgr : public QObject {
    Q_OBJECT
private:
    AudioWavDataMgr(QObject *parent = nullptr);

public:
    static AudioWavDataMgr *instance();

    AudioWavData *getAudioData(QString path);

private:
    static AudioWavDataMgr *s_instance;
    QMap<QString, AudioWavData *> mAudioWavCache;
};

#endif // AUDIOWAVDATAMGR_H
