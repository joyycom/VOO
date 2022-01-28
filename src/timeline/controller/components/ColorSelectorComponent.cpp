#include "ColorSelectorComponent.h"

#include <QJsonArray>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

#include <base/utils/SkyVariantUtil.h>

ColorSelectorComponent::ColorSelectorComponent(QObject *parent) : BaseComponent(parent) {
}

void ColorSelectorComponent::onBindQml(QJsonObject &paramInfo) {
    m_label = paramInfo["paramTitle"].toString();
    QJsonArray paramInfoSettings = paramInfo["paramSettingInfo"].toArray();
    if (paramInfoSettings.count() == 0) {
        return;
    }

    ParamSettingInfo displayInfo(paramInfo);

    for (int i = 0; i < paramInfoSettings.size(); ++i) {
        auto param = paramInfoSettings[i].toObject();
        m_sliderParams.append(ParamSettingInfo(param,displayInfo));
    }

    auto param = m_sliderParams[0];
    QVariant defaultVariant = param.defVal();
    m_color = defaultVariant.type() == QVariant::Color ? defaultVariant.value<QColor>() : QColor(1.0, 1.0, 1.0, 1.0);

    auto defaultColorVariant = SkyVariantUtil::color2Variant(m_color);

    qDebug() << "Param Default Color:" << m_color << " OFColor:"
             << getOfParamValue(param.paramKey(), defaultColorVariant).transToJsonString(false, false).c_str();
    m_color = SkyVariantUtil::variant2Color(getOfParamValue(param.paramKey(), defaultColorVariant), m_color);
}

void ColorSelectorComponent::onUnBind() {
}

QString ColorSelectorComponent::labelText() {
    return translate(m_label);
}

QColor ColorSelectorComponent::selectColor() {
    return m_color;
}

void ColorSelectorComponent::updateColorByUser(QColor preColor, QColor color) {
    SkyVariant variant = SkyVariantUtil::color2Variant(color);
    SkyVariant preVariant = SkyVariantUtil::color2Variant(preColor);
    QMap<QString, SkyVariant> paramMap;
    QMap<QString, SkyVariant> preValueMap;
    for (auto param : m_sliderParams) {
        paramMap.insert(param.paramKey(), variant);
        preValueMap.insert(param.paramKey(), preVariant);
    }
    if (updateOfParamValuesByUser(paramMap, preValueMap)) {
        m_color = color;
    }

    emit colorUpdated(m_color);
}

void ColorSelectorComponent::updateColorSlient(QColor color) {
    auto curSliderVal = SkyVariantUtil::color2Variant(color);
    QMap<QString, SkyVariant> paramMap;
    for (auto param : m_sliderParams) {
        paramMap.insert(param.paramKey(), curSliderVal);
    }

    updateOfParamValuesBySilent(paramMap);
    emit colorUpdated(m_color);
}

void ColorSelectorComponent::onOfParamsChanged(QString key, SkyVariant v) {
    if (m_sliderParams.size() > 0 && key == m_sliderParams[0].paramKey()) {
        m_color = SkyVariantUtil::variant2Color(v, m_color);
        emit colorUpdated(m_color);
    }
}
