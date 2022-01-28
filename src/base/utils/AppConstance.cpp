#include "AppConstance.h"

#include <QDebug>

#include "BuildConfig.h"
#include "SharedPref.h"
#include "base/utils/qt-detect-user-country.h"

QString AppConstance::s_appVersionWidthBuildNum = "";

bool AppConstance::isRelease() {
    return IS_RELEASE;
}

bool AppConstance::isTestServer() {
    if (isRelease()) {
        return false;
    } else {
        bool b = SharedPref::instance()->readBool("env_test");
        return b;
    }
}

void AppConstance::setTestServer(bool b) {
    if (isRelease()) {
        return;
    } else {
        SharedPref::instance()->setBool("env_test", b);
    }
}

QString AppConstance::serverDomain() {
    if (isTestServer()) {
        return SOURCE_DOMAIN_ENVTEST;
    } else {
        return SOURCE_DOMAIN;
    }
}

QString AppConstance::appKey() {
    return APP_KEY;
}
QString AppConstance::machaineName() {
    return QSysInfo::machineHostName();
}
QString AppConstance::appVerWithBuildNum() {
    if (s_appVersionWidthBuildNum.isEmpty()) {
        s_appVersionWidthBuildNum = QString("v%1(%2)").arg(APP_VERSION).arg(BUILD_NUM);
    }
    return s_appVersionWidthBuildNum;
}

QString AppConstance::appVer() {
    return APP_VERSION;
}

QString AppConstance::appId() {
    return KEY_APP_ID;
}

QString AppConstance::osVer() {
    return QSysInfo::productVersion();
}
QString AppConstance::deviceID() {
    return QSysInfo::machineUniqueId();
}

uint64_t AppConstance::uid() {
    return SharedPref::instance()->readUInt64(KEY_APP_UID);
}

QString AppConstance::nickname() {
    return SharedPref::instance()->readString(KEY_APP_NICKNAME);
}

void AppConstance::clearUserInfo() {
    SharedPref::instance()->removeValueForKey(KEY_APP_UID);
    SharedPref::instance()->removeValueForKey(KEY_APP_NICKNAME);
}

static QLocale thisLocale;
QStringList AppConstance::languages() {
    QStringList lst = thisLocale.uiLanguages();
    if (lst.count() > 0)
        return lst;

    return {"en-US"};
    // return QLocale::languageToString(httpLocale.language());
}

QLocale& AppConstance::locate(){
    return thisLocale;
}


QString AppConstance::country() {
    QList<QLocale> locales =
        QLocale::matchingLocales(QLocale::AnyLanguage, QLocale::AnyScript, LocalizationHelpers::detectUserCountry());
    QString ct = "CN";
    if (locales.size() > 0) {
        ct = locales.first().name().split('_').at(1).toUpper();
    }

    if (isRelease()) {
        return ct;
    } else {
        QString configCountry = SharedPref::instance()->readString("env_country");
        if (configCountry.length() > 0) {
            ct = configCountry;
        }
        return ct;
    }
}

void AppConstance::setCountry(QString ct) {
    if (isRelease()) {
        return;
    } else {
        SharedPref::instance()->setString("env_country", ct);
    }
}
