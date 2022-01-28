#include "ImageComponent.h"

#include <QJsonArray>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <project/SEProject.h>
#include <timeline/command/PropertyEditCommand.h>

#include <timeline/controller/timelinecontroller.h>

ImageComponent::ImageComponent(QObject *parent) : BaseComponent(parent) {
}

void ImageComponent::onBindQml(QJsonObject &paramInfo) {
    m_labelImage = paramInfo["paramTitle"].toString();
    QString defValue =  paramInfo["defValue"].toString();
    QJsonValue settingInfo = paramInfo["paramSettingInfo"];
    loadEffectParamInfo(settingInfo);
    if(m_isUpdateClipResource){
        m_imagePath = curClip()->resourceBean().entryPath();
    }else{
        QString ofPath = curEffect()->skyResource().entryPath();
        QDir dir = QFileInfo(ofPath).absoluteDir();
        m_imagePath = QFileInfo(dir, defValue).absoluteFilePath();
    }

	
    updateCurImagePath();

}

void ImageComponent::loadEffectParamInfo(QJsonValue& setting){
    QJsonObject settingInfo;
    if(setting.isArray()){
        auto settingArrays = setting.toArray();

        for(int i=0;i<settingArrays.size();++i){
            auto setting = settingArrays.at(i).toObject();
            auto paramType = setting["paramType"].toString("");
            if(paramType.compare("binary",Qt::CaseInsensitive) == 0 || paramType.compare("randomNum",Qt::CaseInsensitive) == 0){
                continue;
            }else{
                settingInfo = setting;
            }
        }
    }else{
        auto paramType = setting["paramType"].toString("");
        if(paramType.compare("binary",Qt::CaseInsensitive) == 0 || paramType.compare("randomNum",Qt::CaseInsensitive) == 0){
            qWarning()<<" paramType Error?"<<paramType;
            return;
        }else{
            settingInfo = setting.toObject();
        }
    }
    if(settingInfo.keys().length() == 0){
        return;
    }
    m_param = QString("%1:%2").arg(settingInfo["filterIndex"].toInt()).arg(settingInfo["paramName"].toString());

    m_isUpdateClipResource = m_param.compare("-1:updateResource") == 0 && settingInfo["paramType"].toString().compare("SkyResource") == 0;

}

void ImageComponent::onUnBind() {
}

QString ImageComponent::labelText() const {
    return m_labelImage;
}

QString ImageComponent::imagePath() const {
    return QUrl::fromLocalFile(m_imagePath).toString();
}

void ImageComponent::updateImage(QUrl path) {
    QString filePath = path.toLocalFile();

    if(m_isUpdateClipResource){
        TimelineController *timeline = SEProject::current()->dom()->timelineController();
        QModelIndex index = timeline->curSelectedIndex();
        auto command = new ClipUpdateResourceCommand(timeline->timelineModel(), index,filePath, -1, -1, true);
        timeline->addUndoCommand(command);
    }else{
        SkyVariant prePath = SkyVariant::makePath(m_imagePath.toUtf8().data());
        SkyVariant curPath = SkyVariant::makePath(filePath.toUtf8().data());

        updateOfParamValueByUser(m_param,curPath,prePath);
    }
}

void ImageComponent::onOfParamsChanged(QString key, SkyVariant v) {
    if (key == m_param) {
        updateCurImagePath();
    }
}


void ImageComponent::updateCurImagePath(){
    if(curEffect() == nullptr){
        return;
    }
    if(m_isUpdateClipResource){
        m_imagePath = curClip()->resourceBean().entryPath();
    }else{
        QString ofPath = curEffect()->skyResource().entryPath();
        QDir dir = QFileInfo(ofPath).absoluteDir();
        QString paramString = QString::fromUtf8(getOfParamValue(m_param).toString());
        if (paramString.length() > 0) {
            auto filePath = QString::fromUtf8(getOfParamValue(m_param).toString());
            if(QFileInfo::exists(filePath)){
                m_imagePath = filePath;
            } else {
                m_imagePath = QFileInfo(dir, filePath).absoluteFilePath();
            }
        }
    }
    emit imagePathUpdate(m_imagePath);

}
