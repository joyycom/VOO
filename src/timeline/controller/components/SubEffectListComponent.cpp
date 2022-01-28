#include "SubEffectListComponent.h"
#include "timeline/models/videoclip.h"
#include <QJsonArray>
#include <QDebug>
#include <QJsonDocument>
#include <QDir>
#include <timeline/controller/timelinecontroller.h>
#include <project/SEProject.h>
#include <timeline/command/PropertyEditCommand.h>
#include <base/utils/SkyVariantUtil.h>

SubEffectListComponent::SubEffectListComponent(QObject *parent) : BaseComponent(parent) {
}

SubEffectListComponent::~SubEffectListComponent() {
    delete m_effectModel;
}

EffectListModel *SubEffectListComponent::effectModel() {
    return m_effectModel;
}

static QMap<QString,int> m_subEffectSelectedIndexes;

void SubEffectListComponent::onBindQml(QJsonObject &paramInfo) {
    updateEffectList();

    auto selectIndex = m_subEffectSelectedIndexes.value(m_lastSelectedKey,0);
    if(m_effectModel->rowCount() > selectIndex){
        m_lastSelectedIndex = selectIndex;
    }
}

void SubEffectListComponent::onUnBind() {
    m_effectModel->clearAll();
}


void SubEffectListComponent::onOfParamsChanged(QString key, SkyVariant value) {
    if (key == "Clip:RemoveEffect") {
        updateEffectList();
    } else if (key == "Clip:AddEffect") {
        updateEffectList();
    } else if(key == "Clip:MoveEffectIndex"){
//        int fromIndex = value[0].toInt();
//        int toIndex = value[1].toInt();
        updateEffectList();
    }
}

void SubEffectListComponent::updateEffectList() {
    QList<SkyEffectWrapper *> localEffectList = curClip()->effectList();
    QList<SubEffectBean> list;
    m_lastSelectedKey = "sub_effect_";
    for (int i = 0; i < localEffectList.size(); ++i) {
        if (localEffectList[i]->resourceType() != OverlayResource) {
            auto resourceBean = localEffectList[i]->skyResource();
            auto uiConfig = resourceBean.getUIConfig().object();
            auto componentBean = makeComponentBean(uiConfig);
            list.append(SubEffectBean(localEffectList[i],componentBean));

            m_lastSelectedKey = m_lastSelectedKey.append(resourceBean.name());
        }
    }
    m_effectModel->updateData(list);
    emit effectListChanged();
}

ComponentBean* SubEffectListComponent::makeComponentBean(QJsonObject& jsonObj){
    QJsonArray paramList = jsonObj["paramList"].toArray();
    auto rootComponent = GroupComponentBeans::create("Root", "", "ColumnNoScroll");
    for (int i = 0; i < paramList.count(); ++i) {
        QJsonObject paramInfo = paramList.at(i).toObject();
        auto uiType = paramInfo["uiType"].toString();

        if(!s_subEffectSupportUIType.contains(uiType)){
            continue;
        }
        QString title = paramInfo.value("paramTitle").toString("");
        ComponentBean *bean = ComponentBean::create(uiType, title, paramInfo);
        if(bean->qml.isEmpty()){
            qDebug() << " makeComponentBean NOT FOUND Component Type: " << uiType << " qml:" << bean->qml<<" bean->component:"<<bean->component;
            delete bean;
            continue;
        }
        if(rootComponent != nullptr){
            auto translations = jsonObj["translations"].toObject();
            rootComponent->setTransitions(translations);
        }

        qDebug() << " onSelectedClipChanged Type: " << uiType << " bean: " << bean<< " Param:" << paramInfo;
        rootComponent->appendComponent(bean);
    }

    if(rootComponent->count() == 0){
        delete rootComponent;
        rootComponent = nullptr;
    }else{

    }
    return rootComponent;
}

void SubEffectListComponent::attachComponent(int index, QObject *component){
    auto bean = m_effectModel->data(index);
    bean.componentBean->component = (BaseComponent *)component;
    bean.componentBean->bindQml(curClip(), bean.effectWrapper);
}

SubEffectBean &EffectListModel::data(int index) {
    return m_datas[index];
}

QVariant EffectListModel::data(const QModelIndex &index, int role) const{
    const SubEffectBean &bean = m_datas[index.row()];
    if (role == IconRole) {
        QString thumb = bean.effectWrapper->skyResource().thumb();
        if (thumb.isEmpty()) {
            return "qrc:/image/assets/icon_default_effect.png";
        } else {
            return thumb;
        }
    } else if (role == NameRole)
        return bean.effectWrapper->skyResource().name();
    else if (role == PathRole)
        return bean.effectWrapper->skyResource().entryPath();
    else if (role == EffectTypeRole) {
        auto resourceType = bean.effectWrapper->skyResource().resouceType();
        return nameFromResourceType(resourceType);
    }else if(role == QmlRole){
        if(bean.componentBean == nullptr){
            return "";
        }
        return bean.componentBean->qml;
    }else if(role == IsSelectedRole){
        return bean.componentBean != nullptr && index.row() == m_curSelectedIndex;
    } else if(role == QmlHeightRole){
        return bean.componentBean != nullptr?bean.componentBean->estimateHeight():0;
    }
    return "";
}

void EffectListModel::setSelectedIndex(int curIndex){
    int preIndex = m_curSelectedIndex;
    if(m_curSelectedIndex == curIndex){
        m_curSelectedIndex = -1;
    }else{
        m_curSelectedIndex = curIndex;
    }

    if(preIndex >= 0 && preIndex < m_datas.size()){
        QModelIndex modelIndex = index(preIndex);
        QVector<int> vector;
        vector.append(IsSelectedRole);
        emit dataChanged(modelIndex,modelIndex,vector);
    }

    if(curIndex >= 0 && curIndex < m_datas.size()){
        QModelIndex modelIndex = index(curIndex);
        QVector<int> vector;
        vector.append(IsSelectedRole);
        emit dataChanged(modelIndex,modelIndex,vector);
    }

}

void EffectListModel::updateData(const QList<SubEffectBean> &list) {
    int count = m_datas.size();
    if (count > 0) {
        beginRemoveRows(QModelIndex(), 0, count - 1);
        m_datas.clear();
        endRemoveRows();
    }

    qDebug() << "update Data Size:" << count << " -> " << list.size();

    if (list.size() > 0) {
        beginInsertRows(QModelIndex(), 0, list.size() - 1);
        m_datas.append(list);
        endInsertRows();
    }
}

void EffectListModel::insertResource(int index, SubEffectBean &data) {
    beginInsertRows(QModelIndex(), index, index);
    m_datas.insert(index, data);
    endInsertRows();
}

void EffectListModel::moveResource(int index, int targetIndex){
    if(index == targetIndex){
        return;
    }
    beginMoveRows(QModelIndex(),index,index,QModelIndex(),index > targetIndex ? targetIndex : targetIndex + 1);
    m_datas.move(index,targetIndex);
    endMoveRows();
}

void EffectListModel::removeResource(int index) {
    beginRemoveRows(QModelIndex(), index, index);
    auto item = m_datas.at(index);
    m_datas.removeAt(index);
    delete item.componentBean;
    item.componentBean = nullptr;
    endRemoveRows();
}

void EffectListModel::clearAll(){
    int count = m_datas.size();
    if(count > 0){
        beginRemoveRows(QModelIndex(), 0, count - 1);
        for(int i=0;i<count;++i){
            auto item = m_datas.at(i);
            delete item.componentBean;
        }
        m_datas.clear();
        endRemoveRows();
    }
}

void SubEffectListComponent::setSelected(int index) {
    if(index >= m_effectModel->rowCount()){
        return;
    }

    m_subEffectSelectedIndexes.insert(m_lastSelectedKey,index);

    int curIndex = m_effectModel->curSelectedIndex();
    if(curIndex == index){
        SEProject::current()->dom()->propertyEditController()->updateCurFocusEffect(curClip()->skyEffectWrapper());
        m_effectModel->setSelectedIndex(index);
        curClip()->updateCurFocusEffect(nullptr);
    }else{
        auto bean = m_effectModel->data(index);
        SEProject::current()->dom()->propertyEditController()->updateCurFocusEffect(bean.effectWrapper);
        m_effectModel->setSelectedIndex(index);
        curClip()->updateCurFocusEffect(bean.effectWrapper);
    }
}

void SubEffectListComponent::moveSubEffect(int index, int targetIndex){
//    m_effectModel->moveResource(index,targetIndex);
    TimelineController *timeline = SEProject::current()->dom()->timelineController();
    QModelIndex selectedIndex = timeline->curSelectedIndex();
    MoveSubEffectIndexCommand *command = new MoveSubEffectIndexCommand(timeline->timelineModel(),  selectedIndex,index,targetIndex);
    timeline->addUndoCommand(command);
}

void SubEffectListComponent::cleanSelected(){
    int curIndex = m_effectModel->curSelectedIndex();
    if(curIndex >= 0){
        SEProject::current()->dom()->propertyEditController()->updateCurFocusEffect(curClip()->skyEffectWrapper());
        m_effectModel->setSelectedIndex(curIndex);
        curClip()->updateCurFocusEffect(nullptr);
    }
}

void SubEffectListComponent::onComponentHide(){
    m_lastSelectedIndex = m_effectModel->curSelectedIndex();
    cleanSelected();
}

void SubEffectListComponent::onComponentShow(){
    int index = fmax(fmin(m_lastSelectedIndex,m_effectModel->rowCount() - 1),0);
    if(index >= 0 && m_effectModel->curSelectedIndex() != index){
        setSelected(index);
        m_lastContentY = m_subEffectSelectedIndexes.value(m_lastSelectedKey+"_contentY",0);

        emit contentYChanged(m_lastContentY);
    }
}

void SubEffectListComponent::setContentY(int contentY){
    m_lastContentY = contentY;
    m_subEffectSelectedIndexes.insert(m_lastSelectedKey+"_contentY",contentY);
}

void SubEffectListComponent::removeSubEffect(int index) {
    auto resource = m_effectModel->data(index);
    TimelineController *timeline = SEProject::current()->dom()->timelineController();
    QModelIndex selectedIndex = timeline->curSelectedIndex();
    RemoveSubEffectCommand *command = new RemoveSubEffectCommand(timeline->timelineModel(), resource.effectWrapper->skyResource(), selectedIndex);
    timeline->addUndoCommand(command);
}
