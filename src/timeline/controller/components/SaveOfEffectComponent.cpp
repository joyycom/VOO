#include "SaveOfEffectComponent.h"

#include <QJsonArray>
#include <QDebug>

SaveOfEffectComponent::SaveOfEffectComponent(QObject *parent) : BaseComponent(parent) {
}

void SaveOfEffectComponent::onBindQml(QJsonObject &paramInfo) {
    m_labelText = paramInfo["paramTitle"].toString();
}

void SaveOfEffectComponent::onUnBind() {
}

QString SaveOfEffectComponent::labelText() const {
    return translate(m_labelText);
}


void SaveOfEffectComponent::saveEffect(){
    curEffect()->saveOfEffectParamFile();
}
