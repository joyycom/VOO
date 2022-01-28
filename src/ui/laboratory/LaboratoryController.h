#ifndef LaboratoryController_H
#define LaboratoryController_H

#include <QObject>
#include <QUrl>

#include <base/utils/ThreadWroker.h>

class LaboratoryController : public QObject {
    Q_OBJECT
public:
    explicit LaboratoryController(QObject *parent = nullptr);

signals:
    void serverEnvChanged(QString t);
    void countryChanged(QString t);
    void ofPathChanged(QString path);

public slots:
    QString serverType();
    void changeServerEnv();
    QString country();
    void changeCountry(QString ct);
    void testVenus(QUrl url);
    void changeOfPath(QUrl url);
    void changeTimelineWatch(bool check);

    QString ofPath();

private:
    ThreadWroker m_worker;
};

#endif
