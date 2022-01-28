#include "DropdownListComponent.h"

#include <QJsonArray>
#include <QDebug>
#include "base/utils/AppConstance.h"

DropdownListComponent::DropdownListComponent(QObject *parent) : BaseComponent(parent) {
}

void DropdownListComponent::onBindQml(QJsonObject &paramInfo) {
    m_paramText = translate(paramInfo["paramTitle"].toString());

    QJsonArray paramInfoSettings = paramInfo["paramSettingInfo"].toArray();
    QJsonArray paramDataList = paramInfo["DropDownDataList"].toArray();
    QJsonArray paramValueList = paramInfo["DropDownDataValue"].toArray();

    if (paramInfoSettings.count() == 0 || paramDataList.count() == 0 || paramDataList.count() != paramValueList.count()) {
        qDebug()<<"paramInfoSettings is Empty???"<<paramInfo;
        return;
    }
    for(auto data:paramDataList){
        m_dataList.append(translate(data.toString()));
    }
    for(auto data:paramValueList){
        m_dataValueList.append(data.toString());
    }

    auto param = paramInfoSettings[0].toObject();
    m_ofParam = ParamSettingInfo(param);

    updateCurDataIndex();

    qDebug() << "onBindQml Param:" << paramInfo.toVariantMap();
}

void DropdownListComponent::onUnBind() {
}


void DropdownListComponent::onOfParamsChanged(QString key, SkyVariant value){
    if(key == m_ofParam.paramKey()){
        updateCurDataIndex();
    }
}

void DropdownListComponent::updateCurDataIndex(){
    auto curOfValue = getOfParamVariantValue(m_ofParam.paramKey(),m_ofParam.defVal());

    QString curOfValueStr = "";
    if(curOfValue.type() == QVariant::String){
        curOfValueStr = curOfValue.toString();
    }else {
        curOfValueStr = QString("%1").arg(curOfValue.toInt());
    }
    for(int i=0;i<m_dataValueList.size();++i){
        if(curOfValueStr == m_dataValueList[i]){
            m_curIndex = i;
            break;
        }
    }
    emit curDataIndexChanged(m_curIndex);
}

void DropdownListComponent::selectDataIndex(int index){
    if(m_curIndex == index){
        return;
    }
    auto preValue = getOfParamVariantValue(m_ofParam.paramKey(),m_ofParam.defVal());
    auto curValueStr = m_dataValueList[index];
    QVariant curValue;
    if (m_ofParam.valueType == PARAM_TYPE_FLOAT) {
        curValue = curValueStr.toFloat();
    } else if (m_ofParam.valueType == PARAM_TYPE_INT) {
        curValue = curValueStr.toInt();
    } else if (m_ofParam.valueType == PARAM_TYPE_STRING) {
        curValue = curValueStr;
    }
    updateOfParamVariantValueByUser(m_ofParam.paramKey(),curValue,preValue);
}
