#ifndef CRASHHELPER_H
#define CRASHHELPER_H

#include <QObject>

class CrashHelper : public QObject {
    Q_OBJECT
public:
    explicit CrashHelper(QObject *parent = nullptr);

    ~CrashHelper();
signals:

private:
    void initCrash();
};

#endif // CRASHHELPER_H
