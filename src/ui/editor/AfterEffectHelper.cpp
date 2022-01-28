#include "AfterEffectHelper.h"

#include <QCoreApplication>
#include <QProcess>
#include <QTextCodec>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFileInfo>

#include <base/utils/AppPath.h>
#include <base/utils/fileutils.h>
#include <base/utils/threadpools.h>

AfterEffectHelper::AfterEffectHelper()
{
#ifdef Q_OS_WIN
    ThreadPools::instance()->exec([=](){
        copyAeScriptToTargetDir();
        saveExportScriptConfig();
    });
#endif
}

bool AfterEffectHelper::importFromAfterFx(){
#ifdef Q_OS_WIN
    auto afterFxPath = findRunningAfterFxPath();
    if(!afterFxPath.isEmpty()){
        startWatchResultFile();
        m_isImportFromAE = true;

        runAEScript(afterFxPath);
        return true;
    }
    return false;
#else
    return false;
#endif
}

AfterEffectHelper* AfterEffectHelper::instance(){
    static AfterEffectHelper helper;
    return &helper;
}

AfterEffectHelper::~AfterEffectHelper(){
    stopWatchResultFile();
}

void AfterEffectHelper::saveExportScriptConfig(){
    QJsonObject config;
    config.insert("ffmpeg",QCoreApplication::applicationDirPath().append("/ffmpeg.exe"));
    config.insert("targetDir",AppPath::mkPathIfNeed(AppPath::tempProjectPath("AE_Export")));
    config.insert("callback",QCoreApplication::applicationDirPath().append("/VOO.exe"));
    config.insert("enableNormalExport",false);
    config.insert("enableVooExport",true);
    config.insert("logDir",AppPath::logPath());

    auto exportResultPath = AppPath::tempProjectPath("AE_Export").append("/ExportResult.json");
    config.insert("exportResultPath",exportResultPath);

    auto configPath = AppPath::aescript().append("/ExportConfig.json");
    QJsonDocument doc(config);
    QFile file(configPath);
    if(file.open(QFile::WriteOnly)){
        file.write(doc.toJson());
        file.flush();
        file.close();
    }

}

void AfterEffectHelper::copyAeScriptToTargetDir(){
    auto ae2of = QCoreApplication::applicationDirPath().append("/ae2of/");
    auto targetDir = AppPath::aescript();
    FileUtils::copyDirectoryFiles(ae2of,targetDir,true);
}

void AfterEffectHelper::runAEScript(QString exePath){
    QProcess p;
    p.setReadChannel(QProcess::StandardOutput);
    p.setReadChannelMode(QProcess::MergedChannels);
    auto ae2of = AppPath::aescript().append("/ae2of.jsx");
    QStringList opt;
    opt << "-r" << ae2of;
    p.start(exePath, opt);
    p.waitForFinished();
    QTextCodec* pTextCodec = QTextCodec::codecForName("System");
    auto content = pTextCodec->toUnicode(p.readAll());
    qDebug()<<"runAEScript:"<<content;
    p.close();
}

QString AfterEffectHelper::findRunningAfterFxPath(){
    QProcess p;
    p.setReadChannel(QProcess::StandardOutput);
    p.setReadChannelMode(QProcess::MergedChannels);
    QStringList opt;
    opt << "process" << "where"
        << "name='AfterFX.exe'"
        << "get" << "executablepath";
    p.start("wmic", opt);
    p.waitForFinished();
    QTextCodec* pTextCodec = QTextCodec::codecForName("System");
    auto content = pTextCodec->toUnicode(p.readAll());
    if(content.contains("ExecutablePath")){
        content = content.remove("ExecutablePath").remove("\r").remove("\n").trimmed();
    }else{
        content = "";
    }
    p.close();

    qDebug()<<"findRunningAfterFxPath:"<<content;

    return content;
}

void AfterEffectHelper::checkResultFileExits(){

    auto path = AppPath::tempProjectPath("AE_Export").append("/ExportResult.json");

    if(QFileInfo::exists(path) && m_isImportFromAE){
        qDebug()<<"onImportResultFileChanged:"<<path;

        auto resultContent = FileUtils::readJson(path);
        auto doc = QJsonDocument::fromJson(resultContent.toUtf8());
        int code = doc.object().value("code").toInt(-1);
        QString msg =  doc.object().value("msg").toString("");
        QString targetFile =  doc.object().value("targetFile").toString("");

        qDebug()<<"Found Result File:"<<resultContent;

        m_isImportFromAE = false;
        if(code == 0 && QFileInfo::exists(targetFile)){
            emit importFromAeResult(code,msg,targetFile);
        }else{
            emit importFromAeResult(code,msg,"");
        }
        stopWatchResultFile();

    }
}

void AfterEffectHelper::startWatchResultFile(){
    auto exportResultFile =  AppPath::tempProjectPath("AE_Export").append("/ExportResult.json");
    QFile::remove(exportResultFile);
    if(m_timer == nullptr){
        m_timer = new QTimer();
        m_timer->setInterval(1000);
        connect(m_timer, &QTimer::timeout, this,
                &AfterEffectHelper::checkResultFileExits);
        m_timer->start(100);
    }

}

void AfterEffectHelper::stopWatchResultFile(){
    if(m_timer != nullptr){
        m_timer->stop();
        disconnect(m_timer, &QTimer::timeout, this,
                   &AfterEffectHelper::checkResultFileExits);
        m_timer = nullptr;

    }
}
