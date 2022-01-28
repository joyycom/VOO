#include "qmlutil.h"
#include <QGuiApplication>
#include <QDebug>

int QmlUtils::keyboardModifiers() {
    return QGuiApplication::keyboardModifiers();
}

