#include "EffectInputListComponent.h"

#include <QJsonArray>
#include <QDebug>
#include <QDir>
#include "base/utils/AppConstance.h"

EffectInputListComponent::EffectInputListComponent(QObject *parent) : BaseComponent(parent) {
    m_inputTypeList.append("background");
    m_inputTypeList.append("user");
    m_inputTypeList.append("gap");
    m_inputTypeList.append("path");
}

void EffectInputListComponent::onBindQml(QJsonObject &paramInfo) {
    m_paramText = translate(paramInfo["paramTitle"].toString());

    m_inputIndex = paramInfo["InputIndex"].toInt(-1);
    if(m_inputIndex < 0){
        return;
    }

    auto inputList = curEffect()->skyEffect()->getInputList();

    if(inputList->numberOfItems() <= m_inputIndex){
        inputList->ensureNumber(m_inputIndex);
        inputList->getItemAt(m_inputIndex)->setAsResource(SkyResource::makeGap());
        inputList->commit();
    }
    QString resPath = QString::fromUtf8(inputList->getItemAt(m_inputIndex)->getResource().getPath().c_str());
    if(resPath.startsWith("skymedia://")){
        if(resPath.compare("skymedia://user") == 0){
            m_inputTypeIndex = INPUT_TYPE_USER_INDEX;
        }else if(resPath.compare("skymedia://background") == 0){
            m_inputTypeIndex = INPUT_TYPE_BACKGROUND_INDEX;
        }else{
            m_inputTypeIndex = INPUT_TYPE_GAP_INDEX;
        }
    }else{
        m_inputTypeIndex = INPUT_TYPE_PATH_INDEX;

        if(!QFileInfo::exists(resPath)){
            QString ofPath = curEffect()->skyResource().entryPath();
            QDir dir = QFileInfo(ofPath).absoluteDir();
            if(QFileInfo(dir, resPath).exists()){
                m_resPath = QFileInfo(dir, resPath).absoluteFilePath();
            }else{

            }
        }else{
            m_resPath = resPath;
        }
    }

    qDebug() << "onBindQml InputList:"<<m_inputIndex<<" ->" << m_resPath;
}

void EffectInputListComponent::onUnBind() {
}


void EffectInputListComponent::setCustomResPath(QUrl path){
    QString filePath = path.toLocalFile();
    SkyResource resource = SkyResource(filePath.toUtf8().data());
    if(resource.hasVideoStream()){
        auto inputList = curEffect()->skyEffect()->getInputList();
        if(inputList->numberOfItems() < m_inputIndex){
            inputList->ensureNumber(m_inputIndex);
        }else{
        }
        inputList->getItemAt(m_inputIndex)->setAsResource(resource);
        inputList->commit();
        m_inputTypeIndex = INPUT_TYPE_PATH_INDEX;
        m_resPath = QString::fromUtf8(resource.getPath().c_str());
        emit customResPathChanged(m_resPath);
    }else{

    }
}

void EffectInputListComponent::setInputTypeIndex(int inputType){
    if(inputType == INPUT_TYPE_PATH_INDEX || !hadBindComponent()){
        return;
    }

    auto inputList = curEffect()->skyEffect()->getInputList();
    if(inputList->numberOfItems() < m_inputIndex){
        inputList->ensureNumber(m_inputIndex);
    }else{
    }
    if(inputType == INPUT_TYPE_USER_INDEX){
        inputList->getItemAt(m_inputIndex)->setAsUser();
    }else if(inputType == INPUT_TYPE_BACKGROUND_INDEX){
        inputList->getItemAt(m_inputIndex)->setAsBackground();
    }else if(inputType == INPUT_TYPE_GAP_INDEX){
        SkyResource gapResource = SkyResource::makeGap();
        inputList->getItemAt(m_inputIndex)->setAsResource(gapResource);
    }else{

    }
    inputList->commit();
    m_inputTypeIndex = inputType;

}




