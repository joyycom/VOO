#ifndef AUDIOWAVWIDGET_H
#define AUDIOWAVWIDGET_H

#include "audiowavdatamgr.h"

#include <QObject>
#include <QQuickPaintedItem>
#include <QWidget>
#include <QtQml>

#include <skymedia/SkyWav.h>

#include <timeline/models/baseclip.h>
SKYMEDIA_USING_NAMESPACE

class AudioWavWidget : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(int clipIndex READ clipIndex WRITE setClipIndex)
    Q_PROPERTY(int trackIndex READ trackIndex WRITE setTrackIndex)
    Q_PROPERTY(double showStartTime READ showStartTime WRITE setShowStartTime)
    Q_PROPERTY(double showEndTime READ showEndTime WRITE setShowEndTime)

public:
    AudioWavWidget(QQuickItem *parent = 0);

    int clipIndex();

    int trackIndex();

    void setClipIndex(int index);

    void setTrackIndex(int index);

    double showStartTime();

    void setShowStartTime(double time);

    double showEndTime();

    void setShowEndTime(double time);

    void paint(QPainter *painter) override;

    void releaseResources() override;
public slots:
    void onAudioResouceLoaded();

private:
    void loadAudioResouce();

    void drawAudioWavBitmap(QPainter *p, AudioWavData *data, double start, double end, int width, int height);
    int m_clipIndex = -1;
    int m_trackIndex = -1;
    double m_showStartTime = 0;
    double m_showEndTime = 0;
    double m_shownDuring = 0;
    double m_shownStart = 0;
    AudioWavData *m_audioWavData = nullptr;
};

#endif // AUDIOWAVWIDGET_H
