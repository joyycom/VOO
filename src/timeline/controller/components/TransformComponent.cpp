#include "TransformComponent.h"

#include <base/utils/JsonUtils.h>
#include <project/SEProject.h>

#include <QHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMap>
#include "base/utils/AppConstance.h"

TransformComponent::TransformComponent(QObject *parent) : BaseComponent(parent) {
    PreviewController *previewController = SEProject::current()->dom()->previewController();
    QObject::connect(previewController,
                     &PreviewController::videoProgressChanged,
                     this,
                     &TransformComponent::onPreviewProgressChange);
}

void TransformComponent::onBindQml(QJsonObject &doc) {
    initParamInfo(doc);

    qDebug() << "TransformComponent::bindQml m_ready:" << m_ready;
    m_ready =  true;

    auto isOverlayRes = m_curEffect->skyResource().resouceType() == OverlayResource;

    if (!isOverlayRes) {
        sendOfMessage("{\"id\":99}");
    }

    updateSourceChanged();
    updateUIChanged();

    double curPts = SEProject::current()->dom()->previewController()->videoProgress();
    onPreviewProgressChange(curPts);
}

void TransformComponent::onOfMessage(QString msg) {
    QJsonObject doc = QJsonDocument::fromJson(msg.toUtf8()).object();
    int k = doc["id"].toInt();
    qDebug() << " onOfMessage " << doc.toVariantMap();
    if (k == 200 && doc.contains("size")) {
        BaseClip *clip = curClip();
        if (clip == nullptr) {
            return;
        }
        clip->ofWidth = doc["size"].toArray().at(0).toDouble();
        clip->ofHeight = doc["size"].toArray().at(1).toDouble();

        updateSourceChanged();
        updateUIChanged();

        double curPts = SEProject::current()->dom()->previewController()->videoProgress();
        onPreviewProgressChange(curPts);
    }
}

void TransformComponent::onPreviewProgressChange(double pts) {
    BaseClip *clip = curClip();
    if (clip) {
        double effectStart = clip->startTime();
        double effectEnd = clip->endTime();
        double previewDuration = SEProject::current()->dom()->previewController()->during() * pts;
        emit displayTransformView(m_ready, previewDuration >= effectStart && previewDuration <= effectEnd);
    } else {
        emit displayTransformView(false, false);
    }
}

void TransformComponent::onUnBind() {
    m_ready = false;
    m_aspect = 1.0;
    m_curScaleX = 1.0;
    m_curScaleY = 1.0;
    m_curTransX = 0;
    m_curTransY = 0;
    m_curRotate = 0;
}

void TransformComponent::changeScale(float scaleX, float scaleY) {
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
    emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY, m_curRotate);
}


void TransformComponent::changeRotate(float rotate) {
    if (m_curRotate == rotate) {
        return;
    }
    m_curRotate = rotate;
    updateOfParamValueBySilent(m_paramRotate.paramKey(), m_curRotate);
    emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY, m_curRotate);
}

void TransformComponent::changeTranslate(float transx, float transy) {
    m_curTransX = fmin(fmax(m_paramTransX.minValue,transx),m_paramTransX.maxValue);
    m_curTransY = fmin(fmax(m_paramTransY.minValue,transy),m_paramTransY.maxValue);
    QMap<QString, SkyVariant> paramMap;
    paramMap.insert(m_paramTransX.paramKey(), transx);
    paramMap.insert(m_paramTransY.paramKey(), transy);
    updateOfParamValuesBySilent(paramMap);

    emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY, m_curRotate);
}

void TransformComponent::changeTranslateX(bool left) {
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
    emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY, m_curRotate);
}

void TransformComponent::changeTranslateY(bool top) {
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
    emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY, m_curRotate);
}

void TransformComponent::changeScaleOnTouchRelease(float startScaleX, float scaleX,float startScaleY, float scaleY) {
    m_curScaleX = fmin(fmax(m_paramScaleX.minValue,scaleX),m_paramScaleX.maxValue);
    m_curScaleY = fmin(fmax(m_paramScaleY.minValue,scaleY),m_paramScaleY.maxValue);

    QMap<QString, SkyVariant> paramMap;
    paramMap.insert(m_paramScaleX.paramKey(), m_curScaleX);
    paramMap.insert(m_paramScaleY.paramKey(), m_curScaleY);

    QMap<QString, SkyVariant> preMap;
    preMap.insert(m_paramScaleX.paramKey(), startScaleX);
    preMap.insert(m_paramScaleY.paramKey(), startScaleY);
    updateOfParamValuesByUser(paramMap,preMap);
    emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY, m_curRotate);
}


void TransformComponent::changeRotateOnTouchRelease(float startRotate, float rotate) {
    qDebug() << "changeScaleOnTouchRelease " << startRotate << " ->" << rotate << " m_curRotate:" << m_curRotate;
    m_curRotate = rotate;
    updateOfParamValueByUser(m_paramRotate.paramKey(), rotate, startRotate);
}

void TransformComponent::changeTranslateOnTouchRelease(float start_transx,
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

    emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY, m_curRotate);
}

void TransformComponent::onOfParamsChanged(QString key, SkyVariant value) {
    if (key == m_paramTransX.paramKey()) {
        if (abs(value.toDouble() - m_curTransX) > FLOAT_DEVIATION) {
            m_curTransX = value.toDouble();
            emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY, m_curRotate);
        }
    } else if (key == m_paramTransY.paramKey()) {
        if (abs(value.toDouble() - m_curTransY) > FLOAT_DEVIATION) {
            m_curTransY = value.toDouble();
            emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY, m_curRotate);
        }
    } else if (key == m_paramScaleX.paramKey()) {
        if (abs(value.toDouble() - m_curScaleX) > FLOAT_DEVIATION) {
            m_curScaleX = value.toDouble();
            emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY, m_curRotate);
        }
    } else if (key == m_paramScaleY.paramKey()) {
        if (abs(value.toDouble() - m_curScaleY) > FLOAT_DEVIATION) {
            m_curScaleY = value.toDouble();
            emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY, m_curRotate);
        }

    } else if (key == m_paramRotate.paramKey()) {
        if (abs(value.toDouble() - m_curRotate) > FLOAT_DEVIATION) {
            m_curRotate = value.toDouble();
            emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY, m_curRotate);
        }
    }
}

void TransformComponent::updateSourceChanged() {
    SkyTimeline *timeline = SEProject::current()->dom()->timeline();
    if (timeline && m_curClip) {
        double width = 100,height = 100;
        auto isOverlayRes = m_curEffect->skyResource().resouceType() == OverlayResource;

        SkyVideoParams params = timeline->getVideoParams();
        auto scale = m_curEffect->getOfParamValue("0:Scale",1.0).toDouble();
        if(isOverlayRes){
            auto videoStream = m_curClip->resourceBean().skyResouce().firstVideoStream();
            width = videoStream.width * scale;
            height = videoStream.height * scale;
        }else if(m_curClip->ofWidth > 0 && m_curClip->ofHeight > 0){
            width = (double)m_curClip->ofWidth * scale;
            height = (double)m_curClip->ofHeight * scale;
        }else if(m_ratio > 0 && m_aspect > 0){
            width = params.width * m_ratio;
            height = width / m_aspect;
        }
        emit sourceChange(!isOverlayRes,
                              centerPointX,
                              centerPointY,
                              (double)width / params.width,
                              (double)height / params.height,
                              params.width,
                              params.height);
    }
}

void TransformComponent::updateUIChanged() {
    m_curTransX = getOfParamVariantValue(m_paramTransX.paramKey(),0).toDouble();
    m_curTransY = getOfParamVariantValue(m_paramTransY.paramKey(),0).toDouble();
    m_curScaleX = getOfParamVariantValue(m_paramScaleX.paramKey(),1.0).toDouble();
    m_curScaleY = getOfParamVariantValue(m_paramScaleY.paramKey(),1.0).toDouble();

    m_curRotate = getOfParamVariantValue(m_paramRotate.paramKey(),0).toDouble();
    qDebug() << "updateUIChanged Scale:" << m_curScaleX<<" ScaleY:"<<m_curScaleY;
    emit uiChange(m_curTransX, m_curTransY, m_curScaleX, m_curScaleY, m_curRotate);
}

void TransformComponent::initParamInfo(QJsonObject &doc) {
    QJsonArray paramList = doc["paramList"].toArray();
    QJsonArray paramSettingInfo;
    for (int j = 0; j < paramList.size(); j++) {
        if (paramList[j].toObject()["uiType"].toString() == "Transform") {
            paramSettingInfo = paramList[j].toObject()["paramSettingInfo"].toArray();
        }
    }
    if (paramSettingInfo.isEmpty()) {
        qDebug() << "TransformComponent onBindQml fail : paramSettingInfo is null";
        return;
    }

    for (int j = 0; j < paramSettingInfo.count(); j++) {
        QJsonObject paramInfo = paramSettingInfo[j].toObject();
        ParamSettingInfo settingInfo(paramInfo);
        if (paramInfo["paramName"].toString() == "TransX") {
            m_paramTransX = settingInfo;
        } else if (paramInfo["paramName"].toString() == "TransY") {
            m_paramTransY = settingInfo;
        } else if (paramInfo["paramName"].toString() == "Rotate") {
            m_paramRotate = settingInfo;
        } else if (paramInfo["paramName"].toString() == "ScaleX") {
            m_paramScaleX = settingInfo;
        } else if (paramInfo["paramName"].toString() == "ScaleY") {
            m_paramScaleY = settingInfo;
        }
    }
    m_ratio = doc["width"].toDouble(-1);
    m_aspect = doc["aspect"].toDouble(-1);
    centerPointX = doc.value("centerPointX").toDouble(0.5);
    centerPointY = doc["centerPointY"].toDouble(0.5);
    m_init = true;
}

void TransformComponent::bindClip(BaseClip *clip) {
    m_curClip = clip;
    m_curEffect = clip->skyEffectWrapper();
}

TransformSelect TransformComponent::getSelectRect(QJsonObject &doc, int previewWidth, int previewHeight) {
    if (!m_init) {
        initParamInfo(doc);
    }
    QJsonArray paramList = doc["paramList"].toArray();
    for (int k = 0; k < paramList.count(); ++k) {
        QJsonObject paramInfo = paramList.at(k).toObject();
        auto uiType = paramInfo["uiType"].toString();
        if (uiType == "Transform") {
            QRect rectangleLayout;
            SkyTimeline *timeline = SEProject::current()->dom()->timeline();
            SkyVideoParams params = timeline->getVideoParams();
            auto videoStream = m_curClip->resourceBean().skyResouce().firstVideoStream();
            //设置预览宽高
            if (m_curClip->clipType() == EffectClipType) {
                if (m_curClip->resourceBean().resouceType() == TextResource) {
                    float aspect = (double)m_curClip->ofWidth / (double)m_curClip->ofHeight;
                    rectangleLayout.setWidth((double)(m_curClip->ofWidth * previewWidth) / params.width);
                    rectangleLayout.setHeight(rectangleLayout.width() / aspect);
                } else {
                    rectangleLayout.setWidth((double)previewWidth * m_ratio);
                    rectangleLayout.setHeight(rectangleLayout.width() / m_aspect);
                }
            } else {
                rectangleLayout.setWidth((double)(videoStream.width * previewWidth) / params.width);
                rectangleLayout.setHeight((double)(videoStream.height * previewHeight) / params.height);
            }
            //设置预览矩形的位移
            double xRatio = (double)previewWidth / params.width;
            double yRatio = (double)previewHeight / params.height;

            int curTransX = getOfParamIntValue(m_paramTransX.paramKey().toStdString().c_str(), 0, true);

            int curTransY = getOfParamIntValue(m_paramTransY.paramKey().toStdString().c_str(), 0, true);
            int transX = (previewWidth - rectangleLayout.width()) / 2 + curTransX * xRatio;
            int transY = (previewHeight - rectangleLayout.height()) / 2 + curTransY * yRatio;
            rectangleLayout.setLeft(rectangleLayout.left() + transX);
            rectangleLayout.setRight(rectangleLayout.right() + transX);
            rectangleLayout.setTop(rectangleLayout.top() + transY);
            rectangleLayout.setBottom(rectangleLayout.bottom() + transY);
            //设置预览矩形的缩放
            double scaleX = getOfParamValue(m_paramScaleX.paramKey().toStdString().c_str(), 1.0, true).toDouble();
            double scaleY = getOfParamValue(m_paramScaleY.paramKey().toStdString().c_str(), 1.0, true).toDouble();

            int scaleWidth = rectangleLayout.width() * scaleX;
            int scaleHeight = rectangleLayout.height() * scaleY;
            rectangleLayout.setLeft(rectangleLayout.left() + (rectangleLayout.width() - scaleWidth) / 2);
            rectangleLayout.setRight(rectangleLayout.right() - (rectangleLayout.width() - scaleWidth) / 2);
            rectangleLayout.setTop(rectangleLayout.top() + (rectangleLayout.height() - scaleHeight) / 2);
            rectangleLayout.setBottom(rectangleLayout.bottom() - (rectangleLayout.height() - scaleHeight) / 2);
            //矩形的角度
            int curRotate = getOfParamIntValue(m_paramRotate.paramKey().toStdString().c_str(), 0);
            TransformSelect retValue;
            retValue.rect = &rectangleLayout;
            retValue.rotate = curRotate;
            return retValue;
        }
    }
    return TransformSelect();
}
