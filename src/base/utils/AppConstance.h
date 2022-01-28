#ifndef AppConstance_H
#define AppConstance_H

#include <QLocale>
#include <QString>

#include "AppConstDef.h"

class AppConstance {
public:
    static bool isRelease();
    static bool isTestServer();
    static void setTestServer(bool b);

    static QString serverDomain();
    static QString appKey();
    static QString appVer();
    static QString appId();
    static QString appVerWithBuildNum();
    static QString machaineName();
    static QString osVer();
    static QString deviceID();
    static uint64_t uid();
    static QString nickname();
    static void clearUserInfo();
    static QStringList languages();

    static QLocale& locate();


    static QString country();
    static void setCountry(QString ct);

private:
    static QString s_appVersionWidthBuildNum;
};
#endif // AppPath_H
