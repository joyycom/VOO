#include "audiowavdatamgr.h"

#include <base/utils/threadpools.h>

#include <project/SEProject.h>

AudioWavDataMgr *AudioWavDataMgr::s_instance = nullptr;

AudioWavDataMgr::AudioWavDataMgr(QObject *parent) : QObject(parent) {
}

AudioWavDataMgr *AudioWavDataMgr::instance() {
    if (s_instance == nullptr) {
        s_instance = new AudioWavDataMgr();
    }
    return s_instance;
}

AudioWavData *AudioWavDataMgr::getAudioData(QString path) {
    AudioWavData *data = mAudioWavCache.value(path, nullptr);
    if (data == nullptr) {
        data = new AudioWavData(path);
        data->loadAudioWavData();
        mAudioWavCache[path] = data;
    }
    return data;
}

AudioWavData::AudioWavData(QString path, QObject *parent) : QObject(parent), m_resoucePath(path) {
}

double AudioWavData::during() {
    return m_during;
}

int AudioWavData::sample() {
    return m_sample;
}

std::vector<int8_t> *AudioWavData::wavData() {
    return m_wavData;
}

void AudioWavData::loadAudioWavData() {
    ThreadPools::instance()->exec([=] {
        QTime begin = QTime::currentTime();
        SkyTimeline *timeline = SEProject::current()->dom()->timeline();
        SkyResource resource(m_resoucePath.toUtf8().data());
        SkyAudioDecoder audioDecoder = timeline->openAudioDecoder(resource);

        SkyAudioParams audioParams = timeline->getAudioParams();
        audioParams.format = SkySampleFormat_S16;

        int SAMPLE_RATIO = 10;
        m_sample = audioParams.sampleRate / SAMPLE_RATIO;
        m_during = audioDecoder.getAudioStream().duration;
        int samples = m_sample * m_during;
        std::vector<int8_t> *wavData = new std::vector<int8_t>();

        int A = pow(2, 15);
        int total_min = INT_MAX, total_max = INT_MIN;

        int startSample = 0;
        for (int i = 0; i < samples; ++i) {
            int endSample = startSample + SAMPLE_RATIO;
            auto audioFrame = audioDecoder.retrieveAudio(startSample, endSample, audioParams);
            if (audioFrame.getSampleCount() > 0) {
                const int16_t *audioData = (const int16_t *)audioFrame.getData();
                wavData->push_back(audioData[0] * 128 / A);
            }
            startSample = endSample;
        }
        m_wavData = wavData;
        qDebug() << "loadAudioWavData: Path:" << m_resoucePath << " Sample:" << m_sample << " During:" << m_during
                 << " Spend:" << begin.msecsTo(QTime::currentTime()) << " DataSize:" << m_wavData->size();
        qDebug() << "loadAudioWavData: Path:" << m_resoucePath << " Max:" << total_max << " min:" << total_min
                 << " Result: Max:" << total_max * 128 / A << " Min:" << total_min * 128 / A;
        emit loadAudioDataFinish();
    });
}
