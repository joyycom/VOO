#pragma once

#include "skymedia/SkyApi.h"
#include "skymedia/SkyTimelineRenderer.h"
#include <QAudioOutput>
#include "src/project/SEProject.h"
SKYMEDIA_USING_NAMESPACE

class SkyQtAudioOutput final {
public:
    SkyQtAudioOutput() {
        m_output = nullptr;
    }

    ~SkyQtAudioOutput() {
        if (m_output) {
            delete m_output;
            m_output = nullptr;
        }
    }

    void open(const SkyAudioParams &audioParams);

    void cleanup() {
        if (m_output) {
            m_output->stop();
            delete m_output;
        }
        m_output = nullptr;
        m_streamDevice = nullptr;
    }

    void writeAudioData(SkyAudioFrame audioFrame);

    void setState(SkyPlayState state);

    static void cls_setState(void *raw, SkyPlayState state) {
        static_cast<SkyQtAudioOutput *>(raw)->setState(state);
    }

    static SkyAudioOutputClass getClass() {
        auto result = SkyDefaultAudioOutputClass<SkyQtAudioOutput>::getClass();
        result.setState = cls_setState;
        return result;
    }

private:
    QAudioOutput *m_output = nullptr;
    QIODevice *m_streamDevice = nullptr;
    QByteArray m_pcmData;
};
