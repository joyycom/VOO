#include "ParticleFileParserComponent.h"

#include <QJsonArray>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QDomNode>

#include <project/SEProject.h>
#include <timeline/command/PropertyEditCommand.h>
#include <base/utils/fileutils.h>
#include <base/utils/SkyVariantUtil.h>

#include <timeline/controller/timelinecontroller.h>

ParticleFileParserComponent::ParticleFileParserComponent(QObject *parent) : BaseComponent(parent) {
}

void ParticleFileParserComponent::onBindQml(QJsonObject &paramInfo) {
    m_label = paramInfo["paramTitle"].toString();
    QString valueType =  paramInfo["valueType"].toString();
    if(valueType.compare("file",Qt::CaseInsensitive) != 0){
        return;
    }
}




void ParticleFileParserComponent::onUnBind() {
}



void ParticleFileParserComponent::setConfigFile(QString path){
    QUrl url(path);
    if(url.isLocalFile()){
        QString ofPath = curEffect()->skyResource().entryPath();
        QDir dir = QFileInfo(ofPath).absoluteDir();
        QString configDir = dir.absolutePath()+"/config/";
        auto datas = loadParitclePlist(url.toLocalFile(),configDir);
        QMap<QString, SkyVariant> paramMap;
        QMap<QString, SkyVariant> preParamMap;

        for(auto key:datas.keys()){
            paramMap.insert(key, SkyVariantUtil::toSkyVariant(datas[key]));
            preParamMap.insert(key,getOfParamValue(key));
        }
        if(paramMap.isEmpty()){
            return;
        }
        updateOfParamValuesByUser(paramMap,preParamMap);
    }
}


QMap<QString, QVariant> ParticleFileParserComponent::loadParitclePlist(const QString& plistFile, const QString& effectConfigPath){
    QMap<QString, QVariant> result;
    QFile file(plistFile);
    if (!file.open(QFile::ReadOnly))
        return result;

    QDomDocument doc;
    QString errMsg = "";
    int errorLine = -1;
    int errorColumn = -1;
    if (!doc.setContent(&file,&errMsg,&errorLine,&errorColumn)){
        qWarning()<<"XML setContent "<<errorLine<<":"<<errorColumn<<" Msg:"<<errMsg;
        file.close();
        return result;
    }
    file.close();

    QJsonObject orginObj;
    QDomElement root = doc.documentElement();
    qInfo("root.nodeName() = %s", root.nodeName().toStdString().c_str());
    QDomNode node = root.firstChild();
    while (!node.isNull())
    {
        if (node.isElement())
        {
            QDomElement e = node.toElement();
            qInfo("e.tagName() = %s, e.attribute(id) = %s, e.attribute(time)=%s ", e.tagName().toStdString().c_str(), e.attribute("id").toStdString().c_str(), e.attribute("time").toStdString().c_str()); //��ӡ��ֵ�ԣ�tagName��nodeName��һ������
            QDomNodeList list = e.childNodes();
            for (int i = 0; i < list.count(); i += 2)
            {
                QDomNode keyNode = list.at(i);
                if (keyNode.isElement()) {
                    if (keyNode.nodeName() != "key")
                        qInfo("plist parse error");
                }

                QDomNode valueNode = list.at(i + 1);
                if (valueNode.isElement())
                {
                    if (valueNode.nodeName() == "real")
                    {
                        orginObj[keyNode.toElement().text()] = valueNode.toElement().text().toFloat();
                    }
                    else if (valueNode.nodeName() == "integer")
                    {
                        orginObj[keyNode.toElement().text()] = valueNode.toElement().text().toInt();
                    }
                    else if (valueNode.nodeName() == "string")
                    {
                        orginObj[keyNode.toElement().text()] = valueNode.toElement().text();
                    }
                    else
                    {
                        qInfo("plist parse undefined value type error");
                    }
                }
            }
        }
        node = node.nextSibling();
    };

    result["0:particleMode"] = orginObj["emitterType"].toVariant();
    result["0:emitPosX"] = orginObj["sourcePositionx"].toVariant();
    result["0:emitPosY"] = orginObj["sourcePositiony"].toVariant();
    result["0:life"] = orginObj["particleLifespan"].toVariant();
    result["0:lifeVar"] = orginObj["particleLifespanVariance"].toVariant();
    result["0:emissionRate"] = orginObj["maxParticles"].toDouble() / orginObj["particleLifespan"].toDouble();
    result["0:angle"] = orginObj["angle"].toVariant();
    result["0:angleVar"] = orginObj["angleVariance"].toVariant();

    result["0:emitAreaX"] = orginObj["sourcePositionVariancex"].toVariant();
    result["0:emitAreaY"] = orginObj["sourcePositionVariancey"].toVariant();
    result["0:speed"] = orginObj["speed"].toVariant();
    result["0:speedVar"] = orginObj["speedVariance"].toVariant();
    result["0:gravityX"] = orginObj["gravityx"].toVariant();
    result["0:gravityY"] = orginObj["gravityy"].toVariant();
    result["0:accelRad"] = orginObj["radialAcceleration"].toVariant();
    result["0:accelRadVar"] = orginObj["radialAccelVariance"].toVariant();
    result["0:accelTan"] = orginObj["tangentialAcceleration"].toVariant();
    result["0:accelTanVar"] = orginObj["tangentialAccelVariance"].toVariant();

    result["0:minHorizontalRadius"] = orginObj["maxRadius"].toVariant();
    result["0:minVerticalRadius"] = orginObj["maxRadius"].toVariant();
    result["0:maxHorizontalRadius"] = orginObj["minRadius"].toVariant();
    result["0:maxVerticalRadius"] = orginObj["minRadius"].toVariant();
    result["0:minHorizontalRadiusVar"] = orginObj["maxRadiusVariance"].toVariant();
    result["0:minVerticalRadiusVar"] = orginObj["maxRadiusVariance"].toVariant();
    result["0:rotatePerSecond"] = orginObj["rotatePerSecond"].toVariant();
    result["0:rotatePerSecondVar"] = orginObj["rotatePerSecondVariance"].toVariant();

    result["0:startSpin"] = orginObj["rotationStart"].toVariant();
    result["0:startSpinVar"] = orginObj["rotationStartVariance"].toVariant();
    result["0:endSpin"] = orginObj["rotationEnd"].toVariant();
    result["0:endSpinVar"] = orginObj["rotationEndVariance"].toVariant();

    const int tweenCount = 300;
    float interval = 1.0 / (tweenCount - 1);

    uint8_t startTweenSize, endTweenSize;
    if(orginObj["finishParticleSize"].toInt()  == -1)
        orginObj["finishParticleSize"] = orginObj["startParticleSize"];
    if(orginObj["startParticleSize"].toInt() > orginObj["finishParticleSize"].toInt())
    {
        result["0:size"] = orginObj["startParticleSize"].toVariant();
        result["0:sizeVar"] = orginObj["startParticleSizeVariance"].toVariant();
        startTweenSize = 255; endTweenSize = orginObj["finishParticleSize"].toInt() / result["0:size"].toFloat() * 255;
    }
    else
    {
        result["0:size"] = orginObj["finishParticleSize"].toVariant();
        result["0:sizeVar"] = orginObj["finishParticleSizeVariance"].toVariant();
        endTweenSize = 255; startTweenSize = orginObj["startParticleSize"].toInt() / result["0:size"].toFloat() * 255;
    }

    QJsonDocument jsonDoc;
    QJsonObject tweenObj;

    QJsonArray tweenSizeArray;
    for(int i = 0; i < 300; i++) {
        tweenSizeArray.append(uint8_t(startTweenSize * (1.0 - interval * i) + endTweenSize * interval * i));
    }
    tweenObj["tween"] = tweenSizeArray;
    jsonDoc.setObject(tweenObj);
    QFile outputFile(effectConfigPath+"/customize_tween_size.json");
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qInfo("output file parse open error");
    }
    QByteArray jsonData = jsonDoc.toJson();
    outputFile.write(jsonData);
    outputFile.close();

    result["0:tweenConfigSize"] = effectConfigPath+"/customize_tween_size.json";

    QJsonArray tweenAlphaArray;
    uint8_t startTweenAlpha = orginObj["startColorAlpha"].toDouble() * 255, endTweenAlpha = orginObj["finishColorAlpha"].toDouble() * 255;
    for(int i = 0; i < 300; i++) {
        tweenAlphaArray.append(uint8_t(startTweenAlpha * (1.0 - interval * i) + endTweenAlpha * interval * i));
    }
    tweenObj["tween"] = tweenAlphaArray;
    QFile outputFile2(effectConfigPath+"/customize_tween_alpha.json");
    if (!outputFile2.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qInfo("output file parse open error");
    }
    jsonDoc.setObject(tweenObj);
    outputFile2.write(jsonDoc.toJson());
    outputFile2.close();

    result["0:tweenConfigAlpha"] = effectConfigPath+"/customize_tween_alpha.json";

    QJsonArray tweenColorArray;

    QColor startTweenColor(orginObj["startColorRed"].toDouble() * 255,orginObj["startColorGreen"].toDouble() * 255,orginObj["startColorBlue"].toDouble() * 255);
    QColor endTweenColor(orginObj["finishColorRed"].toDouble() * 255,orginObj["finishColorGreen"].toDouble() * 255,orginObj["finishColorBlue"].toDouble() * 255);

    auto converToHex = [=](QColor& c)->QString{
        int r = c.red(), g = c.green(), b = c.blue();
        return "#"+QString::number(((r & 0xffu) << 16) | ((g & 0xffu) << 8) | (b & 0xffu),16);
    };
    for(int i = 0; i < 300; i++) {
        QJsonArray color({(uint8_t)(startTweenColor.red() * (1.0 - interval * i) + endTweenColor.red() * interval * i),
                          (uint8_t)(startTweenColor.green() * (1.0 - interval * i) + endTweenColor.green() * interval * i),
                          (uint8_t)(startTweenColor.blue() * (1.0 - interval * i) + endTweenColor.blue() * interval * i)});
        tweenColorArray.append(color);
    }
    tweenObj["tween"] = tweenColorArray;
    QJsonArray colorPoints;
    colorPoints.append(QJsonObject({QPair<QString, QJsonValue>("color", converToHex(startTweenColor)),
                                    QPair<QString, QJsonValue>("position", 0)}));
    colorPoints.append(QJsonObject({QPair<QString, QJsonValue>("color", converToHex(endTweenColor)),
                                    QPair<QString, QJsonValue>("position", 1)}));
    tweenObj["points"] = colorPoints;
    QFile outputFile3(effectConfigPath+"/customize_tween_color.json");
    if (!outputFile3.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qInfo("output file parse open error");
    }
    jsonDoc.setObject(tweenObj);
    outputFile3.write(jsonDoc.toJson());
    outputFile3.close();

    result["0:tweenConfigColor"] = effectConfigPath+"/customize_tween_color.json";

    return result;
}


