#include "inputDevice.h"

#include <QDateTime>
#include <QObject>
#include <QTextCodec>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>

InputDevice InputDevice::makeInputDeviceFromJson(const QJsonObject &json) {
    InputDevice inputDevice;
    inputDevice.init(json);
    return inputDevice;
}

InputDevice InputDevice::makeInputDevice(const SkyResourceBean &skyResourceBean, SkyClip *skyClip) {
    InputDevice inputDevice;
    inputDevice.init(skyResourceBean, skyClip);
    return inputDevice;
}

bool InputDevice::isNull() {
    return m_data.isEmpty();
}

void InputDevice::init(const QJsonObject &json) {
    m_data = json;
}

void InputDevice::init(const SkyResourceBean &skyResourceBean, SkyClip *skyClip) {
    m_data.insert("id", QDateTime::currentDateTime().toMSecsSinceEpoch());
    ResourceType type = skyResourceBean.resouceType();
    auto range = skyClip->getTrackRange();
    switch (type) {
        case VideoResource: {
            m_data.insert("type", "video");
            m_data.insert("title", QObject::tr("video"));
            m_data.insert("duration", range.end - range.begin);
            break;
        }
        case AudioResource:
            m_data.insert("type", "music");
            m_data.insert("title", QObject::tr("music"));
            m_data.insert("duration", range.end - range.begin);
            break;
        case ImageResource: {
            m_data.insert("type", "image");
            m_data.insert("title", QObject::tr("image"));
            QString path = skyResourceBean.entryPath();
            QFileInfo f(path);
            QString maskFile =
                skyResourceBean.entryPath().replace(f.completeBaseName(), f.completeBaseName() + "_mask");
            if (QFile::exists(maskFile)) {
                m_data.insert("mask", maskFile);
            }
            QString alphaFile =
                skyResourceBean.entryPath().replace(f.completeBaseName(), f.completeBaseName() + "_alpha");
            if (QFile::exists(alphaFile)) {
                m_data.insert("alpha", alphaFile);
            }
            break;
        }
        case TextResource:
            m_data.insert("type", "string");
            m_data.insert("title", QObject::tr("string"));
            break;
        default:
            break;
    }
    m_data.insert("group", m_data.value("type"));
    if (type != TextResource) {
        m_data.insert("path", skyResourceBean.entryPath());
    }
    auto stream = skyClip->getResource().firstVideoStream();
    m_data.insert("width", stream.width);
    m_data.insert("height", stream.height);
}

QJsonObject InputDevice::toJson() {
    return m_data;
}

void InputDevice::setId(qint64 id) {
    m_data.insert("id", id);
}

qint64 InputDevice::getId() {
    return m_data.value("id").toDouble();
}

QString InputDevice::getPath() {
    return m_data.value("path").toString();
}

QString InputDevice::getTitle() {
    return m_data.value("title").toString();
}

void InputDevice::setPath(const QString &path) {
    m_data.insert("path", path);
}

QString InputDevice::getType() {
    return m_data.value("type").toString();
}

void InputDevice::addObjName(const QString &name) {
    if (name.isEmpty()) {
        return;
    }
    QJsonArray array = m_data.value("paramSettingInfo").toArray();
    QJsonObject object;
    object.insert("objName", name);
    array.append(object);
    m_data.insert("paramSettingInfo", array);
}

void InputDevice::addUpdateParam(const QString &key, const QJsonValue &value) {
    QJsonObject params = m_data.value("updateParam").toObject();
    params.insert(key, value);
    m_data.insert("updateParam", params);
}

void InputDevice::addClipId(qint64 id) {
    QJsonArray array = m_data.value("clips").toArray();
    array.append(id);
    m_data.insert("clips", array);
}

bool InputDevice::equalsClipId(qint64 id) {
    QJsonArray array = m_data.value("clips").toArray();
    return array.contains(id) || id == getId();
}

void InputDevice::removeInfos() {
    m_data.remove("clips");
    m_data.remove("updateParam");
    m_data.remove("paramSettingInfo");
}
