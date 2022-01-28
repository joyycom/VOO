#include "AppPath.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>

#include "fileutils.h"

QString AppPath::mkPathIfNeed(QString dir) {
    QDir cacheDir(dir);
    if (!cacheDir.exists()) {
        cacheDir.mkpath(dir);
    }
    return dir;
}

QString AppPath::rootPath(QString file) {
    QString path = mkPathIfNeed(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if (file == nullptr) {
        return path;
    } else {
        return QFileInfo(path, file).filePath();
    }
}

QString AppPath::skymedia(QString file) {
    QString path = mkPathIfNeed(rootPath(".skymedia"));
    if (file == nullptr) {
        return path;
    } else {
        return QFileInfo(path, file).filePath();
    }
}

QString AppPath::tempPath(QString file) {
    QString path = mkPathIfNeed(rootPath(".temps"));
    if (file == nullptr) {
        return path;
    } else {
        return QFileInfo(path, file).filePath();
    }
}

QString AppPath::output(QString file) {
    QString path = mkPathIfNeed(rootPath("output"));
    if (file == nullptr) {
        return path;
    } else {
        return QFileInfo(path, file).filePath();
    }
}

QString AppPath::thumbsPath(QString file) {
    QString path = mkPathIfNeed(rootPath(".thumbs"));
    if (file == nullptr) {
        return path;
    } else {
        return QFileInfo(path, file).filePath();
    }
}

QString AppPath::crashPath(QString file) {
    QString path = mkPathIfNeed(rootPath(".crashes"));
    if (file == nullptr) {
        return path;
    } else {
        return QFileInfo(path, file).filePath();
    }
}

QString AppPath::materialPath(QString file) {
    QString path = mkPathIfNeed(rootPath("materials"));
    if (file == nullptr) {
        return path;
    } else {
        return QFileInfo(path, file).filePath();
    }
}

QString AppPath::tempProjectPath(QString file) {
    QString path = mkPathIfNeed(rootPath("tmp_project"));
    if (file == nullptr) {
        return path;
    } else {
        return QFileInfo(path, file).filePath();
    }
}

QString AppPath::sharedPrefPath(QString file) {
    QString path = mkPathIfNeed(rootPath("shared_pref"));
    if (file == nullptr) {
        return path;
    } else {
        return QFileInfo(path, file).filePath();
    }
}

QString AppPath::logPath(QString file) {
    QString path = mkPathIfNeed(rootPath("log"));
    if (file == nullptr) {
        return path;
    } else {
        return QFileInfo(path, file).filePath();
    }
}

QString AppPath::aescript(){
    return mkPathIfNeed(rootPath("aescript"));
}

QString AppPath::systemFontPath(QString file) {
    QString path = QFileInfo(bundleFontPath(), "SystemFont").filePath();
    if (file == nullptr) {
        return path;
    } else {
        return QFileInfo(path, file).filePath();
    }
}

QString AppPath::animationsPath(QString file) {
    QString path = mkPathIfNeed(rootPath("animations"));
    if (file == nullptr) {
        return path;
    } else {
        return QFileInfo(path, file).filePath();
    }
}

QString AppPath::bundleFontPath() {
    QString fontsDir = rootPath().append("/fonts/");
    QDir dir(fontsDir);
    if (!dir.exists()) {
        dir.mkpath(fontsDir);
        dir.mkdir(fontsDir + "SystemFont/");
        QString appFonts = QCoreApplication::applicationDirPath().append("/fonts/SystemFont");
        FileUtils::copyDirectoryFiles(appFonts, fontsDir + "SystemFont/", true);
    }
    return fontsDir;
}
