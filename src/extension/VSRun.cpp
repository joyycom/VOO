#include "VSRun.h"

#include <QSettings>
#include <QDir>
#include <QUrl>
#include <QVersionNumber>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

#include "base/utils/threadpools.h"

void VSRun::run(const QStringList &arguments, const QStringList &extensions, std::function<void(const bool)> callback) {
    ThreadPools::instance()->exec([=]() {
#ifdef Q_OS_WIN
        QSettings vscodeSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{"
                                 "771FD6B0-FA20-440A-A002-3B3BAC16DC50}_is1",
                                 QSettings::NativeFormat);
        QString installLocation = vscodeSettings.value("InstallLocation").toString();
        QFileInfo vscodeProgram(installLocation, "bin/code.cmd");
#else
        QString installLocation = "/Applications/Visual Studio Code.app/Contents/Resources/app";
        QFileInfo vscodeProgram(installLocation, "bin/code");
#endif
        if (vscodeProgram.exists()) {
            QProcess process;
            QString program = vscodeProgram.filePath();
            process.setProgram(program);
            //获取VOO扩展，判断是否要更新
            process.setArguments(QStringList() << "--show-versions"
                                               << "--list-extensions");
            process.start();
            process.waitForStarted();
            process.waitForReadyRead();
            process.waitForFinished();
            QString cmdContent = QString::fromLocal8Bit(process.readAll());
            for (int index = 0; index < extensions.length(); index++) {
                QString extension = extensions.at(index);
                QString extensionName = extension.left(extension.lastIndexOf("-"));
                bool isInstall = cmdContent.contains("." + extensionName + "@");
                if (!isInstall) {
                    updateExtension(process, extension, "");
                } else {
                    QStringList contentList = cmdContent.split("\n");
                    for (int i = 0; i < contentList.size(); i++) {
                        QString line = contentList.at(i);
                        if (line.contains("." + extensionName + "@")) {
                            QString version = line.split("@").at(1);
                            updateExtension(process, extension, version);
                            break;
                        }
                    }
                }
            }
            //启动VSCode
            process.setArguments(arguments);
            process.start();
            process.waitForStarted();
            process.waitForReadyRead();
            process.waitForFinished();
            if (callback) {
                callback(true);
            }
        } else {
            if (callback) {
                callback(false);
            }
        }
    });
}

void VSRun::updateExtension(QProcess &process, const QString &extension, const QString &version) {
    int startIndex = extension.lastIndexOf("-") + 1;
    int endIndex = extension.lastIndexOf(".");
    QString fileVersion = extension.mid(startIndex, endIndex - startIndex);
    bool update = QVersionNumber::compare(QVersionNumber::fromString(fileVersion), QVersionNumber::fromString(version));
    if (update) {
        //更新VOO扩展
        QString extensionPath =
            QFileInfo(QCoreApplication::applicationDirPath(), "vscode_extensions/" + extension).filePath();
        process.setArguments(QStringList() << "--install-extension" << extensionPath);
        process.start();
        process.waitForStarted();
        process.waitForReadyRead();
        process.waitForFinished();
    }
}
