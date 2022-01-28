#include "LabelComponent.h"

#include <QJsonArray>
#include <QDebug>

LabelComponent::LabelComponent(QObject *parent) : BaseComponent(parent) {
}

void LabelComponent::onBindQml(QJsonObject &paramInfo) {
    m_labelText = paramInfo["paramTitle"].toString();
    emit labelTextChanged(m_labelText);
}

void LabelComponent::onUnBind() {
}

QString LabelComponent::labelText() const {
    return translate(m_labelText);
}
