#include "ParticleValueComponent.h"

#include <QJsonArray>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <project/SEProject.h>
#include <timeline/command/PropertyEditCommand.h>
#include <base/utils/fileutils.h>

#include <timeline/controller/timelinecontroller.h>

ParticleValueComponent::ParticleValueComponent(QObject *parent) : BaseComponent(parent) {
}

void ParticleValueComponent::onBindQml(QJsonObject &paramInfo) {
    m_label = paramInfo["paramTitle"].toString();
    QString valueType =  paramInfo["valueType"].toString();
    if(valueType.compare("file",Qt::CaseInsensitive) != 0){
        return;
    }

    QJsonValue settingInfo = paramInfo["paramSettingInfo"];
    if(settingInfo.isArray()){
        auto setting = settingInfo.toArray()[0].toObject();
        m_defParam = setting;
    }else{
        auto setting = settingInfo.toObject();
        m_defParam = setting;
    }
    loadPartileDatas();
}


void ParticleValueComponent::loadPartileDatas(){
    QString ofPath = curEffect()->skyResource().entryPath();
    QDir dir = QFileInfo(ofPath).absoluteDir();
    auto cfgPathVal = curEffect()->getOfParamValue(m_defParam.paramKey());
    QString cfgPath = cfgPathVal.isNull()?m_defParam.defValue:QString::fromUtf8(cfgPathVal.toString());
    if(QFileInfo::exists(cfgPath)){

    }else if(QFileInfo::exists(dir.absolutePath()+"/config/"+cfgPath)){
        cfgPath = dir.absolutePath()+"/config/"+cfgPath;
    }else{
        return;
    }

    m_datas.clear();
    QString context = FileUtils::readJson(cfgPath);
    auto jsonObject = QJsonDocument::fromJson(context.toUtf8()).object();
    auto tween = jsonObject.value("tween").toArray();
    for(auto i = 0;i<tween.size();++i){
        m_datas.append(tween[i].toInt());
    }

    emit partileDataChanged();
}

void ParticleValueComponent::onUnBind() {
}


void ParticleValueComponent::onOfParamsChanged(QString key, SkyVariant v) {
    if(key == m_defParam.paramKey()){
        loadPartileDatas();
    }
}


void ParticleValueComponent::setPartileDatas(QVariantList list){
    if(list.isEmpty()){
        return;
    }

    QJsonArray tween;
    for(int i=0;i<list.size();++i){
        tween.append(list.at(i).toInt());
    }

    QJsonObject root;
    root.insert("tween",tween);

    auto prefix = m_defParam.defValue.left(m_defParam.defValue.indexOf('.'));
    QString ofPath = curEffect()->skyResource().entryPath();
    QDir dir = QFileInfo(ofPath).absoluteDir();
    QString configPath = QString("%1_%2.json").arg(prefix).arg(QTime::currentTime().msecsSinceStartOfDay());
    QFile file(dir.absolutePath()+"/config/"+configPath);
    if(file.open(QFile::WriteOnly)){
        QJsonDocument doc(root);
        QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
        file.write(jsonData);
        file.flush();
        file.close();


        updateOfParamValueByUser(m_defParam.paramKey(),configPath.toUtf8().data());

        qDebug()<<"setPartileDatas "<<m_defParam.paramKey()<<" -> "<<configPath;
    }

}


