#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QDesktopServices>
#include <QProcess>
#include <QString>
#include <QFile>
#include <QUrl>
#include <QJsonArray>

class FileUtils : public QObject {

public:
    static QString read(const QString &filepath);
    static QString readJson(const QString &path);
    static void write(const QString &path, const QString &content);
    static bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);
    static bool copyFile(const QString &fromFile, const QString &toFile, bool coverFileIfExist);
    static QString getFileExtension(const QString &filePath);
    static void selectFile(const QString &path);
    static void openDir(const QString &path);
    static QString getFileAbsolutePath(const QString &baseDirectory, const QString &path);
    static QString realPath(const QString &baseDir, QString &path);

    static bool isVideoExtension(const QString &path);

    static bool isImageExtension(const QString &path);

    static bool isAudioExtension(const QString &path);

    static void makeFileReadable(QFile &file);

    static QString getFileMd5(const QString &path);

    static void makeFileReadable(const QString &path);

    static QJsonArray readCsvFile(const QString &filePath);

private:
    static QStringList parseCSVLine(const QString &string);
};

class QmlFileUtils : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE bool isVideoExtension(const QString &path);

    Q_INVOKABLE bool isImageExtension(const QString &path);

    Q_INVOKABLE bool isAudioExtension(const QString &path);

    Q_INVOKABLE bool isEffectsFolder(const QString &path);

    Q_INVOKABLE QString urlToFile(const QUrl &url);

    Q_INVOKABLE QString toNativeSeparators(const QString &path);

    Q_INVOKABLE bool exists(const QString &path);

    Q_INVOKABLE bool makeDir(const QString &path);

    Q_INVOKABLE void openDir(const QString &path);

    Q_INVOKABLE double getMediaDuration(const QString &path);
};

#endif // FILEUTILS_H
