#include "SwitchComponent.h"

#include <QJsonArray>
#include <QDebug>
#include "base/utils/AppConstance.h"

SwitchComponent::SwitchComponent(QObject *parent) : BaseComponent(parent) {
}

void SwitchComponent::onBindQml(QJsonObject &paramInfo) {
    m_switchText = paramInfo["paramTitle"].toString();

    QJsonArray paramInfoSettings = paramInfo["paramSettingInfo"].toArray();
    if (paramInfoSettings.count() == 0) {
        qDebug()<<"paramInfoSettings is Empty???"<<paramInfo;
        return;
    }

    auto paramObj = paramInfoSettings[0].toObject();
    m_ofParams = ParamSettingInfo(paramObj);
    qDebug() << "onBindQml Param:" << paramObj.toVariantMap();
}

void SwitchComponent::onUnBind() {
}

QString SwitchComponent::switchText() const {
    return translate(m_switchText);
}

bool SwitchComponent::switchValue(){
    return getOfParamVariantValue(m_ofParams.paramKey(),m_ofParams.defVal()).toBool();
}

void SwitchComponent::setSwitchValue(bool value){
    auto preValue = getOfParamVariantValue(m_ofParams.paramKey(),m_ofParams.defVal()).toBool();
    if(preValue == value){
        return;
    }
    updateOfParamVariantValueByUser(m_ofParams.paramKey(),value,preValue);
}

void SwitchComponent::onOfParamsChanged(QString key, SkyVariant value){
    if(key == m_ofParams.paramKey()){
        emit switchValueChanged(switchValue());
    }
}
