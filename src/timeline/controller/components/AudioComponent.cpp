#include "AudioComponent.h"

#include <QJsonArray>
#include <QDebug>

AudioComponent::AudioComponent(QObject *parent) : BaseComponent(parent) {
}

void AudioComponent::onBindQml(QJsonObject &paramInfo) {
    m_labelText = paramInfo["paramTitle"].toString();
    emit labelTextChanged(m_labelText);
}

void AudioComponent::onUnBind() {
}

QString AudioComponent::labelText() const {
    return translate(m_labelText);
}
