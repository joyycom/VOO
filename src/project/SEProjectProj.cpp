#include "SEProjectProj.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include <QTimer>
#include <QUrl>
#include <QUuid>
#include <QtGlobal>

#include "SEProject.h"
#include "base/http/SEHttpClient.h"
#include "base/utils/AppConstance.h"
#include "src/base/utils/fileutils.h"
#include "user/Login.h"

ProjType SEProjectProj::projTypeWithString(QString c) {
    if (c.toLower() == "timeline") {
        return Timeline;
    } else if (c.toLower() == "effect") {
        return Effect;
    } else if (c.toLower() == "file") {
        return File;
    } else {
        return UnKnow;
    }
}
QString SEProjectProj::stringWithProjType(ProjType projType) {
    switch (projType) {
        case Timeline:
            return "Timeline";
        case Effect:
            return "Effect";
        case File:
            return "File";
        default:
            return "";
    }
}

QString SEProjectProj::id() const {
    return m_id;
}
QString SEProjectProj::title() const {
    return m_title;
}
QString SEProjectProj::remoteId() const {
    return m_remoteId;
}
QString SEProjectProj::version() const {
    return m_version;
}
QString SEProjectProj::anchor() const {
    return m_anchor;
}
QString SEProjectProj::summary() const {
    return m_summary;
}
ProjType SEProjectProj::type() const {
    return m_type;
}
QString SEProjectProj::thumb() const {
    return FileUtils::getFileAbsolutePath(m_rootPath, m_thumb);
}

QString SEProjectProj::inputList() const {
    return FileUtils::getFileAbsolutePath(m_rootPath, m_inputList);
}

QString SEProjectProj::inputConfig() const {
    return m_inputConfig;
}
QDateTime SEProjectProj::createTimestamp() const {
    return m_createTimestamp;
}
QDateTime SEProjectProj::lastChangedTimestamp() const {
    return m_lastChangedTimestamp;
}
QString SEProjectProj::projectDir() const {
    return m_rootPath;
}
bool SEProjectProj::isDebug() const {
    return m_isDebug;
}

bool SEProjectProj::isTemp() const {
    return m_isTemp;
}

void SEProjectProj::setTitle(QString title) {
    m_title = title;
}

void SEProjectProj::updateCreateTimestamp() {
    m_createTimestamp = QDateTime::currentDateTime();
}

void SEProjectProj::setAnchor(QString s) {
    m_anchor = s;
}
void SEProjectProj::setSummary(QString s) {
    m_summary = s;
}
void SEProjectProj::setType(ProjType t) {
    m_type = t;
}
void SEProjectProj::setThumb(QString s) {
    m_thumb = s;
}
void SEProjectProj::setFile(QString filePath) {
    QFileInfo info(filePath);
    switch (m_type) {
        case Timeline:
            m_skyFile = "bundle://" + info.fileName();
            break;
        case Effect:
            m_ofFile = "bundle://" + info.fileName();
            break;
        case File:
        default:
            m_resFile = "bundle://" + info.fileName();
            break;
    }
}

void SEProjectProj::setRemoteId(QString remoteId) {
    m_remoteId = remoteId;
}

QString SEProjectProj::entryFile() {
    switch (m_type) {
        case Timeline:
            return FileUtils::realPath(m_rootPath, m_skyFile);
        case Effect:
            return FileUtils::realPath(m_rootPath, m_ofFile);
        case File:
        default:
            return FileUtils::realPath(m_rootPath, m_resFile);
    }
}

SEProjectProj::SEProjectProj(QString projectFile) {
    QFileInfo projFile(projectFile);
    m_rootPath = projFile.absolutePath();
    QDir dir(m_rootPath);
    if (!dir.exists()) {
        qInfo("open root_dir fail, not found!");
        return;
    }
    if (projFile.exists()) {
        QString context = FileUtils::readJson(projFile.filePath());
        qDebug() << "projFile  json : " << context;
        auto jsonObject = QJsonDocument::fromJson(context.toUtf8()).object();
        m_id = jsonObject.value("id").toString();
        m_title = jsonObject.value("title").toString().replace(":", "");
        m_remoteId = jsonObject.value("remoteId").toString();
        m_version = jsonObject.value("version").toString();
        m_anchor = jsonObject.value("anchor").toString();
        m_summary = jsonObject.value("summary").toString();
        QString t = jsonObject.value("type").toString();
        m_type = projTypeWithString(t);
        m_thumb = jsonObject.value("thumb").toString();
        if (m_thumb.isEmpty()) {
            m_thumb = "cover.jpg";
        }
        m_inputList = jsonObject.value("inputList").toString();
        if (m_inputList.isEmpty()) {
            m_inputList = "inputList.conf";
        }
        m_skyFile = jsonObject.value("skyFile").toString();
        if (m_skyFile.isEmpty()) {
            m_skyFile = "timeline.sky";
        }
        m_ofFile = jsonObject.value("ofFile").toString();
        m_resFile = jsonObject.value("resFile").toString();
        m_inputConfig = jsonObject.value("inputConfig").toString();
        m_createTimestamp = QDateTime::fromTime_t(jsonObject.value("createTimestamp").toDouble());
        m_lastChangedTimestamp = QDateTime::fromTime_t(jsonObject.value("lastChangedTimestamp").toDouble());
        m_isDebug = jsonObject.value("isDebug").toBool();
        m_extraData = jsonObject.value("extra").toObject();
    } else {
        qDebug("SEProjectProj init");
        QUuid IID_MyInterface(0x67c8770b, 0x44f1, 0x410a, 0xab, 0x9a, 0xf9, 0xb5, 0x44, 0x6f, 0x13, 0xee);
        m_id = IID_MyInterface.toString();
        m_title = dir.dirName();
        m_remoteId = "";
        m_version = AppConstance::appVer();
        m_anchor = "unknow";
        m_summary = "";
        m_type = Timeline;
        m_thumb = "cover.jpg";
        m_inputList = "inputList.conf";
        m_skyFile = "timeline.sky";
        m_ofFile = "";
        m_resFile = "";
        m_inputConfig = "";
        m_createTimestamp = QDateTime::currentDateTime();
        m_lastChangedTimestamp = m_createTimestamp;
        m_isDebug = true;
        m_isTemp = true;
    }
}

QString SEProjectProj::projFilePath(QString root_dir) {
    QString oldProjFile = findProjectFile(root_dir);
    if (oldProjFile.length() > 10) {
        return oldProjFile;
    } else {
        return QFileInfo(root_dir, QString("template.") + PROJ_SUFFIX).filePath();
    }
}

QString SEProjectProj::findProjectFile(QString root_dir) {
    QDir dir(root_dir);
    if (!dir.exists()) {
        qInfo("open root_dir fail, not found!");
        return "";
    }
    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList();
    if (list.count() == 0) {
        return "";
    }
    int i = 0;
    QFileInfo projFile;
    bool finded = false;
    do {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.suffix() == PROJ_SUFFIX) {
            projFile = fileInfo;
            finded = true;
            break;
        }
        i++;
    } while (i < list.size());
    if (!finded) {
        return "";
    }
    return QDir::toNativeSeparators(projFile.absoluteFilePath());
}

void SEProjectProj::createFile(QString projectFile) {
    SEProjectProj newProjFile(projectFile);
    QFileInfo file(projectFile);
    if (file.exists()) {
        QFile::remove(projectFile);
    }
    // create empty file
    newProjFile.realSave(file.absoluteDir().absolutePath());
}

void SEProjectProj::save(QString root_dir) {
    if (!SEProject::isQuit() && m_type == Timeline) {
        QImage image = SEProject::current()->dom()->previewController()->getSkyTimelineView()->snapFrame();
        qDebug() << "SEProjectProj::snapFrame " << &image;
        if (!image.isNull()) {
            QString imagePath = thumb();
            if (QFile::exists(imagePath)) {
                QFile::remove(imagePath);
            }
            image.save(imagePath, "JPG");
        }
    }
    this->realSave(root_dir);
}

void SEProjectProj::realSave(QString root_dir) {
    QString prjFilePath = projFilePath(root_dir);
    QFile file(prjFilePath);
    m_lastChangedTimestamp = QDateTime::currentDateTime();
    if (file.open(QIODevice::WriteOnly)) {
        QJsonObject root;
        root.insert("id", m_id);
        root.insert("title", m_title);
        root.insert("remoteId", m_remoteId);
        root.insert("version", m_version);
        root.insert("anchor", m_anchor);
        root.insert("summary", m_summary);
        root.insert("type", stringWithProjType(m_type));
        root.insert("thumb", m_thumb);
        root.insert("inputList", m_inputList);
        root.insert("skyFile", m_skyFile);
        root.insert("ofFile", m_ofFile);
        root.insert("resFile", m_resFile);
        root.insert("inputConfig", m_inputConfig);
        root.insert("createTimestamp", QString::number(m_createTimestamp.toTime_t()));
        root.insert("lastChangedTimestamp", QString::number(m_lastChangedTimestamp.toTime_t()));
        root.insert("isDebug", QString::number(m_isDebug));
        root.insert("extra", m_extraData);
        QJsonDocument doc(root);
        QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
        file.write(jsonData);
        file.close();
    } else {
        qInfo("SEProjectProj updateFile fail! File: %s", file.fileName().toStdString().c_str());
    }
}

QJsonValue SEProjectProj::getExtraData(const QString &key) const {
    return m_extraData.value(key);
}

void SEProjectProj::setExtraData(const QString &key, const QJsonValue &value) {
    m_extraData.insert(key, value);
}

void SEProjectProj::setIsTemp(bool isTemp) {
    m_isTemp = isTemp;
}
