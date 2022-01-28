#include "ComponentBean.h"
#include <QDebug>
#include <QStringBuilder>
#include <base/utils/SkyVariantUtil.h>
#include <project/SEProject.h>
#include <timeline/command/PropertyEditCommand.h>
#include <timeline/models/effectclip.h>
#include <skymedia/SkyCurve.h>
#include "AnimationListComponent.h"
#include "ColorSelectorComponent.h"
#include "CurvedSpeedComponent.h"
#include "ImageComponent.h"
#include "LayoutComponent.h"
#include "NameComponent.h"
#include "SimpleComponent.h"
#include "SimpleSpeedComponent.h"
#include "SliderComponent.h"
#include "TransitionComponent.h"
#include "VideoComponent.h"
#include "VolumeComponent.h"
#include "WordComponent.h"
#include "base/utils/AppConstance.h"


ParamSettingInfo::ParamSettingInfo() {
}

ParamSettingInfo::ParamSettingInfo(QJsonObject &param, ParamSettingInfo &info) {
    minValue = param["minValue"].toString(QString::number(info.minValue)).toDouble();
    maxValue = param["maxValue"].toString(QString::number(info.maxValue)).toDouble();
    if (param["defValue"].isString()) {
        defValue = param["defValue"].toString(info.defValue);
    }

    paramName = param["paramName"].toString(info.paramName);
    filterIndex = param["filterIndex"].toInt(info.filterIndex);
    valueType = param["valueType"].toString(info.valueType).toLower();
    decimals = param["decimals"].toInt(info.decimals);
    auto format = param["format"].toString();
    auto formats = format.split("${value}");
    m_prefix = info.m_prefix;
    m_suffix = info.m_suffix;
    m_defValue = info.m_defValue;
    m_paramKey = info.m_paramKey;
    if (formats.length() == 2) {
        m_prefix = formats[0];
        m_suffix = formats[1];
    }

    if (valueType == PARAM_TYPE_FLOAT) {
        m_defValue = defValue.toFloat();
    } else if (valueType == PARAM_TYPE_INT) {
        m_defValue = defValue.toInt();
    } else if (valueType == PARAM_TYPE_STRING) {
        m_defValue = defValue;
    } else if (valueType == PARAM_TYPE_COLOR_RGBA) {
        if (param["defValue"].isArray()) {
            auto defValue = param["defValue"].toArray();
            QColor c;
            if (defValue.count() == 4) {
                c.setRedF(defValue.at(0).toDouble());
                c.setGreenF(defValue.at(1).toDouble());
                c.setBlueF(defValue.at(2).toDouble());
                c.setAlphaF(defValue.at(3).toDouble());
                m_defValue = c;
            }
        }else if(param["defValue"].isString()){
            auto defValue = param["defValue"].toString();
            auto colors = defValue.split(",");
            QColor c;
            if (colors.count() == 4) {
                c.setRedF(colors.at(0).toDouble());
                c.setGreenF(colors.at(1).toDouble());
                c.setBlueF(colors.at(2).toDouble());
                c.setAlphaF(colors.at(3).toDouble());
                m_defValue = c;
            }
        }
    }
    m_paramKey = QString("%1:%2").arg(filterIndex).arg(paramName);
}

ParamSettingInfo::ParamSettingInfo(QJsonObject &param) {
    minValue = param["minValue"].toString("1").toDouble();
    maxValue = param["maxValue"].toString("0").toDouble();
    defValue = param["defValue"].toString();
    paramName = param["paramName"].toString();
    filterIndex = param["filterIndex"].toInt();
    valueType = param["valueType"].toString().toLower();
    int defDecimals = valueType == PARAM_TYPE_FLOAT ? 2 : 0;
    decimals = param["decimals"].toInt(defDecimals);
    auto format = param["format"].toString("");
    auto formats = format.split("${value}");
    if (formats.length() == 2) {
        m_prefix = formats[0];
        m_suffix = formats[1];
    }

    if (valueType == PARAM_TYPE_FLOAT) {
        m_defValue = defValue.toFloat();
    } else if (valueType == PARAM_TYPE_INT) {
        m_defValue = defValue.toInt();
    } else if (valueType == PARAM_TYPE_STRING) {
        m_defValue = defValue;
    }else if (valueType == PARAM_TYPE_COLOR_RGBA) {
        if (param["defValue"].isArray()) {
            auto defValue = param["defValue"].toArray();
            QColor c;
            if (defValue.count() == 4) {
                c.setRedF(defValue.at(0).toDouble());
                c.setGreenF(defValue.at(1).toDouble());
                c.setBlueF(defValue.at(2).toDouble());
                c.setAlphaF(defValue.at(3).toDouble());
                m_defValue = c;
            }
        }
    } else if(valueType == PARAM_TYPE_BOOL){
        m_defValue = defValue.compare("true",Qt::CaseInsensitive) == 0;
    }
    m_paramKey = QString("%1:%2").arg(filterIndex).arg(paramName);
}

QVariant ParamSettingInfo::defVal() {
    return m_defValue;
}

QString ParamSettingInfo::paramKey() {
    return m_paramKey;
}

double ParamSettingInfo::stepSize() {
    return pow(10, -decimals);
}

QString ParamSettingInfo::displayText(double val) {
    return QString("%1%2%3").arg(m_prefix).arg(QString::number(val, 'f', decimals)).arg(m_suffix);
}

QString ParamSettingInfo::displayTextPrefix() {
    return m_prefix;
}

QString ParamSettingInfo::displayTextSuffix(){
    return m_suffix;
}

double ParamSettingInfo::textToValue(QString text) {
    int startIndex = 0;
    if (m_prefix.length() > 0 && text.startsWith(m_prefix)) {
        startIndex = m_prefix.length();
    }
    int endIndex = text.length();
    if (m_suffix.length() > 0 && text.endsWith(m_suffix)) {
        endIndex = text.length() - m_suffix.length();
    }
    auto displayValue = text.mid(startIndex, endIndex - startIndex).toDouble();
    return displayValue;
}



double ParamSettingInfo::transformTargetParamValue(double curVal, ParamSettingInfo &target) {
    return (curVal - minValue) * (target.maxValue - target.minValue) / (maxValue - minValue) + target.minValue;
}

ComponentBean::ComponentBean(QString type, QString paramTitle, QJsonObject &param)
    :
      uiType(type),
      title(paramTitle),
      paramObj(param) {
    qml = m_componentDict.value(type,"");
}


ComponentBean::~ComponentBean() {
    unBindQml();
}

void ComponentBean::bindQml(BaseClip *clip,SkyEffectWrapper* wrapper) {
    component->setComponentBean(this);
    component->setTranslations(m_translations);
    component->bindQml(paramObj, clip, wrapper);
}

void ComponentBean::unBindQml(){
    onUnbindQml();
    if (component) {
        component->unBind();
        component = nullptr;
    }
}

void ComponentBean::onClipParamsChanged() {
    if (component) {
        component->onClipParamsChanged();
    }
}

void ComponentBean::setTransitions(QJsonObject& transitions){
    auto locate = AppConstance::locate();
    QString locateName = locate.name();
    QStringList langLocates = locateName.split("_");
    QString lang = "";
    if(langLocates.size() == 2){
        lang = langLocates[0];
    }
    if(lang.isEmpty()){
        return ;
    }
    m_translations = transitions.value(lang).toObject();
}


int ComponentBean::estimateHeight(){
    return m_componentEstimateHeight.value(uiType,40);
}


QString ComponentBean::toString(){
    return "ComponentBean:"+uiType+" ParamSize:"+paramObj.size();
}

ComponentBean* ComponentBean::create(QString type, QString title, QJsonObject &param){
    int index = 0;
    for(auto t:s_groupLayoutType){
        if(t == type){
            return new GroupComponentBeans((GroupComponentBeans::GroupLayout)index, type,title,param);
        }
        index++;
    }
    return new ComponentBean(type,title,param);
}

GroupComponentBeans::GroupComponentBeans(GroupLayout layout, QString type, QString paramTitle, QJsonObject &param)
    : ComponentBean(type,paramTitle,param) {
    m_layout = layout;
}


GroupComponentBeans* GroupComponentBeans::create(QString id, QString title, QString layout){
    GroupLayout l = DefaultLayout;
    if (layout.compare("Stack", Qt::CaseInsensitive) == 0) {
        l = StackLayout;
    } else if (layout.compare("Column", Qt::CaseInsensitive) == 0) {
        l = ColumnLayout;
    } else if (layout.compare("Single", Qt::CaseInsensitive) == 0) {
        l = SingleLayout;
    } else if(layout.compare("ColumnNoScroll", Qt::CaseInsensitive) == 0){
        l = ColumnLayoutNoScroll;
    }
    auto type = s_groupLayoutType[l];
    QJsonObject param;
    return new GroupComponentBeans(l, type,title,param);
}

GroupComponentBeans::~GroupComponentBeans() {
    qDeleteAll(m_components);
    m_components.clear();
}

void GroupComponentBeans::onUnbindQml(){
    for (ComponentBean *component : m_components) {
        component->unBindQml();
    }
}

void GroupComponentBeans::appendComponent(ComponentBean *component) {
    m_components.append(component);
}


ComponentBean *GroupComponentBeans::get(int index) {
    return m_components[index];
}

int GroupComponentBeans::count() {
    return m_components.count();
}

void GroupComponentBeans::onClipParamsChanged() {
    if (component) {
        component->onClipParamsChanged();
    }
    for (ComponentBean *component : m_components) {
        component->onClipParamsChanged();
    }
}


int GroupComponentBeans::estimateHeight(){
    int height = (m_components.size() - 1) * 10;
    for (ComponentBean *component : m_components) {
        height+=component->estimateHeight();
    }
    return height;
}

QString GroupComponentBeans::toString(){
    QString str = "ComponentBean:"+uiType+" QML:"+qml +" Children:[";
    for (ComponentBean *component : m_components) {
        str= str + component->toString()+",";
    }
    return str+"]";
}

void GroupComponentBeans::changeGroupUIType(){
    if(m_layout == DefaultLayout){
        uiType = m_components.count() == 1?"SingleLayout":"ColumnLayout";
        qml = m_componentDict[uiType];
    }
    qDebug()<<"changeGroupUIType "<<this<<" changeGroupUIType :"<<" Size:"<<m_components.size() << " count:"<<m_components.count();
}

