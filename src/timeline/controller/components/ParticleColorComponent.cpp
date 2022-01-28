#include "ParticleColorComponent.h"

#include <QJsonArray>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <project/SEProject.h>
#include <timeline/command/PropertyEditCommand.h>
#include <base/utils/fileutils.h>

#include <timeline/controller/timelinecontroller.h>

ParticleColorComponent::ParticleColorComponent(QObject *parent) : BaseComponent(parent) {
}

void ParticleColorComponent::onBindQml(QJsonObject &paramInfo) {
    m_label = paramInfo["paramTitle"].toString();
    QString valueType =  paramInfo["valueType"].toString();
    QString defValue =  paramInfo["defValue"].toString();
    QString ofPath = curEffect()->skyResource().entryPath();
    QDir dir = QFileInfo(ofPath).absoluteDir();

    QJsonValue settingInfo = paramInfo["paramSettingInfo"];
    if(settingInfo.isArray()){
        auto setting = settingInfo.toArray()[0].toObject();
        m_defParam = setting;
    }else{
        auto setting = settingInfo.toObject();
        m_defParam = setting;
    }

    if(valueType.compare("file",Qt::CaseInsensitive) != 0){
        return;
    }

    loadPartileColorData();

}



void ParticleColorComponent::onUnBind() {
}


void ParticleColorComponent::onOfParamsChanged(QString key, SkyVariant v) {
    if(key == m_defParam.paramKey()){
        loadPartileColorData();
    }
}

void ParticleColorComponent::loadPartileColorData(){
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

    m_colors.clear();

    QString context = FileUtils::readJson(cfgPath);
    auto jsonObject = QJsonDocument::fromJson(context.toUtf8()).object();
    auto points = jsonObject.value("points").toArray();
    for(auto i = 0;i<points.size();++i){
        auto point = points[i].toObject();
        m_colors.append(point.toVariantMap());
    }

    emit partileColorDataChanged();
}


void ParticleColorComponent::setPartileColors(QVariantList list){
    if(list.isEmpty()){
        return;
    }
    QJsonObject root;
    QJsonArray points;
    for(int i=0;i<list.size();++i){
        QVariantMap map = list.at(i).toMap();
        QJsonObject point;
        point.insert("position",map.value("position").toFloat());
        point.insert("color",map.value("color").toString());
        points.append(point);
    }

    int SIZE = 300;
    int curIndex = -1;
    QJsonArray tween;
    for(int i=0;i<SIZE;++i){
        auto pos = i * 1.0 / SIZE;
        while(curIndex < points.size() - 1){
            if(points.at(curIndex + 1).toObject().value("position").toDouble() >= pos){
                break;
            }else{
                curIndex++;
            }
        }

        QJsonObject pre;
        QJsonObject next;
        QColor targetColor;
        if(curIndex >= 0){
            pre = points.at(curIndex).toObject();
        }else{
        }
        if(curIndex < points.size() - 1){
            next = points.at(curIndex + 1).toObject();
        }else{

        }
        if(pre.isEmpty()){
            targetColor = QColor(next.value("color").toString());
        }else if(next.isEmpty()){
            targetColor = QColor(pre.value("color").toString());
        }else {
            QColor nextColor = QColor(next.value("color").toString());
            QColor preColor = QColor(pre.value("color").toString());

            auto percent = (pos - pre.value("position").toDouble())/(next.value("position").toDouble() - pre.value("position").toDouble());
            targetColor = QColor(
                        percent * (nextColor.red() - preColor.red()) + preColor.red(),
                        percent * (nextColor.green() - preColor.green()) + preColor.green(),
                        percent * (nextColor.blue() - preColor.blue()) + preColor.blue()
                        );

        }
        QJsonArray colorArray;
        colorArray.append(targetColor.red());
        colorArray.append(targetColor.green());
        colorArray.append(targetColor.blue());

        tween.append(colorArray);
    }

    root.insert("points",points);
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

        qDebug()<<"setPartileColors "<<m_defParam.paramKey()<<" -> "<<configPath;
    }

}


