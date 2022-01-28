#include "propertyeditcontroller.h"

#include <base/utils/JsonUtils.h>
#include <project/SEProject.h>
#include <timeline/models/effectclip.h>
#include <timeline/models/videoclip.h>

#include <QHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMap>
#include <QtMath>

PropertyEditController::PropertyEditController(QObject *parent) : QObject(parent) {
    SEProject::current()->dom()->setPropertyEditController(this);
}

PropertyEditController::~PropertyEditController() {
    if(m_rootComponent != nullptr){
        delete  m_rootComponent;
        m_rootComponent = nullptr;
    }
    if (model) {
        delete model;
        model = nullptr;
    }
}

void PropertyEditController::receiveOfMessage(BaseClip *clip, QString msg) {
    if (m_curSelectedClip != clip) {
        return;
    }
    //    for (GroupComponentBeans *group : m_componentGroups) {
    //        for (int i = 0; i < group->count(); ++i) {
    //            ComponentBean *bean = group->get(i);
    //            if (bean->component != nullptr) {
    //                bean->component->ofMessage(msg);
    //            }
    //        }
    //    }
    m_transformComponent->ofMessage(msg);
}

void PropertyEditController::onLoadCompleted() {
    connect(SEProject::current()->dom()->timelineController(),
            &TimelineController::clipSelectChanged,
            this,
            &PropertyEditController::onSelectedClipChanged);

    QObject::connect(SEProject::current()->dom()->timelineController()->timelineModel(),
                     &TimelineModel::removeClipData,
                     this,
                     &PropertyEditController::onTimelineRemoveClipData);
    QObject::connect(SEProject::current()->dom()->timelineController()->timelineModel(),
                     &TimelineModel::clipDataChanged,
                     this,
                     &PropertyEditController::onTimelineClipDataChanged);
};

void PropertyEditController::onSelectedClipChanged(bool isSelected, BaseClip *clip) {
    if (m_curSelectedClip == clip && m_rootComponent != nullptr) {
        qDebug() << " onSelectedClipChanged NOT CHANGED skip!" << clip
                 << " m_componentGroups:" << m_rootComponent;
        return;
    }
    m_curSelectedClip = clip;
    qDebug() << clip << " onSelectedClipChanged ";

    unBindComponents();

    QMap<QString, GroupComponentBeans *> groups;
    QMap<int, GroupComponentBeans *> idGroups;

    if (isSelected && clip != nullptr) {
        QList<GroupComponentBeans*> groupList;
        QJsonObject doc = clip->getUIConfig().object();
        QJsonArray paramList = doc["paramList"].toArray();
        for (int i = 0; i < paramList.count(); ++i) {
            QJsonObject paramInfo = paramList.at(i).toObject();
            auto uiType = paramInfo["uiType"].toString();
            if (uiType == "Transform") {
                m_transformComponent->bindQml(doc, clip, clip->skyEffectWrapper());
                continue;
            }
            if(uiType == "SimpleTransform"){
                m_simpleTransformComponent->bindQml(paramInfo, clip, clip->skyEffectWrapper());
                continue;
            }

            QString title = paramInfo.value("paramTitle").toString("");
            ComponentBean *bean = ComponentBean::create(uiType, title, paramInfo);
            if(bean->qml.isEmpty()){
                qDebug() << clip << " onSelectedClipChanged NOT FOUND Component Type: " << uiType << " qml:" << bean->qml<<" bean->component:"<<bean->component;
                delete bean;
                continue;
            }

            int id = paramInfo.value("id").toInt(-1);

            qDebug() << clip << " onSelectedClipChanged Type: " << uiType << " bean: " << bean;
            QString groupId = paramInfo.value("group").toString("");

            GroupComponentBeans *group = nullptr;
            int parentId = paramInfo.value("parent").toInt(-1);
            group = parentId>=0?idGroups.value(parentId):nullptr;
            if(group == nullptr){
                group = groups.value(groupId);
                if (group == nullptr) {
                    if(bean->isGroupLayout()){
                        group = (GroupComponentBeans*)bean;
                        groupList.append(group);
                    }else{
                        QString title = paramInfo.value("groupTitle").toString("");
                        QString layout = paramInfo.value("groupLayout").toString("");
                        group = GroupComponentBeans::create(groupId, title, layout);
                        groups.insert(groupId, group);
                        groupList.append(group);
                    }
                }
            }
            if(idGroups.contains(id)){
                qWarning()<<" Error! uiconfig duplicate ID:"<<id;
            }
            if(id >= 0 && bean->isGroupLayout()){
                idGroups.insert(id,(GroupComponentBeans*)bean);
            }
            if(group != nullptr && group != bean){
                group->appendComponent(bean);
            }
        }

        if(groupList.size() > 1){
            m_rootComponent = GroupComponentBeans::create("Root", "", "Stack");
            for(auto group:groupList){
                if(group->isGroupLayout() ){
                    ((GroupComponentBeans*)group)->changeGroupUIType();
                }
                ((GroupComponentBeans*)m_rootComponent)->appendComponent(group);
            }
        }else if(groupList.size() == 1){
            m_rootComponent = groupList.at(0);
        }else{

        }

        if(m_rootComponent != nullptr){
            auto translations = doc["translations"].toObject();
            m_rootComponent->setTransitions(translations);
        }

    } else {
        QJsonObject object;
        m_rootComponent = new ComponentBean("Empty", "", object);
    }
    qDebug()<<" onSelectedClipChanged Root:"<<m_rootComponent;

    emit editPanelDataChanged();
    qDebug()<<" onSelectedClipChanged Done !";
}

void PropertyEditController::attachComponent(int index, QObject *component) {
    if(m_rootComponent != nullptr){
        qDebug()<<" m_rootComponent attachComponent  Root:"<<m_rootComponent << " component:"<<component;

        m_rootComponent->component = (BaseComponent *)component;
        m_rootComponent->bindQml(m_curSelectedClip, m_curSelectedClip->skyEffectWrapper());
    }
}


QString PropertyEditController::rootQml(){
    if(m_rootComponent == nullptr){
        return "";
    }
    return m_rootComponent->qml;
}


void PropertyEditController::attachTransformComponent(TransformComponent *component) {
    m_transformComponent = component;
}

void PropertyEditController::attachSimpleTransformComponent(SimpleTransformComponent *component) {
    m_simpleTransformComponent = component;
}

void PropertyEditController::selectClipFromPreview(int x, int y, int previewWidth, int previewHeight) {
    SEProjectDom *dom = SEProject::current()->dom();
    SkyTimeline *timeline = dom->timeline();
    double currentTime = timeline->getCurrentTime();
    TimelineModel *model = dom->timelineController()->timelineModel();
    QVariantList data;
    for (int i = 0; i < model->rowCount(); i++) {
        QModelIndex countIndex = model->parent(model->makeIndex(i, 0));
        for (int j = 0; j < model->rowCount(countIndex); j++) {
            QModelIndex modelIndex = model->makeIndex(i, j);
            BaseClip *clip = model->data(modelIndex);
            if (clip && clip->clipType() != AudioClipType) {
                if (currentTime >= clip->startTime() && currentTime <= clip->endTime()) {
                    QJsonObject doc = clip->getUIConfig().object();
                    QPoint point(x, y);
                    m_transformComponent->bindClip(clip);
                    TransformSelect select = m_transformComponent->getSelectRect(doc, previewWidth, previewHeight);
                    if (select.rect) {
                        bool inRect = isPosInRotationRect(point, *select.rect, select.rotate);
                        if (inRect) {
                            QVariantList selection;
                            selection.append(i);
                            selection.append(j);
                            selection.append(false);
                            dom->timelineController()->setSelection(selection);
                            return;
                        }
                    }
                }
            }
        }
    }
    QVariantList selection;
    selection.append(-1);
    selection.append(-1);
    selection.append(false);
    dom->timelineController()->setSelection(selection);
}

bool PropertyEditController::isPosInRotationRect(QPoint point, QRect rect, int angle) {
    QPoint center = rect.center();
    int hw = rect.width() / 2;
    int hh = rect.height() / 2;
    int x = point.x();
    int y = point.y();
    double r = -angle * (M_PI / 180);
    int nTempX = center.x() + (x - center.x()) * cos(r) - (y - center.y()) * sin(r);
    int nTempY = center.y() + (x - center.x()) * sin(r) + (y - center.y()) * cos(r);
    if (nTempX > center.x() - hw && nTempX < center.x() + hw && nTempY > center.y() - hh && nTempY < center.y() + hh) {
        return true;
    }
    return false;
}

void PropertyEditController::updateUI() {
    if (m_transformComponent) {
        m_transformComponent->updateSourceChanged();
    }
}

int PropertyEditController::videoClipVolume() {
    if (m_curSelectedClip == nullptr) {
        return 0;
    }
    SkyClip *clip = m_curSelectedClip->skyClip();
    return clip->getVolume() * 100;
}

BaseClip *PropertyEditController::curSelectedClip() {
    return m_curSelectedClip;
}

void PropertyEditController::setVideoClipVolume(int volume) {
    if (m_curSelectedClip == nullptr) {
        return;
    }
    SkyClip *clip = m_curSelectedClip->skyClip();
    clip->setVolume(volume / 100.0);

    qDebug() << "setVideoClipVolume" << clip << " --> Volume" << clip->getVolume() << "  " << volume;
}

QString PropertyEditController::effectEditWidgetJSON() {
    if (m_curSelectedClip == nullptr) {
        qDebug() << "CGJ: m_curSelectedClip is null, cannot parse uiinfo " << m_curSelectedClip;
        return "";
    }
    QJsonDocument doc = m_curSelectedClip->getUIConfig();
    return doc.toJson();
}

EffectGridPropertyModel *PropertyEditController::convertToGridModel(QJsonArray jsonArray) {
    model->setDataWithJSON(jsonArray);
    return model;
}

void PropertyEditController::onTimelineRemoveClipData(BaseClip *clip) {
    if (m_curSelectedClip == clip && clip != nullptr) {
        m_curSelectedClip = nullptr;
        unBindComponents();
        qDebug() << " remove Clip And Unbind Components";
    }
}

void PropertyEditController::onTimelineClipDataChanged(BaseClip *clip) {
    if (m_curSelectedClip == clip && m_rootComponent != nullptr) {
        m_rootComponent->onClipParamsChanged();
    }
}

void PropertyEditController::unBindComponents() {
    m_transformComponent->unBind();

    m_simpleTransformComponent->unBind();

    if(m_rootComponent != nullptr){
        delete m_rootComponent;
    }
    m_rootComponent = nullptr;
    emit editPanelDataChanged();
    qDebug() << " Unbind Components";
}

void PropertyEditController::showToast(QString msg, int during) {
    emit toastContentChanged(msg, during);
}


void PropertyEditController::updateCurFocusEffect(SkyEffectWrapper *wrapper){
    if(m_transformComponent != nullptr){
        m_transformComponent->unBind();
    }
    if(wrapper != nullptr){
        auto uiconfig = wrapper->skyResource().getUIConfig().object();
        QJsonObject obj;
        QJsonArray paramList = uiconfig["paramList"].toArray();
        for (int i = 0; i < paramList.count(); ++i) {
            QJsonObject paramInfo = paramList.at(i).toObject();
            auto uiType = paramInfo["uiType"].toString();
            if (uiType == "Transform") {
                m_transformComponent->bindQml(uiconfig,m_curSelectedClip,wrapper);
                break;
            }
        }
    }else{
        if(m_curSelectedClip != nullptr){
            QJsonObject doc = m_curSelectedClip->getUIConfig().object();
            m_transformComponent->bindQml(doc,m_curSelectedClip,wrapper);
        }
    }

}
