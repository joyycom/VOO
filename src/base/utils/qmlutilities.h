#ifndef QMLUTILITIES_H
#define QMLUTILITIES_H

#include <QObject>
#include <QQmlApplicationEngine>

class QmlUtilities : public QObject {
    Q_OBJECT

public:
    explicit QmlUtilities(QObject *parent = 0);

    static void registerCommonTypes();
    static QQmlApplicationEngine *sharedEngine();
};

#endif // QMLUTILITIES_H
