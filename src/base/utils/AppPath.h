#ifndef AppPath_H
#define AppPath_H

#include <QString>

class AppPath {
public:
    static QString rootPath(QString file = nullptr);
    static QString materialPath(QString file = nullptr);
    static QString tempProjectPath(QString file = nullptr);
    static QString sharedPrefPath(QString file = nullptr);
    static QString tempPath(QString file = nullptr);
    static QString output(QString file = nullptr);
    static QString thumbsPath(QString file = nullptr);
    static QString systemFontPath(QString file = nullptr);
    static QString skymedia(QString file = nullptr);
    static QString crashPath(QString file = nullptr);
    static QString logPath(QString file = nullptr);
    static QString animationsPath(QString file = nullptr);
    static QString bundleFontPath();
    static QString mkPathIfNeed(QString dir);
    static QString aescript();

};
#endif // AppPath_H
