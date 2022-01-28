#ifndef PREVIEWCONTROLLER_H
#define PREVIEWCONTROLLER_H

#include "SkyTimelineView.h"
#include "SkyTimelineView2.h"
#include "MiniPlayerView.h"

#include <QObject>

#include <timeline/models/skyresourcebean.h>

#include <timeline/widget/SkyMediaPlayer.h>

enum PreviewType {
    PreviewTimeline = 0,
    PreivewResourceVideo = 1,
    PreviewResourceAudio = 2,
    PreviewResourceEffect = 3,
    PreviewResourceImage = 4
};

class PreviewController : public QObject {
    Q_OBJECT
public:
    explicit PreviewController(QObject *parent = nullptr);
    ~PreviewController();

    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY playingStateChanged)
    Q_PROPERTY(double videoProgress READ videoProgress WRITE seekVideo NOTIFY videoProgressChanged)
    Q_PROPERTY(int previewType READ previewType WRITE seekVideo NOTIFY previewTypeChanged)

    Q_INVOKABLE void setSkyTimelineView(SkyTimelineView *timelineView);
    Q_INVOKABLE void setSkyTimelineView2(SkyTimelineView2 *timelineView);
    Q_INVOKABLE void setMiniPlayerView(MiniPlayerView *view);
    SkyTimelineView *getSkyTimelineView();
    SkyTimelineView2 *getSkyTimelineView2();
    MiniPlayerView *getMiniPlayerView();

    bool isPlaying() const;

    void seekVideo(double timePts);

    double videoProgress() const;

    Q_INVOKABLE double previewProgress() const;

    Q_INVOKABLE double previewDuring() const;

    void refreshPreview();

    void startPreviewResource(SkyTimeRange &range);
    void stopPreview();

    void startPreviewVideoResource(const SkyResourceBean &bean);

    Q_INVOKABLE void play(SkyTimeRange range = SkyTimeRange());

    Q_INVOKABLE void stop();

    Q_INVOKABLE double during() const;

    Q_INVOKABLE void mute();
    Q_INVOKABLE void unmute();

    Q_INVOKABLE QString formatTime(double time) const;

    Q_INVOKABLE int previewType() const;

    Q_INVOKABLE QString previewResourcePath() const;

    Q_INVOKABLE void attachMediaPlayer(QObject *mediaPlayer, QObject *resoucePreview);

    Q_INVOKABLE QVariantList changeFrame(int frameIndex);
    Q_INVOKABLE QVariantList getFrameConfig();
    Q_INVOKABLE double changeVideoConfig(int width, int height, int fpsIndex);

    void setFrameConfig(int width,int height);
    void notifyFrameConfig();
signals:
    void frameConfig();
    void playingStateChanged(bool isPlaying);
    void videoProgressChanged(double progress);
    void previewTypeChanged(int type);
    void resoucePreviewProgressChanged(double progress);
private slots:
    void onProgressChanged();
    void onPlayEnd();

private:
    SkyTimeRange m_previewTimeRange;
    PreviewType m_previewType = PreviewTimeline;
    QString m_previewResourcePath = "";
    SkyMediaPlayer *m_skyMediaPlayer = nullptr;

    SkyTimeRange m_playRange;

    SkyTimelineView *m_skyTimelineView = nullptr;
    SkyTimelineView2 *m_skyTimelineView2 = nullptr;
    MiniPlayerView *m_miniPlayerView = nullptr;
};

#endif // PREVIEWCONTROLLER_H
