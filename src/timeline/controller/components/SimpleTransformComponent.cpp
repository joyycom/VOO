#include "SimpleTransformComponent.h"

#include <base/utils/JsonUtils.h>
#include <project/SEProject.h>

#include <QHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMap>
#include "base/utils/AppConstance.h"

SimpleTransformComponent::SimpleTransformComponent(QObject *parent) : BaseComponent(parent) {
    PreviewController *previewController = SEProject::current()->dom()->previewController();
    QObject::connect(previewController,
                     &PreviewController::videoProgressChanged,
                     this,
                     &SimpleTransformComponent::onPreviewProgressChange);
}

void SimpleTransformComponent::onBindQml(QJsonObject &doc) {
    initParamInfo(doc);

    qDebug() << "SimpleTransformComponent::bindQml m_ready:" << m_ready;

    m_curTransX = getOfParamVariantValue(m_paramTransX.paramKey(),m_curTransX).toDouble();
    m_curTransY = getOfParamVariantValue(m_paramTransY.paramKey(),m_curTransY).toDouble();

    SkyTimeline *timeline = SEProject::current()->dom()->timeline();
    SkyVideoParams params = timeline->getVideoParams();

    if(m_paramScaleX.isValid() && m_paramScaleY.isValid()){
        m_curScaleX = getOfParamVariantValue(m_paramScaleX.paramKey(),m_curScaleX).toDouble();
        m_curScaleY = getOfParamVariantValue(m_paramScaleY.paramKey(),m_curScaleY).toDouble();
    }else{
        float size = fmin(0.2 * params.width,0.2 * params.height);
        m_curScaleX = size;
        m_curScaleY = size;
    }

    emit sourceChange(m_curWidth, m_curHeight, params.width, params.height);

    emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY);


    double curPts = SEProject::current()->dom()->previewController()->videoProgress();
    onPreviewProgressChange(curPts);
}

void SimpleTransformComponent::onOfMessage(QString msg) {

}

void SimpleTransformComponent::onPreviewProgressChange(double pts) {
    BaseClip *clip = curClip();
    if (clip) {
        double effectStart = clip->startTime();
        double effectEnd = clip->endTime();
        double previewDuration = SEProject::current()->dom()->previewController()->during() * pts;
        emit displayTransformView(true, previewDuration >= effectStart && previewDuration <= effectEnd);
    } else {
        emit displayTransformView(false, false);
    }
}

void SimpleTransformComponent::onUnBind() {
    m_ready = false;
    m_curTransX = 0;
    m_curTransY = 0;
    m_curHeight = 1;
    m_curWidth = 1;
    m_curScaleX = 1.0;
    m_curScaleY = 1.0;
}

void SimpleTransformComponent::changeTranslate(float transx, float transy) {
    m_curTransX = fmin(fmax(m_paramTransX.minValue,transx),m_paramTransX.maxValue);
    m_curTransY = fmin(fmax(m_paramTransY.minValue,transy),m_paramTransY.maxValue);
    QMap<QString, SkyVariant> paramMap;
    paramMap.insert(m_paramTransX.paramKey(), transx);
    paramMap.insert(m_paramTransY.paramKey(), transy);
    updateOfParamValuesBySilent(paramMap);

    emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY);
}

void SimpleTransformComponent::changeTranslateX(bool left) {
    if (left) {
        if (m_curTransX - 1 < m_paramTransX.minValue) {
            return;
        }
        m_curTransX--;
    } else {
        if (m_curTransX + 1 > m_paramTransX.maxValue) {
            return;
        }
        m_curTransX++;
    }
    QMap<QString, SkyVariant> paramMap;
    paramMap.insert(m_paramTransX.paramKey(), m_curTransX);
    updateOfParamValuesBySilent(paramMap);
    emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY);
}

void SimpleTransformComponent::changeTranslateY(bool top) {
    if (top) {
        if (m_curTransY - 1 < m_paramTransY.minValue) {
            return;
        }
        m_curTransY--;
    } else {
        if (m_curTransY + 1 > m_paramTransY.maxValue) {
            return;
        }
        m_curTransY++;
    }
    QMap<QString, SkyVariant> paramMap;
    paramMap.insert(m_paramTransY.paramKey(), m_curTransY);
    updateOfParamValuesBySilent(paramMap);
    emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY);
}

void SimpleTransformComponent::changeScaleOnTouchRelease(float startScaleX, float scaleX,float startScaleY, float scaleY) {
    if(!m_paramScaleX.isValid() || !m_paramScaleY.isValid()){
        return;
    }

    m_curScaleX = fmin(fmax(m_paramScaleX.minValue,scaleX),m_paramScaleX.maxValue);
    m_curScaleY = fmin(fmax(m_paramScaleY.minValue,scaleY),m_paramScaleY.maxValue);

    QMap<QString, SkyVariant> paramMap;
    paramMap.insert(m_paramScaleX.paramKey(), m_curScaleX);
    paramMap.insert(m_paramScaleY.paramKey(), m_curScaleY);

    QMap<QString, SkyVariant> preMap;
    preMap.insert(m_paramScaleX.paramKey(), startScaleX);
    preMap.insert(m_paramScaleY.paramKey(), startScaleY);
    updateOfParamValuesByUser(paramMap,preMap);
    emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY);
}


void SimpleTransformComponent::changeScale(float scaleX, float scaleY) {
    if(!m_paramScaleX.isValid() || !m_paramScaleY.isValid()){
        return;
    }
    if (m_curScaleX == scaleX && m_curScaleY == scaleY) {
        return;
    }
    qDebug()<<"changeScale X:"<<m_curScaleX<<" -> "<<scaleX<<" Y:"<<m_curScaleY<<" -> "<<scaleY;

    m_curScaleX = fmin(fmax(m_paramScaleX.minValue,scaleX),m_paramScaleX.maxValue);
    m_curScaleY = fmin(fmax(m_paramScaleY.minValue,scaleY),m_paramScaleY.maxValue);

    QMap<QString, SkyVariant> paramMap;
    paramMap.insert(m_paramScaleX.paramKey(), m_curScaleX);
    paramMap.insert(m_paramScaleY.paramKey(), m_curScaleY);

    updateOfParamValuesBySilent(paramMap);
    emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY);
}

void SimpleTransformComponent::changeTranslateOnTouchRelease(float start_transx,
                                                             float start_transy,
                                                             float transx,
                                                             float transy) {
    transx = fmin(fmax(transx, m_paramTransX.minValue),m_paramTransX.maxValue);
    transy = fmin(fmax(transy, m_paramTransY.minValue),m_paramTransY.maxValue);

    m_curTransX = transx;
    m_curTransY = transy;
    QMap<QString, SkyVariant> paramMap;
    paramMap.insert(m_paramTransX.paramKey(), transx);
    paramMap.insert(m_paramTransY.paramKey(), transy);

    QMap<QString, SkyVariant> start_paramMap;
    start_paramMap.insert(m_paramTransX.paramKey(), start_transx);
    start_paramMap.insert(m_paramTransY.paramKey(), start_transy);
    updateOfParamValuesByUser(paramMap, start_paramMap);

    emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY);
}

void SimpleTransformComponent::onOfParamsChanged(QString key, SkyVariant value) {
    if (key == m_paramTransX.paramKey()) {
        if (abs(value.toDouble() - m_curTransX) > FLOAT_DEVIATION) {
            m_curTransX = value.toDouble();
            emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY);
        }
    } else if (key == m_paramTransY.paramKey()) {
        if (abs(value.toDouble() - m_curTransY) > FLOAT_DEVIATION) {
            m_curTransY = value.toDouble();
            emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY);
        }
    } else if (key == m_paramScaleX.paramKey() && m_paramScaleX.isValid()) {
        if (abs(value.toDouble() - m_curWidth) > FLOAT_DEVIATION) {
            m_curScaleX = value.toDouble();
            emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY);
        }
    } else if (key == m_paramScaleY.paramKey() && m_paramScaleY.isValid()) {
        if (abs(value.toDouble() - m_curHeight) > FLOAT_DEVIATION) {
            m_curScaleY = value.toDouble();
            emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY);
        }

    }
}

void SimpleTransformComponent::updateUI() {
    SkyTimeline *timeline = SEProject::current()->dom()->timeline();
    if (timeline && m_curClip) {
        SkyVideoParams params = timeline->getVideoParams();

            emit sourceChange(
                              m_curWidth,
                              m_curHeight,
                              params.width,
                              params.height);
    }
}

void SimpleTransformComponent::initParamInfo(QJsonObject &doc) {
    QJsonArray paramSettingInfo = doc["paramSettingInfo"].toArray();

    if (paramSettingInfo.size()  < 2) {
        qDebug() << "SimpleTransformComponent onBindQml fail : paramSettingInfo is null";
        return;
    }
    QJsonObject paramInfo = paramSettingInfo[0].toObject();
    m_paramTransX = ParamSettingInfo(paramInfo);
    paramInfo = paramSettingInfo[1].toObject();
    m_paramTransY = ParamSettingInfo(paramInfo);
    if(paramSettingInfo.size() >= 4){
        paramInfo = paramSettingInfo[2].toObject();
        m_paramScaleX = ParamSettingInfo(paramInfo);
        paramInfo = paramSettingInfo[3].toObject();
        m_paramScaleY = ParamSettingInfo(paramInfo);
    }else if(paramSettingInfo.size() == 3){
        paramInfo = paramSettingInfo[2].toObject();
        m_paramScaleX = ParamSettingInfo(paramInfo);
        m_paramScaleY = ParamSettingInfo(paramInfo);
    } else if(paramSettingInfo.size() == 2){
        m_paramScaleX = ParamSettingInfo();
        m_paramScaleY = ParamSettingInfo();
    }

    m_init = true;
    m_ready =  true;
}

void SimpleTransformComponent::bindClip(BaseClip *clip) {
    m_curClip = clip;
    m_curEffect = clip->skyEffectWrapper();
}
