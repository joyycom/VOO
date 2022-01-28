#include "ZipUtils.h"

#include <QDir>
#include <QCoreApplication>
#include <QDebug>
#include <QProcess>
#include <JlCompress.h>

#include <thirds/quazip/quazip.h>
#include <thirds/quazip/quazipfile.h>

bool ZipUtils::decompressFromZip(QString zipFile, QString outputDir) {
    QFile zip(zipFile);
    auto list = JlCompress::extractDir(&zip, outputDir);
    qDebug() << "decompressFromZip zip:" << zipFile << " Dest:" << outputDir << " Result:" << list.size();
    if (list.size() == 0) {
        return decompressFromZipBySystem(zipFile, outputDir);
    } else {
        return true;
    }
}

QString ZipUtils::compressFolder(QString dir, QString zipFilePath) {
    bool result = JlCompress::compressDir(zipFilePath, dir, true);
    if (result) {
        return zipFilePath;
    } else {
        qDebug() << "compressFolder Dir:" << dir << " DestZip:" << zipFilePath << " Result:" << result;
        return compressFolderBySystem(dir, zipFilePath);
    }
}

QString ZipUtils::compressFile(QString filePath, QString zipFilePath) {
    bool result = JlCompress::compressFile(zipFilePath, filePath);
    if (result) {
        return zipFilePath;
    } else {
        qDebug() << "compressFolder Dir:" << filePath << " DestZip:" << zipFilePath << " Result:" << result;
        return compressFolderBySystem(filePath, zipFilePath);
    }
}

bool ZipUtils::decompressFromZipBySystem(QString zipFile, QString outputDir) {
    QString s;
#ifdef Q_OS_WIN
    s = QCoreApplication::applicationDirPath() + "/7za.exe x \"" + zipFile + "\" -y -aos -o\"" + outputDir + "\"";
    qDebug() << "sdfksdklfsdjf -> " << s;
#else
    s = "/usr/bin/unzip -o \"" + zipFile + "\" -d \"" + outputDir + "\"";
#endif
    QProcess *proc = new QProcess();
    proc->start(s);
    proc->waitForStarted();
    proc->waitForFinished();
    QString out = QString::fromLocal8Bit(proc->readAllStandardOutput());
    qDebug() << "unzip result=" << out;
#ifdef Q_OS_WIN
    return out.contains("Everything is Ok");
#else
    return out.contains("Archive:");
#endif
}

QString ZipUtils::compressFolderBySystem(QString dir, QString zipFilePath) {
    QString s;
#ifdef Q_OS_WIN
    s = QCoreApplication::applicationDirPath() + "/7za.exe a \"" + zipFilePath + "\" \"" + dir + "\\*\"";
    qDebug() << "sdfksdklfsdjf -> " << s;
#else
    s = "/usr/bin/zip -q -r \"" + zipFilePath + "\" ./";
#endif
    QProcess *proc = new QProcess();
    proc->setWorkingDirectory(dir);
    proc->start(s);
    proc->waitForStarted();
    proc->waitForFinished();
    QString out = QString::fromLocal8Bit(proc->readAllStandardOutput());
    qDebug() << "zip result=" << out;
#ifdef Q_OS_WIN
    return out.contains("Everything is Ok") ? zipFilePath : "";
#else
    return out.contains("Archive:") || out.length() == 0 ? zipFilePath : "";
#endif
}
