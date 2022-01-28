#include "SkyQtAudioOutput.h"
#include <QDebug>

static QAudioFormat convertAudioFormat(const SkyAudioParams &audioParams) {
    QAudioFormat result;
    result.setSampleRate(audioParams.sampleRate);
    result.setChannelCount(audioParams.channelCount);
    result.setCodec("audio/pcm");
    result.setSampleSize(8 * audioParams.bytesPerSamplePerChannel());

    if (audioParams.format == SkySampleFormat_S16) {
        result.setByteOrder(QAudioFormat::LittleEndian);
        result.setSampleType(QAudioFormat::SignedInt);
    } else {
        result.setSampleType(QAudioFormat::Float);
    }
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultOutputDevice();
    if (info.isFormatSupported(result)) {
        return result;
    } else {
        qDebug() << "default format not supported try to use nearest";
        return info.nearestFormat(result);
    }
}

void SkyQtAudioOutput::open(const SkyAudioParams &audioParams) {
    if (m_output) {
        m_output->stop();
        delete m_output;
    }
    QAudioFormat audioFormat = convertAudioFormat(audioParams);
    m_output = new QAudioOutput(audioFormat, 0);
    m_streamDevice = m_output->start();
}

void SkyQtAudioOutput::writeAudioData(SkyAudioFrame audioFrame) {
    if (SEProject::current()->dom()->isMute()) {
        return;
    }

    m_pcmData.append((char *)audioFrame.getData(), audioFrame.getDataSize());
    int presize = m_output->periodSize();

    while (m_pcmData.size() >= presize && m_output->bytesFree() >= presize) {
        QByteArray buffer = m_pcmData.mid(0, presize);
        m_pcmData.remove(0, presize);
        if (m_streamDevice) {
            m_streamDevice->write(buffer);
        }
    }
}

void SkyQtAudioOutput::setState(SkyPlayState state) {
    if (state == SkyPlayState_Pause) {
        m_pcmData.clear();
    }
}
