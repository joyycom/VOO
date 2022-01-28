#include "TTSComponent.h"

#include <QJsonArray>
#include <QDebug>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QFileInfo>

#include <base/utils/AppPath.h>
#include <base/utils/JsonUtils.h>
#include <base/utils/fileutils.h>

#include <base/http/SEHttpClient.h>
#include <project/SEProjectDom.h>

#include <project/SEProject.h>
#include "base/utils/AppConstDef.h"


void TTSBeanListModel::addData(const QList<TTSBean> &data) {
    if (data.size() > 0) {
        int preSize = m_datas.size();
        int curSize = m_datas.size() + data.size() - 1;
        beginInsertRows(QModelIndex(), preSize, curSize);
        m_datas.append(data);
        endInsertRows();
    }
}

TTSBean TTSBeanListModel::dataWith(int index) {
    return m_datas[index];
}

TTSBean TTSBeanListModel::curSelectedBean(){
    if(m_selectedIndex < 0|| m_selectedIndex >= m_datas.size()){
        return TTSBean("","");
    }
    return m_datas[m_selectedIndex];
}

void TTSBeanListModel::setCurItemDownloaded(bool downloaded){
    if(m_selectedIndex < 0|| m_selectedIndex >= m_datas.size()){
        return ;
    }

    QModelIndex modelIndex = index(m_selectedIndex);
    m_datas[m_selectedIndex].setDownloaded(downloaded);
    QVector<int> vector;
    vector.append(DownloadedRole);
    emit dataChanged(modelIndex, modelIndex, vector);
}

void TTSBeanListModel::clearAll() {
    if (m_datas.size() > 0) {
        beginRemoveRows(QModelIndex(), 0, m_datas.size() - 1);
        m_datas.clear();
        endRemoveRows();
    }
    m_selectedIndex = -1;
}

void TTSBeanListModel::setSelectedIndex(int i) {
    int preIndex = m_selectedIndex;
    m_selectedIndex = i;
    QModelIndex modelIndex = index(i);
    QVector<int> vector;
    vector.append(SelectedRole);
    emit dataChanged(modelIndex, modelIndex, vector);
    emit dataChanged(index(preIndex), index(preIndex), vector);
}



TTSComponent::TTSComponent(QObject *parent) : BaseComponent(parent) {
}

void TTSComponent::onBindQml(QJsonObject &paramInfo) {
    auto languages = loadTtsSettings();
    m_languageModel->addData(languages);


}

void TTSComponent::selectLanguageIndex(int index){
    m_languageModel->setSelectedIndex(index);
    auto languageBean = m_languageModel->dataWith(index);
    m_voiceModel->clearAll();
    m_voiceModel->addData(languageBean.subBeans());
}

void TTSComponent::selectVoiceIndex(int index){
    m_voiceModel->setSelectedIndex(index);
    downloadTTSVoice();
}

void TTSComponent::onUnBind() {
    if(m_voiceSound){
        m_voiceSound->stop();
        delete m_voiceSound;
    }
    m_voiceSound = nullptr;
}

void TTSComponent::downloadTTSVoice(){
    QString language = m_languageModel->curSelectedBean().id();
    QString voice = m_voiceModel->curSelectedBean().id();
    QString text = QString::fromUtf8(getOfParamValue("0:Text").toString());
    if(language.isEmpty() || voice.isEmpty() || text.isEmpty()){
        qDebug()<<"args is Empty?"<<language << " Text"<<text;
        return;
    }

    QString fileName = QString("%1_%2_%3.wav")
            .arg(language)
            .arg(voice)
            .arg(QString::fromUtf8(QCryptographicHash::hash(text.toUtf8(), QCryptographicHash::Md5).toHex()));
    QString filePath = AppPath::mkPathIfNeed(AppPath::tempPath("tts/"))+fileName;

    if(QFileInfo::exists(filePath)){
        qDebug()<<" Audio File Exists! Play Vocie NOW! "<<filePath;
        m_voiceModel->setCurItemDownloaded(true);
        playCurVoice(filePath);
        return;
    }

    if(m_voiceSound){
        m_voiceSound->stop();
        delete m_voiceSound;
    }
    m_voiceSound = nullptr;
    if(m_curReply != nullptr){
        m_curReply->abort();
        qDebug()<<" TTS Task Exists! task: "<<fileName;
    }

    qDebug()<<" request TTS fileName:"<<fileName<< " Text:"<<text;

    m_voiceModel->setCurItemDownloaded(false);

    auto  jsonParam = QJsonObject();
    jsonParam.insert("language",language);
    jsonParam.insert("voice",voice);
    jsonParam.insert("bet","bet1");
    jsonParam.insert("audio_format","wav");
    jsonParam.insert("ssml",false);
    jsonParam.insert("text",text);
    jsonParam.insert("sr",24000);
    m_curReply = SEHttpClient(TTS_URL)
            .addHeader("Authorization",TTS_AUTHORIZATION)
            .addHeader("Content-Type","application/json")
            .addJsonParam(jsonParam)
        .post([=](const QString &response) {
            auto respObj = QJsonDocument::fromJson(response.toUtf8()).object();
            if(respObj.value("code") == 200){
                qDebug()<<" request TTS language Success :"<<language<<" Voice:"<<voice << " Text:"<<text;

                auto audio_data_base64 = respObj.value("data").toObject().value("audio_data").toString();
                auto audioData = QByteArray::fromBase64(audio_data_base64.toUtf8(), QByteArray::Base64Encoding);
                QFile audioFile(filePath);
                if(audioFile.open(QFile::WriteOnly)){
                    audioFile.write(audioData);
                    audioFile.flush();
                    audioFile.close();
                }

            }else{
                qDebug()<<" request TTS response Error :"<<response;
            }
            QString curLanguage = m_languageModel->curSelectedBean().id();
            QString curVoice = m_voiceModel->curSelectedBean().id();
            QString curText = QString::fromUtf8(getOfParamValue("0:Text").toString());
            if(curLanguage == language && curVoice == voice && text == curText){
                m_curReply = nullptr;
                if(QFileInfo::exists(filePath)){
                    m_voiceModel->setCurItemDownloaded(true);
                    playCurVoice(filePath);
                }
            }
    },[=](const QString &error){
        QString curLanguage = m_languageModel->curSelectedBean().id();
        QString curVoice = m_voiceModel->curSelectedBean().id();
        QString curText = QString::fromUtf8(getOfParamValue("0:Text").toString());
        if(curLanguage == language && curVoice == voice && text == curText){
            m_curReply = nullptr;
            SEProject::current()->dom()->propertyEditController()->showToast("Network Error!");
        }
        qDebug()<<" request TTS Network Error :"<<error;

    });
}

void TTSComponent::playCurVoice(QString path){
    if(m_voiceSound){
        m_voiceSound->stop();
        delete m_voiceSound;
    }
    m_voiceSound = new QSound(path);
    m_voiceSound->play();
}

void TTSComponent::addCurVoiceResource(){
    QString language = m_languageModel->curSelectedBean().id();
    QString voice = m_voiceModel->curSelectedBean().id();
    QString text = QString::fromUtf8(getOfParamValue("0:Text").toString());
    if(language.isEmpty() || voice.isEmpty() || text.isEmpty()){
        qDebug()<<"args is Empty?"<<language << " Text"<<text;
        return;
    }

    QString fileName = QString("%1_%2_%3.wav")
            .arg(language)
            .arg(voice)
            .arg(QString::fromUtf8(QCryptographicHash::hash(text.toUtf8(), QCryptographicHash::Md5).toHex()));
    QString filePath = AppPath::mkPathIfNeed(AppPath::tempPath("tts/"))+fileName;
    auto targetPath = SEProject::current()->rootPath()+"/"+fileName;
    FileUtils::copyFile(filePath,targetPath,true);

    SkyResourceBean resourceBean(targetPath,text,AudioResource);

    double startTime = curClip()->startTime();
    SEProject::current()->dom()->timelineController()->addResource(resourceBean,-1,startTime);
}

TTSComponent::~TTSComponent() {
    if(m_languageModel != nullptr){
        delete m_languageModel;
    }
    m_languageModel = nullptr;

    if(m_voiceModel){
        delete m_voiceModel;
    }
    m_voiceModel = nullptr;
}

QList<TTSBean> TTSComponent::loadTtsSettings(){
    QList<TTSBean> languageBeans;
    auto ttsSettingsPath =
        QCoreApplication::applicationDirPath().append("/Configs/tts_settings.conf");
    auto ttsSettings = JsonUtils::jsonObjectFromFile(ttsSettingsPath);
    if(!ttsSettings.isObject()){
        return languageBeans;
    }
    auto language_voice = ttsSettings.object().value("language_voice");
    auto description = ttsSettings.object().value("description");
    if(!language_voice.isObject()||!description.isObject()){
        return languageBeans;
    }
    auto languageObj = language_voice.toObject().keys();
    auto descObj = description.toObject();

    QList<TTSBean> EMTPY;

    for(auto lang:languageObj){
        auto langDesc = descObj.value(lang).toString(lang);
        QList<TTSBean> voices;
        auto voiceArrays = language_voice.toObject().value(lang).toArray();
        for(auto v:voiceArrays){
            auto voice = v.toString();
            auto voiceDesc = descObj.value(voice).toString(voice);
            voices.append(TTSBean(voice,voiceDesc,EMTPY));
        }
        languageBeans.append(TTSBean(lang,langDesc,voices));
    }
    return languageBeans;
}
