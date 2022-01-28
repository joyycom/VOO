#include "CurvedSpeedComponent.h"

#include <QJsonArray>
#include <QDebug>
#include <math.h>

#include <timeline/controller/timelinecontroller.h>

#include <project/SEProject.h>

#include <base/utils/JsonUtils.h>
#include "base/utils/AppConstance.h"

#include <timeline/command/PropertyEditCommand.h>

#pragma execution_character_set("utf-8");

CurvedSpeedBean::CurvedSpeedBean(const QJsonObject &object) {
    name = object.value("name").toString();
    thumb = object.value("thumb").toString();
    auto pointArray = object.value("points").toArray();
    for (int i = 0; i < pointArray.size(); ++i) {
        double x = pointArray[i].toObject().value("x").toDouble();
        double y = pointArray[i].toObject().value("y").toDouble();
        orignalPoints.append(QPointF(x, y));
    }
    curPoints.append(orignalPoints);
    qDebug() << "orignalPoints:" << orignalPoints;
}

CurvedSpeedBean::CurvedSpeedBean(QString n, QString icon, QList<QPointF> points) {
    name = n;
    thumb = icon;
    orignalPoints.clear();
    orignalPoints.append(points);
    curPoints.append(points);
}

void CurvedSpeedBean::updatePoints(QList<QPointF> points) {
    curPoints.clear();
    curPoints.append(points);
}

CurvedSpeedComponent::CurvedSpeedComponent(QObject *parent) : BaseComponent(parent) {
}

void CurvedSpeedComponent::onBindQml(QJsonObject &paramInfo) {

    QString defaultConfigPath = QCoreApplication::applicationDirPath().append("/Configs/curved_speed.conf");
    ;
    QJsonDocument document = JsonUtils::jsonObjectFromFile(defaultConfigPath);
    QMap<QString, SkyVariant> defaultValues;

    QList<CurvedSpeedBean> curvedList;
    if (document.isArray()) {
        QJsonArray arrays = document.array();
        for (int i = 0; i < arrays.size(); ++i) {
            CurvedSpeedBean bean(arrays[i].toObject());
            curvedList.append(bean);
            qDebug() << "onBindQml add Curved " + bean.name;
        }
    }
    QList<QPointF> empty;
    curvedList.insert(0, CurvedSpeedBean("NONE", "qrc:/image/assets/animation_none.png", empty));

    m_model->setDatas(curvedList);



    BaseClip *clip = curClip();
    auto curPoints = clip->getCurvedPoints();
    int selectedIndex = curPoints.isEmpty()?0:1;
    m_model->selectCurvedSpeedBeanByName(clip->getSelectCurvedSpeedName(), selectedIndex);
    m_model->updateCurPoints(curPoints);
    emit bzizerPointsChanged(bzizerPoints());

    auto preview = SEProject::current()->dom()->previewController();
    connect(preview, &PreviewController::videoProgressChanged, this, &CurvedSpeedComponent::onPreviewProgressChanged);
}

void CurvedSpeedComponent::selectCurvedSpeed(int index) {
    if (index == m_model->curSelectCurvedIndex()) {
        return;
    }
    m_model->setSelectedIndex(index);
    CurvedSpeedBean curBean = m_model->data(index);
    setClipCurvedSpeed(index, curBean.curPoints);
}

QVariant CurvedSpeedComponent::listModel() {
    return QVariant::fromValue(m_model);
}

void CurvedSpeedComponent::onUnBind() {
    auto project = SEProject::current();
    if (project) {
        auto preview = SEProject::current()->dom()->previewController();
        disconnect(
            preview, &PreviewController::videoProgressChanged, this, &CurvedSpeedComponent::onPreviewProgressChanged);
    }
}

CurvedSpeedComponent::~CurvedSpeedComponent() {
    if (m_model) {
        delete m_model;
        m_model = nullptr;
    }
}

QVariantList CurvedSpeedComponent::bzizerPoints() {
    CurvedSpeedBean curBean = m_model->curData();
    return toVariantList(curBean.curPoints);
}

void CurvedSpeedComponent::updatePoints(QVariantList list) {
    QList<QPointF> points;
    for (QVariant data : list) {
        points.append(data.toPointF());
    }
    m_model->updateCurPoints(points);

    setClipCurvedSpeed(m_model->curSelectCurvedIndex(), points);
}

void CurvedSpeedComponent::setClipCurvedSpeed(int selectIndex, QList<QPointF> points) {
    BaseClip *clip = curClip();
    if (clip == nullptr || clip->skyClip() == nullptr) {
        return;
    }

    TimelineController *timeline = SEProject::current()->dom()->timelineController();
    QModelIndex index = timeline->curSelectedIndex();
    auto selectName = m_model->data(selectIndex).name;


    ClipSpeedChangedCommand *speedCommand = new ClipSpeedChangedCommand(timeline->timelineModel(), index, clip->getSpeed(), 1.0);
    ClipCurvedSpeedChangedCommand *curvedSpeedCmd =
        new ClipCurvedSpeedChangedCommand(timeline->timelineModel(), index, selectName, points);
    QList<BaseSkyUndoCommand*> list;
    list.append(speedCommand);
    list.append(curvedSpeedCmd);
    GroupSkyUndoCommand* command = new GroupSkyUndoCommand(timeline->timelineModel(),list);
    timeline->addUndoCommand(command);

    qDebug() << "setClipCurvedSpeed:  Speed: " << clip->skyClip()->getSpeed()
             << " Timeline During:" << clip->skyClip()->getTimeline()->getDuration();
}

void CurvedSpeedComponent::resetPoints() {
    CurvedSpeedBean curBean = m_model->curData();
    m_model->updateCurPoints(curBean.orignalPoints);
    setClipCurvedSpeed(m_model->curSelectCurvedIndex(), curBean.orignalPoints);
}

void CurvedSpeedComponent::onPreviewProgressChanged(double progress) {
    auto clip = curClip();
    if (clip == nullptr) {
        return;
    }
    auto timeline = SEProject::current()->dom()->timeline();
    double curTime = timeline->getCurrentTime();
    double clipProgress = 0;
    if (clip->startTime() > curTime) {
        clipProgress = 0;
    } else if (clip->endTime() < curTime) {
        clipProgress = 1;
    } else {
        auto sourcePTS = clip->getSourcePTS(curTime) - clip->cropStartTime();
        clipProgress = sourcePTS / clip->sourceDuring();
    }

    emit resourceProgressChanged(clipProgress);
}

void CurvedSpeedComponent::updateProgress(double progress) {
    auto clip = curClip();
    if (clip == nullptr) {
        return;
    }
    auto preview = SEProject::current()->dom()->previewController();
    double resourceTime = progress * clip->sourceDuring() + clip->cropStartTime();
    qDebug() << "updateProgress Clip:" << clip << " progress:" << progress << " resourceTime " << resourceTime << " -> CurProgress:"<<preview->videoProgress();
    auto playPts = clip->getPlayPTS(resourceTime);
    qDebug() << "updateProgress222 Clip:" << clip << " progress:" << progress << " resourceTime " << resourceTime
             << " ->" << playPts;


    preview->seekVideo(playPts);
    preview->stop();
}

CurvedSpeedListModel::~CurvedSpeedListModel() {
    m_datas.clear();
}

void CurvedSpeedComponent::onOfParamsChanged(QString key, SkyVariant value) {
	if (key.compare(OF_PARAMS_SELECT_CURVED_NAME, Qt::CaseInsensitive) == 0) {
        QList<QPointF> points = curClip()->getCurvedPoints();
        int index = points.size()>0?1:0;
        m_model->selectCurvedSpeedBeanByName(curClip()->getSelectCurvedSpeedName(),index);
        m_model->updateCurPoints(points);
        emit bzizerPointsChanged(bzizerPoints());
    } else if (key.compare("Clip:CurvedSpeed", Qt::CaseInsensitive) == 0) {
        auto curPoints = curClip()->getCurvedPoints();
        m_model->updateCurPoints(curPoints);
        emit bzizerPointsChanged(bzizerPoints());
    }
}

int CurvedSpeedListModel::rowCount(const QModelIndex &parent) const {
    return m_datas.size();
}

template <typename T>
QVariantList CurvedSpeedComponent::toVariantList(QList<T> list) {
    QVariantList variants;
    for (auto item : list) {
        variants.append(QVariant::fromValue(item));
    }
    return variants;
}

QVariant CurvedSpeedListModel::data(const QModelIndex &index, int role) const {
    const CurvedSpeedBean &bean = m_datas[index.row()];
    if (role == NameRole)
        return bean.name;
    else if (role == ThumbRole)
        return bean.thumb;
    else if (role == PointsRole)
        return QVariant::fromValue(CurvedSpeedComponent::toVariantList(bean.orignalPoints));
    else if (role == SelectedRole) {
        return m_selectedIndex == index.row();
    }
    return "";
}

void CurvedSpeedListModel::setDatas(QList<CurvedSpeedBean> data) {
    if (m_datas.size() > 0) {
        beginRemoveRows(QModelIndex(), 0, m_datas.size() - 1);
        m_datas.clear();
        endRemoveRows();
    }
    if (data.size() > 0) {
        beginInsertRows(QModelIndex(), 0, data.size() - 1);
        m_datas.append(data);
        endInsertRows();
    }
}

CurvedSpeedBean &CurvedSpeedListModel::data(int index) {
    return m_datas[index];
}

CurvedSpeedBean &CurvedSpeedListModel::curData() {
    return m_datas[m_selectedIndex];
}

void CurvedSpeedListModel::updateCurPoints(QList<QPointF> points) {
    m_datas[m_selectedIndex].updatePoints(points);
}

void CurvedSpeedListModel::selectCurvedSpeedBeanByName(QString name, int defaultIndex){
    int selectIndex = defaultIndex;
    int index = 0;
    for(auto const & item:m_datas){
        if(item.name == name){
            selectIndex = index;
            break;
        }
        index++;
    }
    setSelectedIndex(selectIndex);
}

void CurvedSpeedListModel::setSelectedIndex(int i) {
    if (m_selectedIndex == i) {
        return;
    }
    int preIndex = m_selectedIndex;
    m_selectedIndex = i;
    QModelIndex modelIndex = index(i);
    QVector<int> vector;
    vector.append(SelectedRole);
    emit dataChanged(modelIndex, modelIndex, vector);
    emit dataChanged(index(preIndex), index(preIndex), vector);
}

QHash<int, QByteArray> CurvedSpeedListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ThumbRole] = "thumb";
    roles[NameRole] = "name";
    roles[SelectedRole] = "isSelected";
    return roles;
}
