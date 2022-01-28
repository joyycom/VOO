#include "SharedPref.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>

#include "AppPath.h"
#include "fileutils.h"
using namespace std;

Q_GLOBAL_STATIC(SharedPref, sharedPref)
SharedPref *SharedPref::instance() {
    return sharedPref();
}

SharedPref::SharedPref() {
    QString save = AppPath::sharedPrefPath();
    MMKV::initializeMMKV(string2MMKVPath_t(save.toStdString()));
    m_mmkv = MMKV::mmkvWithID("app.config");
}

SharedPref::SharedPref(MMKV *mmkv) {
    m_mmkv = mmkv;
}

SharedPref::~SharedPref() {
    m_mmkv->close();
    m_mmkv = nullptr;
}

// read
QString SharedPref::readString(QString k) {
    string result;
    m_mmkv->getString(k.toStdString(), result);
    return QString::fromStdString(result);
}
int SharedPref::readInt(QString k, int32_t defaultValue) {
    return m_mmkv->getInt32(k.toStdString(), defaultValue);
}
uint64_t SharedPref::readUInt64(QString k, uint64_t defaultValue) {
    return m_mmkv->getUInt64(k.toStdString(), defaultValue);
}
bool SharedPref::readBool(QString k, bool defaultValue) {
    return m_mmkv->getBool(k.toStdString(), defaultValue);
}
QJsonObject SharedPref::readJson(QString k) {
    QString json = readString(k);
    if (json.isEmpty()) {
        return QJsonObject();
    }
    return QJsonDocument::fromJson(json.toUtf8()).object();
}
QJsonArray SharedPref::readJsonArray(QString k) {
    QString json = readString(k);
    if (json.isEmpty()) {
        return QJsonArray();
    }
    return QJsonDocument::fromJson(json.toUtf8()).array();
}

// write
void SharedPref::setString(QString k, QString v) {
    m_mmkv->set(v.toStdString(), k.toStdString());
}
void SharedPref::setInt(QString k, int v) {
    m_mmkv->set(v, k.toStdString());
}
void SharedPref::setUInt64(QString k, uint64_t v) {
    m_mmkv->set(v, k.toStdString());
}
void SharedPref::setBool(QString k, bool v) {
    m_mmkv->set(v, k.toStdString());
}
void SharedPref::setJson(QString k, QJsonObject v) {
    m_mmkv->set(QJsonDocument(v).toJson().toStdString(), k.toStdString());
}
void SharedPref::setJsonArray(QString k, QJsonArray v) {
    m_mmkv->set(QJsonDocument(v).toJson().toStdString(), k.toStdString());
}
void SharedPref::removeValueForKey(QString k) {
    m_mmkv->removeValueForKey(k.toStdString());
}
