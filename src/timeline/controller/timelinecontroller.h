#ifndef TIMELINECONTROLLER_H
#define TIMELINECONTROLLER_H

#include <QObject>
#include <QPoint>
#include <QQmlContext>
#include <QUndoStack>
#include <timeline/command/baseskyundocommand.h>
#include "../models/timelinemodel.h"

class TimelineController : public QObject {
    Q_OBJECT
    Q_PROPERTY(int fps READ getFps NOTIFY fpsChanged)
    Q_PROPERTY(double timeScale READ timeScale WRITE setTimeScale NOTIFY timeScaleChanged)
    Q_PROPERTY(QVariantList selection READ selection WRITE setSelection NOTIFY selectionChanged)
    Q_PROPERTY(double cursorTime READ cursorTime WRITE setCursorTime NOTIFY cursorTimeChanged)
    Q_PROPERTY(double totalTime READ totalTime NOTIFY timelineTotalTimeChanged)
public:
    explicit TimelineController(QObject *parent = nullptr);
    ~TimelineController();

    Q_INVOKABLE void attachModel(TimelineModel *model);

    Q_INVOKABLE int getFps() const;
    Q_INVOKABLE double timeScale() const;
    Q_INVOKABLE void setTimeScale(double scale);
    void setSelection(const QVariantList &data);

    void setSelection(int trackIndex, int clipIndex, bool isTransition = false);

    QVariantList selection() const;
    void setCursorTime(const double time);
    double cursorTime() const;
    BaseClip *curSelectedClip();
    TimelineModel *timelineModel();

    Q_INVOKABLE double totalTime();

    Q_INVOKABLE void addResource(SkyResourceBean& item, int trackIndex = -1, double time = -1, const SkyTimeRange& timeRange = SkyTimeRange(), const SkyVariant& params = SkyVariant::makeNull());

    Q_INVOKABLE void addResource(QString item);

    Q_INVOKABLE void addEffect(SkyResourceBean& resource, int trackIndex, int clipIndex);

    Q_INVOKABLE void cropLeft(double time);

    Q_INVOKABLE void cropRight(double time);

    Q_INVOKABLE void cropPositionChanged(double time);

    Q_INVOKABLE void cancelCropPosition();

    Q_INVOKABLE double moveClip(double time, int trackIndex, bool newTrack, bool pushStack = true, double startTime = -1);

    Q_INVOKABLE void splitClip();

    Q_INVOKABLE void updateClipResource(const QString& path, double startTime, double endTime, bool isReuse);

    Q_INVOKABLE void removeClip();

    Q_INVOKABLE void doubleClickClip();

    Q_INVOKABLE void undo();

    Q_INVOKABLE void redo();

    Q_INVOKABLE int getTypeFromIndex(int index);

    Q_INVOKABLE int getClipCount(int index);

    Q_INVOKABLE void setShownTimeRange(double start, double during);

    Q_INVOKABLE double getTimeScale();

    Q_INVOKABLE bool isPlaying();

    Q_INVOKABLE void play();

    Q_INVOKABLE void stop();

    Q_INVOKABLE bool isPlayEnd();

    Q_INVOKABLE bool hasClip();

    Q_INVOKABLE QVariantList getClipEdgeList(int trackIndex, int clipIndex);

    Q_INVOKABLE void rewindVideoClip();

    Q_INVOKABLE void preparewCopySelectedClip();

    Q_INVOKABLE void copySelectedClip();

    Q_INVOKABLE void copyClip();

    Q_INVOKABLE void copyCurrentFrameFromClip();

    Q_INVOKABLE void copyFirstFrameFromClip();

    Q_INVOKABLE void copyEndFrameFromClip();

    Q_INVOKABLE void addTransitionModelIndex(int trackIndex, int clipIndex);

    Q_INVOKABLE void setTransitionDuring(double during);

    Q_INVOKABLE void setTransitionDuring(double preDuring, double during);

    Q_INVOKABLE void attachAsyncProcessModel(int trackIndex, int clipIndex, AsyncProcessModel *model);

    /*
     * 是否有输入器配置
     * -1:没选中或者不支持，0:没配置，1:有配置
     * */
    Q_INVOKABLE int hasInputDevice(int trackIndex, int clipIndex);

    Q_INVOKABLE int configInputDevice(int inputDeviceType);

    Q_INVOKABLE bool cropVisible();

    Q_INVOKABLE QVariantList getCropInfo();

    Q_INVOKABLE void
    cropVideo(double rotation, int radioIndex, double oriX, double oriY, double x, double y, double w, double h);

    void cancelRewindVideoClip();

    SkyTimeRange getResourceShowingRange(QString path);

    void saveInstanceState();
    void restoreInstanceState();

    void addUndoCommand(BaseSkyUndoCommand *command);

    QModelIndex curSelectedIndex();

    void destroyTimelineModel();

    void setOutputMode(bool output);

    bool getOutputMode();

    void showToast(QString msg, int during = 3000);

    Q_INVOKABLE QColor getRandomColor(QString group);

    Q_INVOKABLE void editEffect(int trackIndex,int clipIndex);

signals:
    void fpsChanged();
    void timeScaleChanged();
    void selectionChanged();
    void cursorTimeChanged();
    void undoEnabledChanged(bool enabled);
    void redoEnabledChanged(bool enabled);

    void timelineTotalTimeChanged(double time);

    void clipSelectChanged(bool isSelected, BaseClip *clip = nullptr);

    void clipRewindEnabledChanged(bool enabled);

    void toastContentChanged(QString msg, int during);

    void watchChanged(bool check);

    void undoTextChanged(QString text);
    void redoTextChanged(QString text);

private slots:
    void onVideoProgressChanged(double progress);
    void onUndoTextChanged(QString text);
    void onRedoTextChanged(QString text);

private:
    void updateBestScale(double videoTime, bool notifyBestScale = false);

    void checkTotalTimeChanged(bool notifyBestScale = false);

    TimelineModel *m_timelineModel = nullptr;

    double m_time_scale = 50;
    double m_total_time = 0;
    // Current select clip
    bool m_is_preview_mode = false;
    bool m_is_output_mode = false;
    QUndoStack m_stack;
    QVariantList m_selection;
    double m_cursor_time = 0;

    SkyTimeRange m_screenShowingRange;

    QMap<QString, SkyTimeRange> m_resourceShowingRanges;
    //精准添加转场到片段里
    QModelIndex m_transitionModelIndex;
    SkyResourceBean preparewCopyClipRes;
    SkyTimeRange timeRange;
    SkyVariant preparewCopyClipParams;
};

#endif // TIMELINECONTROLLER_H
