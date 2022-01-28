#include "SliderComponent.h"

#include <QJsonArray>
#include <QDebug>
#include "base/utils/AppConstance.h"

SliderComponent::SliderComponent(QObject *parent) : BaseComponent(parent) {
}

void SliderComponent::onBindQml(QJsonObject &paramInfo) {
    m_sliderText = paramInfo["paramTitle"].toString();
    if (paramInfo["EnableKeyFrame"].isBool()) {
        m_keyFrameEnable = paramInfo["EnableKeyFrame"].toBool(true);
    }

    QJsonArray paramInfoSettings = paramInfo["paramSettingInfo"].toArray();
    if (paramInfoSettings.count() == 0) {
        qDebug()<<"paramInfoSettings is Empty???"<<paramInfo;
        return;
    }

    for (int i = 0; i < paramInfoSettings.size(); ++i) {
        auto param = paramInfoSettings[i].toObject();
        m_sliderParams.append(ParamSettingInfo(param));
    }

    m_displayParam = ParamSettingInfo(paramInfo, m_sliderParams[0]);
    qDebug() << "onBindQml Param:" << paramInfo.toVariantMap();
}

QString SliderComponent::sliderDisplayText(double value) {
    return m_displayParam.displayText(value);
}

QList<QString> SliderComponent::sliderKeys() {
    QList<QString> keys;
    for (auto param : m_sliderParams) {
        keys.append(param.paramKey());
    }
    return keys;
}

void SliderComponent::enableSliderKeyFrames() {
    QStringList keys;

    QList<double> values;
    double displayValue = sliderValue();
    for (auto param : m_sliderParams) {
        keys.append(param.paramKey());
        auto curSliderVal = m_displayParam.transformTargetParamValue(displayValue, param);
        values.append(curSliderVal);
    }
    enableKeyFrames(keys, values);
}

void SliderComponent::removeSliderKeyFrames() {
    removeKeyFrames(sliderKeys());
}

bool SliderComponent::isCurSliderKeyFrame() {
    return isCurKeyFrame(sliderKeys());
}

QString SliderComponent::sliderDisplayTextPrefix() {
    return m_displayParam.displayTextPrefix();
}

QString SliderComponent::sliderDisplayTextSuffix() {
    return m_displayParam.displayTextSuffix();
}

double SliderComponent::sliderValueFromText(QString text) {
    return m_displayParam.textToValue(text);
}

double SliderComponent::stepSize() {
    return m_displayParam.stepSize();
}

QString SliderComponent::sliderKey() {
    return m_sliderParams[0].paramKey();
}

int SliderComponent::decimals() {
    return m_displayParam.decimals;
}

void SliderComponent::setSliderValue(double value) {
    QMap<QString, SkyVariant> paramMap;
    for (auto param : m_sliderParams) {
        auto curSliderVal = m_displayParam.transformTargetParamValue(value, param);
        paramMap.insert(param.paramKey(), curSliderVal);
    }

    updateOfParamValuesBySilent(paramMap);
}

void SliderComponent::setSliderValue(double preValue, double curVal) {
    QMap<QString, SkyVariant> paramMap;
    QMap<QString, SkyVariant> preValueMap;
    for (auto param : m_sliderParams) {
        auto curSliderVal = m_displayParam.transformTargetParamValue(curVal, param);
        paramMap.insert(param.paramKey(), curSliderVal);
        auto preSliderVal = m_displayParam.transformTargetParamValue(preValue, param);
        preValueMap.insert(param.paramKey(), preSliderVal);
    }

    updateOfParamValuesByUser(paramMap, preValueMap);
}

double SliderComponent::sliderMaxValue() {
    return m_displayParam.maxValue;
}

double SliderComponent::sliderMinValue() {
    return m_displayParam.minValue;
}

void SliderComponent::onOfParamsChanged(QString key, SkyVariant v) {
    if (m_sliderParams.size() > 0 && key == m_sliderParams[0].paramKey()) {
        emit sliderValueChanged(sliderValue());
    }
}

void SliderComponent::onUnBind() {
}

QString SliderComponent::sliderText() const {
    return translate(m_sliderText);
}

bool SliderComponent::isKeyFrameEnable() {
    return m_keyFrameEnable;
}

QString SliderComponent::sliderKeyFrameTweenType(){
    return keyFrameTweenType(sliderKey());
}

void SliderComponent::updateSliderKeyFrameTweenType( QString tween){
    updateKeyFrameTweenTypes(sliderKeys(),tween);
}

double SliderComponent::sliderValue() {
    if (m_sliderParams.isEmpty()) {
        return m_displayParam.defVal().toDouble();
    }
    auto sliderParam = m_sliderParams[0];
    auto key = sliderParam.paramKey();
    auto defVal = m_displayParam.transformTargetParamValue(m_displayParam.defVal().toDouble(), sliderParam);
    auto sliderVal = getOfParamVariantValue(key, defVal).toDouble();
    auto displayValue = sliderParam.transformTargetParamValue(sliderVal, m_displayParam);
    return displayValue;
}
