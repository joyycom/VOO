#include "MutliInputTextComponent.h"

#include <QJsonArray>
#include <QDebug>
#include "base/utils/AppConstance.h"

MutliInputTextComponent::MutliInputTextComponent(QObject *parent) : BaseComponent(parent) {
}

void MutliInputTextComponent::onBindQml(QJsonObject &paramInfo) {
    m_labelText = paramInfo["paramTitle"].toString();

    QJsonArray paramInfoSettings = paramInfo["paramSettingInfo"].toArray();
    if (paramInfoSettings.count() == 0) {
        qDebug()<<"paramInfoSettings is Empty???"<<paramInfo;
        return;
    }
    auto paramInfoObj = paramInfoSettings.at(0).toObject();
    m_paramInfo = paramInfoObj;
    qDebug() << "onBindQml Param:" << paramInfo.toVariantMap();
}

QString MutliInputTextComponent::labelText() const{
    return translate(m_labelText);
}

void MutliInputTextComponent::setTextBySlient(QString text){
    QString preText = getText();
    if(preText != text){
        updateOfParamValueBySilent(m_paramInfo.paramKey(),text.toUtf8().data());
    }
}

void MutliInputTextComponent::setText(QString preText,QString text){
    if(preText != text){
        updateOfParamValueByUser(m_paramInfo.paramKey(),text.toUtf8().data(),preText.toUtf8().data());
    }
}

QString MutliInputTextComponent::getText(){
    return getOfParamVariantValue(m_paramInfo.paramKey(),m_paramInfo.defValue).toString();
}


void MutliInputTextComponent::onOfParamsChanged(QString key, SkyVariant v) {
    if (m_paramInfo.paramKey() == key) {
        emit textValueChanged(v.toString());
    }
}

void MutliInputTextComponent::onUnBind() {
}

