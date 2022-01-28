#include "NameComponent.h"

#include <QJsonArray>
#include <QDebug>

NameComponent::NameComponent(QObject *parent) : BaseComponent(parent) {
}

void NameComponent::onBindQml(QJsonObject &paramInfo) {
    m_labelName = paramInfo["paramTitle"].toString();
    if (curEffect() != nullptr) {
        m_name = curEffect()->skyResource().name();
    } else {
        m_name = "";
    }
}

void NameComponent::onUnBind() {
}

QString NameComponent::labelText() const {
    return translate(m_labelName);
}

QString NameComponent::name() const {
    return m_name;
}
