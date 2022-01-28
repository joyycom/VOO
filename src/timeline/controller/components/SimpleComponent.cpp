#include "SimpleComponent.h"

#include <QJsonArray>
#include <QDebug>

SimpleComponent::SimpleComponent(QObject *parent) : BaseComponent(parent) {
}

void SimpleComponent::onBindQml(QJsonObject &paramInfo) {
}

void SimpleComponent::onUnBind() {
}
