#include "AudioWavWidget.h"

#include <QDir>
#include <QPainter>
#include <QQuickItem>

#include <timeline/models/baseclip.h>

#include <project/SEProject.h>

#include <base/utils/AppPath.h>
#include <QPainterPath>

AudioWavWidget::AudioWavWidget(QQuickItem *parent) {
}

void AudioWavWidget::setClipIndex(int index) {
    m_clipIndex = index;
    loadAudioResouce();
}

void AudioWavWidget::setTrackIndex(int index) {
    m_trackIndex = index;
    loadAudioResouce();
}

void AudioWavWidget::loadAudioResouce() {
    if (m_clipIndex < 0 || m_trackIndex < 0) {
        return;
    }
    TimelineModel *timelineModel = SEProject::current()->dom()->timelineController()->timelineModel();
    BaseClip *clip = timelineModel->data(timelineModel->makeIndex(trackIndex(), clipIndex()));
    if (clip == nullptr) {
        return;
    }
    m_audioWavData = AudioWavDataMgr::instance()->getAudioData(clip->entryPath());
    if (m_audioWavData->wavData() == nullptr) {
        connect(m_audioWavData, &AudioWavData::loadAudioDataFinish, this, &AudioWavWidget::onAudioResouceLoaded);
    }
}

void AudioWavWidget::releaseResources() {
    if (m_audioWavData != nullptr) {
        disconnect(m_audioWavData, &AudioWavData::loadAudioDataFinish, this, &AudioWavWidget::onAudioResouceLoaded);
        m_audioWavData = nullptr;
    }
}

void AudioWavWidget::onAudioResouceLoaded() {
    update();
    qDebug() << "Load Finish! Update!!!";
}

int AudioWavWidget::clipIndex() {
    return m_clipIndex;
}

int AudioWavWidget::trackIndex() {
    return m_trackIndex;
}

double AudioWavWidget::showStartTime() {
    return m_showStartTime;
}

void AudioWavWidget::setShowStartTime(double time) {
    qDebug() << "setShowStartTime m_showStartTime:" << m_showStartTime << " start:" << time;
    m_showStartTime = time;
}

double AudioWavWidget::showEndTime() {
    return m_showEndTime;
}

void AudioWavWidget::setShowEndTime(double time) {
    qDebug() << "setShowEndTime m_showEndTime:" << m_showEndTime << " ->" << time;
    m_showEndTime = time;
    update();
}

void AudioWavWidget::paint(QPainter *painter) {
    if (m_audioWavData == nullptr || m_audioWavData->wavData() == nullptr) {
        return;
    }
    qDebug() << "AudioWavWidget::paint Width:" << width() << " Height:" << height()
             << " m_showStartTime:" << m_showStartTime << " m_showEndTime:" << m_showEndTime;

    // painter->fillRect(0, 0, width(), height(), QColor(0x3B,0x3E,0x47));

    double start = m_showStartTime;
    double end = m_showEndTime;
    drawAudioWavBitmap(painter, m_audioWavData, start, end, width(), height());

    m_shownStart = start;
    m_shownDuring = end - start;
    qDebug() << "AudioWavWidget: Width:" << width() << " Height:" << height();
}

void AudioWavWidget::drawAudioWavBitmap(QPainter *p,
                                        AudioWavData *audioData,
                                        double start,
                                        double end,
                                        int width,
                                        int height) {
    p->setPen(QColor(0x54, 0x65, 0xFF)); //波形图画笔

    QTime readFinish = QTime::currentTime();
    int startX = 0, y = height / 2;
    int endX = width;
    int startIndex = start * audioData->sample();
    int endIndex = end * audioData->sample();
    int x = startX;
    std::vector<int8_t> data = *audioData->wavData();
    double step = (endIndex - startIndex) / (double)width;
    QPainterPath path;

    while (x <= endX) {
        int min = INT_MAX, max = INT_MIN;
        int start_i = x * step + startIndex;
        int end_i = fmin(start_i + 200, start_i + step);
        for (int j = start_i; j < end_i; j++) {
            if (j > 0 && j < data.size() && data[j] < min)
                min = data[j];
            if (j > 0 && j < data.size() && data[j] > max)
                max = data[j];
        }
        if (max == INT_MIN) {
            max = 1;
        }
        if (min == INT_MAX) {
            min = -1;
        }
        if (max == min) {
            max = min + 1;
        }
        double y1 = fmax(y + (max * height / 2.0 / 128), y);
        double y2 = fmin(y + (min * height / 2.0 / 128), y);

        path.moveTo(x, y1);
        path.lineTo(x, y2);
        x++;
    }
    p->drawPath(path);
    qDebug() << "drawAudioWavBitmap: Draw Finish endIndex:" << endIndex << " startIndex:" << startIndex
             << " Step:" << step << " Spend:" << QTime::currentTime().msecsTo(readFinish);
    qDebug() << "drawAudioWavBitmap: Draw start:" << start << " end:" << end << " path:" << path.elementCount()
             << " width:" << width << " Data:" << data.size();
}
