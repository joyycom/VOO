#include "EffectGroupEditorComponent.h"

#include <QJsonArray>
#include <QDebug>
#include "base/utils/AppConstance.h"

EffectGroupEditorComponent::EffectGroupEditorComponent(QObject *parent) : BaseComponent(parent) {
}

void EffectGroupEditorComponent::onBindQml(QJsonObject &paramInfo) {
    m_paramText = translate(paramInfo["paramTitle"].toString());

    m_paramKey = paramInfo["paramKey"].toString("EffectGroupKey");
    QJsonArray paramDataList = paramInfo["GroupList"].toArray();

    if (paramDataList.count() == 0) {
        qDebug()<<"paramInfoSettings is Empty???"<<paramInfo;
        return;
    }
    for(auto data:paramDataList){
        m_dataList.append(data.toString());
    }

    auto defValue = paramInfo["defValue"].toString(m_dataList[0]);
    auto paramData = curEffect()->getUserData(m_paramKey);
    QString curGroup = paramData.isEmpty()?defValue:paramData;
    if(curGroup != "" && !m_dataList.contains(curGroup)){
        m_dataList.insert(0,curGroup);
    }
    setCurGroup(curGroup);


    qDebug() << "onBindQml Param:" << paramInfo.toVariantMap();
}

void EffectGroupEditorComponent::onUnBind() {
}


void EffectGroupEditorComponent::setCurGroup(QString group){
    group = group.trimmed();
    if(group.isEmpty()){
        return;
    }

    m_curGroup = group;
    curEffect()->setUserData(m_paramKey,group);
}


