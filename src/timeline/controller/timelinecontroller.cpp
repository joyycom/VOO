#include "timelinecontroller.h"

#include <base/utils/fileutils.h>
#include <base/utils/threadpools.h>
#include <base/utils/OrangeFilterUtils.h>
#include <project/SEProject.h>
#include <timeline/command/CopyClipThread.h>
#include <timeline/command/PropertyEditCommand.h>
#include <timeline/command/RewindVideoCommand.h>
#include <timeline/command/baseskyundocommand.h>
#include <timeline/controller/components/TransitionComponent.h>
#include <timeline/models/videoclip.h>
#include "base/utils/AppConstance.h"
#include "extension/VSRun.h"

#include <QDebug>
#include <QFile>
#include <QGuiApplication>
#include <QPointF>
#include <QQmlContext>
#include <QScreen>
#define _USE_MATH_DEFINES
#include <math.h>

TimelineController::TimelineController(QObject *parent) : QObject(parent) {
    qDebug() << "TimelineController register Timeline! " << this;
    SEProject::current()->dom()->setTimelineController(this);
    QObject::connect(&m_stack, &QUndoStack::canUndoChanged, this, &TimelineController::undoEnabledChanged);
    QObject::connect(&m_stack, &QUndoStack::canRedoChanged, this, &TimelineController::redoEnabledChanged);
    QObject::connect(&m_stack, &QUndoStack::undoTextChanged, this, &TimelineController::onUndoTextChanged);
    QObject::connect(&m_stack, &QUndoStack::redoTextChanged, this, &TimelineController::onRedoTextChanged);
    QObject::connect(&m_stack, &QUndoStack::undoTextChanged, this, &TimelineController::undoTextChanged);
    QObject::connect(&m_stack, &QUndoStack::redoTextChanged, this, &TimelineController::redoTextChanged);
}

TimelineController::~TimelineController() {
    m_stack.clear();
    m_stack.deleteLater();
    qInfo() << "TimelineController::~TimelineController";
}

void TimelineController::attachModel(TimelineModel *model) {
    m_timelineModel = model;

    PreviewController *previewController = SEProject::current()->dom()->previewController();
    QObject::connect(
        previewController, &PreviewController::videoProgressChanged, this, &TimelineController::onVideoProgressChanged);
    qDebug() << "TimelineController attachModel " << this << " Model:" << model;
    updateBestScale(10, true);
}

int TimelineController::getFps() const {
    return SEProject::current()->dom()->timeline()->getVideoParams().frameRate;
}

void TimelineController::setTimeScale(double scale) {
    m_time_scale = scale;
    emit timeScaleChanged();
    qDebug() << "TimelineController m_time_scale:" << m_time_scale;
    return;
}

double TimelineController::timeScale() const {
    return m_time_scale;
}

void TimelineController::setSelection(const QVariantList &data) {
    qDebug() << "setSelection data:" << data << " CurSelect:" << m_selection << " equals?" << (m_selection == data);
    m_selection = data;
    m_transitionModelIndex = QModelIndex();
    qDebug() << "setSelection: Row:" << data[0] << " Col:" << data[1] << " Transition:" << data[2];
    BaseClip *clip = curSelectedClip();

    if (m_selection[2].toBool()) {
        QModelIndex index = m_timelineModel->makeIndex(m_selection[0].toInt(), m_selection[1].toInt());
        BaseClip *parentClip = m_timelineModel->data(index);
        setCursorTime(parentClip->startTime() + clip->during() / 2);
    }
    bool enableRewind = clip != nullptr && clip->clipType() == VideoClipType;
    emit clipRewindEnabledChanged(enableRewind && !m_selection[2].toBool());
    emit clipSelectChanged(m_selection[0].toInt() >= 0 && m_selection[1].toInt() >= 0, clip);
    emit selectionChanged();
}

void TimelineController::showToast(QString msg, int during) {
    emit toastContentChanged(msg, during);
}

void TimelineController::setSelection(int trackIndex, int clipIndex, bool isTransition) {
    QVariantList list;
    list.append(trackIndex);
    list.append(clipIndex);
    list.append(isTransition);
    setSelection(list);
}

void TimelineController::cropLeft(double time) {
    qDebug() << "cropLeft::" << time;
    QModelIndex index = m_timelineModel->makeIndex(m_selection[0].toInt(), m_selection[1].toInt());
    BaseClip *clip = m_timelineModel->data(index);
    if (clip == nullptr || abs(time - clip->startTime()) < TIME_DEVIATION) {
        m_is_preview_mode = false;
        return;
    }
    SkyTimeRange range(time, clip->endTime());
    m_stack.push(new CropClipCommand(m_timelineModel, index, range));

    if (m_is_preview_mode) {
        m_is_preview_mode = false;
        SEProject::current()->dom()->previewController()->seekVideo(m_cursor_time);
    }
    qDebug() << "CropLeft Index:" << m_stack.index();
}

void TimelineController::cropRight(double time) {
    qDebug() << "cropRight::" << time;
    if (m_selection.isEmpty() || m_selection[0] < 0 || m_selection[1] < 0) {
        return;
    }
    QModelIndex index = m_timelineModel->makeIndex(m_selection[0].toInt(), m_selection[1].toInt());
    BaseClip *clip = m_timelineModel->data(index);
    if (clip == nullptr || abs(time - clip->endTime()) < TIME_DEVIATION) {
        m_is_preview_mode = false;
        return;
    }
    SkyTimeRange range(clip->startTime(), time);
    m_stack.push(new CropClipCommand(m_timelineModel, index, range));
    if (m_is_preview_mode) {
        m_is_preview_mode = false;
        SEProject::current()->dom()->previewController()->seekVideo(m_cursor_time);
    }

    checkTotalTimeChanged();
    qDebug() << "CropRight Index:" << m_stack.index();
}

void TimelineController::cropPositionChanged(double time) {
    if (m_selection.isEmpty() || m_selection[0] < 0 || m_selection[1] < 0) {
        return;
    }
    qDebug() << "cropPositionChanged::" << time;
    m_is_preview_mode = true;
    PreviewController *previewController = SEProject::current()->dom()->previewController();
    if (previewController->isPlaying()) {
        previewController->stop();
    }
    previewController->seekVideo(time);
}

void TimelineController::cancelCropPosition() {
    m_is_preview_mode = false;
    qDebug() << "cancelCropPosition::";
}

void TimelineController::splitClip() {
    if (m_cursor_time < 0 || m_selection.isEmpty() || m_selection[0] < 0 || m_selection[1] < 0) {
        return;
    }
    qDebug() << "splitClip::" << m_cursor_time << " Row:" << m_selection[0] << " Col:" << m_selection[1];
    m_stack.push(new SplitClipCommand(m_timelineModel, m_selection[0].toInt(), m_selection[1].toInt(), m_cursor_time));
}

void TimelineController::updateClipResource(const QString &path, double startTime, double endTime, bool isReuse) {
    auto command =
        new ClipUpdateResourceCommand(timelineModel(), curSelectedIndex(), path, startTime, endTime, isReuse);
    addUndoCommand(command);
}

QVariantList TimelineController::selection() const {
    return m_selection;
}

void TimelineController::setCursorTime(const double time) {
    qDebug() << "setCursorTime:" << time << " DOM:" << SEProject::current()->dom();
    SEProject::current()->dom()->previewController()->seekVideo(time);
}

double TimelineController::cursorTime() const {
    return m_cursor_time;
}

double
TimelineController::moveClip(double startTime, int trackIndex, bool newTrack, bool pushStack, double preStartTime) {
    if (m_selection.isEmpty() || m_selection[0] < 0 || m_selection[1] < 0) {
        qDebug() << "MoveClip Invalid:";
        return -1;
    }
    QModelIndex index = m_timelineModel->makeIndex(m_selection[0].toInt(), m_selection[1].toInt());
    MoveClipCommand *cmd = new MoveClipCommand(m_timelineModel, index, preStartTime, startTime, trackIndex, newTrack);
    if (pushStack) {
        m_stack.push(cmd);
    } else {
        cmd->redo();
    }
    checkTotalTimeChanged();
    double newPreStartTime = cmd->getPreStartTime();
    if (!pushStack) {
        delete cmd;
        cmd = nullptr;
    }
    return newPreStartTime;
}

void TimelineController::removeClip() {
    if (m_selection.isEmpty() || m_selection[0] < 0 || m_selection[1] < 0) {
        return;
    }
    QModelIndex index = m_timelineModel->makeIndex(m_selection[0].toInt(), m_selection[1].toInt());
    m_stack.push(new RemoveClipCommand(m_timelineModel, index, m_selection[2].toBool()));

    checkTotalTimeChanged();
}

void TimelineController::doubleClickClip() {
    if (m_selection.isEmpty() || m_selection[0] < 0 || m_selection[1] < 0) {
        return;
    }
    BaseClip *clip = curSelectedClip();
    if (clip == nullptr) {
        return;
    }

    const char *path = clip->skyEffect()->getParams().getString("effectPath", "");
    QString qpath = QString::fromStdString(path);
    if (qpath.length() > 0) {
        OrangeFilterUtils::openEffect(qpath);
    }
}

void TimelineController::addResource(SkyResourceBean &item,
                                     int trackIndex,
                                     double time,
                                     const SkyTimeRange &timeRange,
                                     const SkyVariant &params) {
    if (!QFile(item.entryPath()).exists()) {
        qWarning() << "add Resource Failed  File Not Exist! " << item.toString();
        showToast(tr("Add resource failed, File path is not exist!"));
        return;
    }
    if (item.isEffectResource() && item.isVenusEffect()) {
        auto curClip = curSelectedClip();
        if (curClip != nullptr && m_selection[2] == false &&
            (curClip->clipType() == VideoClipType || curClip->clipType() == ImageClipType)) {
            addEffect(item, m_selection[0].toInt(), m_selection[1].toInt());
            return;
        } else {
            showToast(tr("Add resource failed, AI effect can drag into video clip!"));
            qWarning() << "add Resource Failed  Venus Effect Only Add in Video Clip! " << item.toString();
            return;
        }
    }
    if (time < 0) {
        //光标时间+往前挪一帧
        int fps = SEProject::current()->dom()->timeline()->getVideoParams().frameRate;
        double gap = (m_cursor_time * fps) - (int)(m_cursor_time * fps);
        if (gap <= 0.5) {
            time = std::max(0.0, m_cursor_time - gap / fps);
        } else {
            time = m_cursor_time;
        }
    }

    SEProject::current()->dom()->previewController()->stop();

    if ((item.resouceType() == VideoResource || item.resouceType() == ImageResource) &&
        !m_timelineModel->hasVideoClip()) {
        auto videoStream = item.skyResouce().firstVideoStream();
        if (videoStream.height > 0 && videoStream.width > 0) {
            int width = 1080;
            int height = videoStream.height * width / videoStream.width;
            SEProject::current()->dom()->previewController()->setFrameConfig(width, height);
        }
    }

    QUndoCommand *command =
        new AddResourceCommand(m_timelineModel, item, trackIndex, timeRange, time, m_transitionModelIndex, params);
    m_stack.push(command);
    qDebug() << "AddResouce Index:" << m_stack.index() << " Count:" << m_stack.count();
    if (m_timelineModel->rowCount() == 1) {
        QModelIndex index = m_timelineModel->parent(m_timelineModel->makeIndex(0, 0));
        if (m_timelineModel->rowCount(index) == 1) {
            checkTotalTimeChanged(true);
            return;
        }
    }
    checkTotalTimeChanged(false);
}

void TimelineController::addEffect(SkyResourceBean &resource, int trackIndex, int clipIndex) {
    if (!QFile(resource.entryPath()).exists()) {
        qWarning() << "add Resource Failed  File Not Exist! " << resource.toString();
        return;
    }
    SEProject::current()->dom()->previewController()->stop();

    auto index = m_timelineModel->makeIndex(trackIndex, clipIndex);
    QUndoCommand *command = new AddSubEffectCommand(m_timelineModel, resource, index);
    m_stack.push(command);
}

void TimelineController::addResource(QString path) {
    path = QUrl(path).toLocalFile();
    if (!QFile(path).exists()) {
        qWarning() << "add Resource Failed  File Not Exist! Path:" << path;
        return;
    }
    ResourceType type;
    QString name = QFile(path).fileName();
    if (FileUtils::isImageExtension(path)) {
        type = ImageResource;

    } else if (path.endsWith(".ofeffect", Qt::CaseInsensitive)) {
        type = EffectResource;
    } else if (FileUtils::isAudioExtension(path)) {
        type = AudioResource;
    } else if (FileUtils::isVideoExtension(path)) {
        type = VideoResource;
    } else {
        qDebug() << "Not Support File Type: " << path;
        return;
    }
    SkyResourceBean resouce(path, name, type, "", "");
    addResource(resouce);
}

void TimelineController::updateBestScale(double videoTime, bool notifyBestScale) {
    if (videoTime <= 0) {
        return;
    }
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    double maxScaleWidthScale = screenGeometry.width() / 4 / videoTime / 30;
    double bestWidthScale = screenGeometry.width() / videoTime / 30;
    if (bestWidthScale < maxScaleWidthScale) {
        bestWidthScale = maxScaleWidthScale;
    }
    if (ThreadPools::instance()->isMainThread()) {
        QVariant scale;
        QMetaObject::invokeMethod(parent(),
                                  "updateBestScale",
                                  Q_RETURN_ARG(QVariant, scale),
                                  Q_ARG(QVariant, maxScaleWidthScale),
                                  Q_ARG(QVariant, bestWidthScale),
                                  Q_ARG(QVariant, notifyBestScale));
        if (notifyBestScale && scale.isValid()) {
            setTimeScale(scale.toDouble());
        }
    } else {
        ThreadPools::instance()->mainThread([=] {
            QVariant scale;
            QMetaObject::invokeMethod(parent(),
                                      "updateBestScale",
                                      Q_RETURN_ARG(QVariant, scale),
                                      Q_ARG(QVariant, maxScaleWidthScale),
                                      Q_ARG(QVariant, bestWidthScale),
                                      Q_ARG(QVariant, notifyBestScale));
            if (notifyBestScale && scale.isValid()) {
                setTimeScale(scale.toDouble());
            }
        });
    }
}

void TimelineController::undo() {
    if (m_stack.canUndo()) {
        m_stack.undo();
        qDebug() << "undo Index:" << m_stack.index();
        checkTotalTimeChanged();
    }
}

void TimelineController::redo() {
    if (m_stack.canRedo()) {
        m_stack.redo();
        qDebug() << "Redo Index:" << m_stack.index();
        checkTotalTimeChanged();
    }
}

int TimelineController::getTypeFromIndex(int index) {
    if (index >= 0 && m_timelineModel) {
        TimelineTrack *track = m_timelineModel->getTrackFromIndex(index);
        if (track) {
            return track->type();
        }
    }
    return 0;
}

int TimelineController::getClipCount(int index) {
    if (index >= 0 && m_timelineModel) {
        TimelineTrack *track = m_timelineModel->getTrackFromIndex(index);
        if (track) {
            return track->count();
        }
    }
    return 0;
}
void TimelineController::onVideoProgressChanged(double progress) {
    if (m_is_preview_mode || m_is_output_mode) {
        return;
    }
    double time = progress * SEProject::current()->dom()->previewController()->during();
    if (m_cursor_time != time && !isnan(time)) {
        m_cursor_time = time;
        emit cursorTimeChanged();
    }
}

TimelineModel *TimelineController::timelineModel() {
    return m_timelineModel;
}

BaseClip *TimelineController::curSelectedClip() {
    BaseClip *clip = nullptr;
    if (!m_selection.isEmpty() && m_selection[0] >= 0 && m_selection[1] >= 0) {
        QModelIndex index = m_timelineModel->makeIndex(m_selection[0].toInt(), m_selection[1].toInt());
        clip = m_timelineModel->data(index);
        if (clip != nullptr && (clip->clipType() == VideoClipType || clip->clipType() == ImageClipType) &&
            m_selection[2].toBool()) {
            TransitionClip *transClip = ((VideoClip *)clip)->getTransition();
            if (transClip != nullptr) {
                clip = transClip;
            }
        }
    }
    return clip;
}

double TimelineController::totalTime() {
    return m_total_time;
}

void TimelineController::checkTotalTimeChanged(bool notifyBestScale) {
    double time = SEProject::current()->dom()->previewController()->during();
    if (time - m_total_time > 0.0001 || time - m_total_time < -0.0001) {
        qDebug() << "checkTotalTimeChanged Time: " << m_total_time << "->" << time;
        m_total_time = time;
        if (m_cursor_time > m_total_time) {
            setCursorTime(m_total_time);
        }
        updateBestScale(time, notifyBestScale);
    }
}

void TimelineController::restoreInstanceState() {
    ThreadPools::instance()->mainThread([=] {
        m_timelineModel->restoreInstanceState();
    });
    checkTotalTimeChanged(true);
}

void TimelineController::saveInstanceState() {
    m_timelineModel->saveInstanceState();
}

void TimelineController::setShownTimeRange(double start, double during) {
    m_screenShowingRange.begin = start;
    m_screenShowingRange.end = start + during;
    qDebug() << "setShownTimeRange Range:" << m_screenShowingRange.begin << " ," << m_screenShowingRange.end;
}

double TimelineController::getTimeScale() {
    return m_time_scale;
}

bool TimelineController::isPlaying() {
    return SEProject::current()->dom()->previewController()->isPlaying();
}

void TimelineController::play() {
    SEProject::current()->dom()->previewController()->play();
}

void TimelineController::stop() {
    SEProject::current()->dom()->previewController()->stop();
}

bool TimelineController::isPlayEnd() {
    return SEProject::current()->dom()->previewController()->getSkyTimelineView()->isPlayEnd();
}

bool TimelineController::hasClip() {
    if (m_timelineModel) {
        return m_timelineModel->hasClip();
    }
    return false;
}

QVariantList TimelineController::getClipEdgeList(int trackIndex, int clipIndex) {
    return m_timelineModel->getClipEdgeList(trackIndex, clipIndex);
}

SkyTimeRange TimelineController::getResourceShowingRange(QString path) {
    return m_resourceShowingRanges[path];
}

void TimelineController::addUndoCommand(BaseSkyUndoCommand *command) {
    m_stack.push(command);
}

QModelIndex TimelineController::curSelectedIndex() {
    return m_timelineModel->makeIndex(m_selection[0].toInt(), m_selection[1].toInt());
}

void TimelineController::destroyTimelineModel() {
    if (m_timelineModel) {
        delete m_timelineModel;
        m_timelineModel = nullptr;
    }
}

void TimelineController::setOutputMode(bool output) {
    m_is_output_mode = output;
}

bool TimelineController::getOutputMode() {
    return m_is_output_mode;
}

void TimelineController::onRedoTextChanged(QString text) {
    qInfo() << "QStack onRedoTextChanged Index:" << m_stack.index() << " Text:" << text;
}

void TimelineController::onUndoTextChanged(QString text) {
    qInfo() << "QStack onUndoTextChanged Index:" << m_stack.index() << " Text:" << text;
}

void TimelineController::rewindVideoClip() {
    BaseClip *clip = curSelectedClip();
    if (clip && clip->clipType() == VideoClipType) {
        SEProject::current()->dom()->previewController()->stop();

        RewindVideoCommand *cmd = new RewindVideoCommand(m_timelineModel, curSelectedIndex());

        addUndoCommand(cmd);
        qInfo() << "rewindVideoClip Index:" << m_stack.index()
                << " Command Top:" << m_stack.command(m_stack.index() - 1) << " CMD:" << cmd;
    }
}

void TimelineController::preparewCopySelectedClip() {
    BaseClip *clip = curSelectedClip();
    if (clip) {
        SkyResourceBean res = clip->resourceBean();
        if (res.isValidResource()) {
            preparewCopyClipRes = res;
            timeRange = clip->skyClip()->getTrimRange();
            preparewCopyClipParams = clip->backupClipConfig();
        }
    }
}

void TimelineController::copySelectedClip() {
    if (preparewCopyClipRes.isValidResource()) {
        addResource(preparewCopyClipRes, -1, -1, timeRange, preparewCopyClipParams);
    }
}

void TimelineController::copyClip() {
    BaseClip *clip = curSelectedClip();
    QUndoCommand *command = new CopyClipCommand(m_timelineModel, clip);
    m_stack.push(command);
}

void TimelineController::copyCurrentFrameFromClip() {
    auto clip = curSelectedClip();
    if (clip == nullptr) {
        return;
    }
    double time = clip->getSourcePTS(cursorTime());
    CopyClipThread::copy(&m_stack, clip, time);
}

void TimelineController::copyFirstFrameFromClip() {
    auto clip = curSelectedClip();
    if (clip == nullptr) {
        return;
    }
    double time = clip->cropStartTime();
    CopyClipThread::copy(&m_stack, clip, time);
}

void TimelineController::copyEndFrameFromClip() {
    auto clip = curSelectedClip();
    if (clip == nullptr) {
        return;
    }
    double time = clip->cropEndTime();
    CopyClipThread::copy(&m_stack, clip, time);
}

void TimelineController::addTransitionModelIndex(int trackIndex, int clipIndex) {
    m_transitionModelIndex = timelineModel()->makeIndex(trackIndex, clipIndex);
}

void TimelineController::setTransitionDuring(double during) {
    TransitionClip *transClip = dynamic_cast<TransitionClip *>(curSelectedClip());
    if (transClip) {
        if (during < transClip->getMinTransition()) {
            during = transClip->getMinTransition();
        } else if (during > transClip->getMaxTransition()) {
            during = transClip->getMaxTransition();
        }
        transClip->skyTransition()->setDuration(during);
        emit transClip->ofParamChanged(transClip, "Clip:TransitionDuring", during);
    }
}

void TimelineController::setTransitionDuring(double preDuring, double during) {
    if (preDuring == during) {
        return;
    }
    QModelIndex index = curSelectedIndex();
    TransitionDuringChangedCommand *command =
        new TransitionDuringChangedCommand(m_timelineModel, index, during, preDuring);
    addUndoCommand(command);
}

void TimelineController::attachAsyncProcessModel(int trackIndex, int clipIndex, AsyncProcessModel *model) {
    QModelIndex index = m_timelineModel->makeIndex(trackIndex, clipIndex);
    BaseClip *clip = m_timelineModel->data(index);
    if (clip) {
        clip->attachAsyncProcessModel(model);
    }
}

int TimelineController::hasInputDevice(int trackIndex, int clipIndex) {
    if (trackIndex != m_selection[0].toInt() || clipIndex != m_selection[1].toInt()) {
        return -1;
    }
    QModelIndex index = m_timelineModel->makeIndex(trackIndex, clipIndex);
    BaseClip *clip = m_timelineModel->data(index);
    if (clip) {
        auto type = clip->resourceBean().resouceType();
        if (type == VideoResource || type == AudioResource || type == ImageResource || type == TextResource) {
            return clip->getInputId() ? 1 : 0;
        }
    }
    return -1;
}

int TimelineController::configInputDevice(int inputDeviceType) {
    if (inputDeviceType != 0 && inputDeviceType != 1) {
        return -1;
    }
    int returnValue = inputDeviceType;
    BaseClip *clip = curSelectedClip();
    if (inputDeviceType == 0) {
        timelineModel()->addInputDevice(clip);
        returnValue = 1;
    } else {
        timelineModel()->removeInputDevice(clip);
        returnValue = 0;
    }
    m_timelineModel->notifyFromClip(clip);
    return returnValue;
}

bool TimelineController::cropVisible() {
    BaseClip *clip = curSelectedClip();
    if (clip == nullptr) {
        return false;
    }
    return clip->clipType() == VideoClipType || clip->clipType() == ImageClipType;
}

QVariantList TimelineController::getCropInfo() {
    QVariantList data;
    BaseClip *clip = curSelectedClip();
    if (clip == nullptr || (clip->clipType() != VideoClipType && clip->clipType() != ImageClipType)) {
        return data;
    }
    SkyVideoClip *videoClip = ((SkyVideoClip *)clip->skyClip());
    //资源路径
    data.append(clip->resourceBean().entryPath());
    //是否视频
    data.append(clip->clipType() == VideoClipType);
    //旋转角度
    auto transform = videoClip->getCropTransform();
    data.append(int(-transform.rotation * 180 / M_PI));
    //宽高比
    auto stream = clip->skyClip()->getResource().firstVideoStream();
    data.append((float)stream.width / stream.height);
    //宽高比下拉框下标
    data.append(videoClip->getUserData("crop_radio_index").toInt());
    // x
    auto x = videoClip->getUserData("crop_x");
    if (x.isNull()) {
        data.append(-1);
    } else {
        data.append(x.toDouble());
    }
    // y
    auto y = videoClip->getUserData("crop_y");
    if (y.isNull()) {
        data.append(-1);
    } else {
        data.append(y.toDouble());
    }
    auto cropRect = videoClip->getCropRect();
    //裁剪后的宽
    data.append(cropRect.width);
    //裁剪后的高
    data.append(cropRect.height);
    data.append(cropRect.x);
    data.append(cropRect.y);
    return data;
}

void TimelineController::cropVideo(double rotation,
                                   int radioIndex,
                                   double oriX,
                                   double oriY,
                                   double x,
                                   double y,
                                   double w,
                                   double h) {
    QUndoCommand *command =
        new CropVideoCommand(m_timelineModel, curSelectedIndex(), rotation, radioIndex, oriX, oriY, x, y, w, h);
    m_stack.push(command);
}

void TimelineController::cancelRewindVideoClip() {
    const QUndoCommand *cmd = m_stack.command(m_stack.index() - 1);
    if (cmd != nullptr && cmd->id() == RewindVideoCmd) {
        ((RewindVideoCommand *)cmd)->cancelRewindVideoClip();
    }
    qDebug() << "cancelRewindVideoClip TOP CMD:" << cmd;
}

QString colorArray[] = {"#FF6633", "#FFB399", "#FF33FF", "#00B3E6", "#E6B333", "#3366E6", "#999966", "#B34D4D",
                        "#80B300", "#809900", "#E6B3B3", "#6680B3", "#66991A", "#FF99E6", "#DD1A66", "#E6331A",
                        "#33CCAA", "#66994D", "#B366CC", "#4D8000", "#B33300", "#CC80CC", "#66664D", "#991AFF",
                        "#E666FF", "#4DB3FF", "#1AB399", "#E666B3", "#33991A", "#CC9999", "#B3B31A", "#00E680",
                        "#4D8066", "#809980", "#E6FF80", "#1AFF33", "#999933", "#FF3380", "#CCCC00", "#66E64D",
                        "#4D80CC", "#9900B3", "#E64D66", "#4DB380", "#FF4D4D", "#99E6E6", "#6666FF"};

QMap<QString, QColor> groupColorMap;

QColor TimelineController::getRandomColor(QString group) {
    if (groupColorMap.contains(group)) {
        return groupColorMap[group];
    } else {
        QColor color = QColor(colorArray[groupColorMap.size() % colorArray->length()]);
        groupColorMap.insert(group, color);
        return color;
    }
}

void TimelineController::editEffect(int trackIndex, int clipIndex) {
    QModelIndex index = m_timelineModel->makeIndex(trackIndex, clipIndex);
    auto clip = m_timelineModel->data(index);
    if (clip != nullptr) {
        auto dir = clip->resourceBean().resourceDir();
        VSRun::run(QStringList() << dir, QStringList() << EXTENSIONS_OF_EFFECT, [=](const bool result) {
            if (!result) {
                showToast(tr("Visual Studio Code is not installed"));
            }
        });
    }
}
