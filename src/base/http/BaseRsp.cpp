#include "BaseRsp.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

BaseRsp::BaseRsp(const QString &response) {

    auto jsonObject = QJsonDocument::fromJson(response.toUtf8()).object();

    if (jsonObject.contains("code"))
        m_code = jsonObject.value("code").toInt();
    else
        m_code = -99;
    if (jsonObject.contains("msg"))
        m_message = jsonObject.value("msg").toInt();
    else
        m_message = "unkonw error";
    if (jsonObject.contains("data"))
        m_data = jsonObject.value("data");
    else
        m_data = "";
}

bool BaseRsp::isSuccess() {
    return m_code == 0;
}

int BaseRsp::code() const {
    return m_code;
}

QString BaseRsp::message() const {
    return m_message;
}

QJsonValue BaseRsp::data() const {
    return m_data;
}
