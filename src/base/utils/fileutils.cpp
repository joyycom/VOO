#include "fileutils.h"

#include <QCryptographicHash>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QTextStream>
#include <skymedia/SkyResource.h>

QString FileUtils::read(const QString &filepath) {
    QString content = "";
    QFile file;
    file.setFileName(filepath);
    makeFileReadable(file);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    content = QString::fromUtf8(file.readAll());
    file.close();
    return content;
}

QString FileUtils::readJson(const QString &path) {
    QString filepath = QString(path);
    QString content = "{}";
    QFile file;
    file.setFileName(filepath);
    makeFileReadable(file);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        content = QString::fromUtf8(file.readAll());
    }
    file.close();
    return content;
}

void FileUtils::makeFileReadable(QFile &file) {
    auto p = file.permissions();
    auto readPermssions =
        QFileDevice::ReadUser | QFileDevice::ReadGroup | QFileDevice::ReadOwner | QFileDevice::ReadOther;
    if ((p & readPermssions) != readPermssions) {
        auto newPermission = p | readPermssions;
        qDebug() << "FileUtils filePath:" << file.fileName() << " permission:" << p << " -> " << newPermission;
        file.setPermissions(newPermission);
    }
    //    if(file.open(QFile::ReadOnly)){
    //        file.close();
    //    }else{
    //        qDebug()<<"FileUtils open File:"<<file.fileName()<<" permission:"<<p<<" -> "<<file.errorString();
    //    }
}

void FileUtils::makeFileReadable(const QString &path) {
    QFileInfo fileInfo(path);
    if (fileInfo.isDir()) {
        QDir dir(path);
        QFileInfoList localEntryInfoList = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
        for (int i = 0; i < localEntryInfoList.size(); ++i) {
            auto filePath = localEntryInfoList.at(i).absoluteFilePath();
            makeFileReadable(filePath);
        }
    } else {
        QFile file(path);
        makeFileReadable(file);
    }
}

void FileUtils::write(const QString &path, const QString &content) {
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(content.toUtf8());
        file.close();
    } else {
        qDebug() << "FileUtils::write fail! path is :" << path;
    }
}

bool FileUtils::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist) {
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if (!targetDir.exists()) { /**< 如果目标目录不存在，则进行创建 */
        if (!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach (QFileInfo fileInfo, fileInfoList) {
        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if (fileInfo.isDir()) { /**< 当为目录时，递归的进行copy */
            if (!copyDirectoryFiles(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()), coverFileIfExist))
                return false;
        } else { /**< 当允许覆盖操作时，将旧文件进行删除操作 */
            if (coverFileIfExist && targetDir.exists(fileInfo.fileName())) {
                targetDir.remove(fileInfo.fileName());
            }

            /// 进行文件copy
            if (!QFile::copy(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()))) {
                return false;
            }
        }
    }
    return true;
}

bool FileUtils::copyFile(const QString &fromFile, const QString &toFile, bool coverFileIfExist) {
    QFileInfo fromFileInfo(fromFile);
    QFileInfo toFileInfo(toFile);
    if (!fromFileInfo.exists()) {
        return false;
    }
    if (toFileInfo.exists()) {
        if (coverFileIfExist) {
            QFile::remove(toFile);
        } else {
            return false;
        }
    }
    QDir dir;
    if (!dir.exists(toFileInfo.path())) {
        dir.mkpath(toFileInfo.path());
    }
    return QFile::copy(fromFile, toFile);
}

QString FileUtils::getFileExtension(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    return fileInfo.suffix();
}

void FileUtils::selectFile(const QString &path) {
#if defined(Q_OS_WINDOWS)
    const QString explorer = "explorer";
    QStringList param;
    if (!QFileInfo(path).isDir()) {
        param << QLatin1String("/select,");
    }
    param << QDir::toNativeSeparators(path);
    QProcess::startDetached(explorer, param);
#elif defined(Q_OS_MAC)
    QStringList scriptArgs;
    scriptArgs << QLatin1String("-e")
               << QString::fromLatin1("tell application \"Finder\" to reveal POSIX file \"%1\"").arg(path);
    QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
    scriptArgs.clear();
    scriptArgs << QLatin1String("-e") << QLatin1String("tell application \"Finder\" to activate");
    QProcess::execute("/usr/bin/osascript", scriptArgs);
#endif
}

void FileUtils::openDir(const QString &path) {
    QString dir = QDir::toNativeSeparators(path);
#if defined(Q_OS_WINDOWS)
    QProcess::startDetached("explorer " + dir);
#elif defined(Q_OS_MAC)
    QDesktopServices::openUrl(QUrl::fromLocalFile(dir));
#endif
}

QString FileUtils::getFileAbsolutePath(const QString &baseDirectory, const QString &path) {
    QUrl url(path);
    if (url.isRelative()) {
        QFileInfo fileInfo(baseDirectory, path);
        return QDir::toNativeSeparators(fileInfo.absoluteFilePath());
    }
    return path;
}

QString FileUtils::realPath(const QString &baseDir, QString &path) {
    QUrl url(path);
    if (url.isRelative()) {
        return FileUtils::getFileAbsolutePath(baseDir, path);
    } else {
        if (path.startsWith("bundle://")) {
            return QFileInfo(baseDir, path.replace("bundle://", "")).filePath();
        } else {
            return path;
        }
    }
}

bool FileUtils::isVideoExtension(const QString &path) {
    QString videoExtensions[] = {
        ".mp4", ".avi", ".m4v", ".mkv", ".mpg", ".f4v", ".flv", ".rmvb", ".mov", ".rm", ".wmv", ".3gp"};
    for (const QString &extension : videoExtensions) {
        if (path.endsWith(extension, Qt::CaseInsensitive)) {
            return true;
        }
    }
    return false;
}

bool FileUtils::isAudioExtension(const QString &path) {
    QString extensions[] = {".mp3", ".wma", ".aac", ".ape", ".flac", ".wav"};
    for (const QString &extension : extensions) {
        if (path.endsWith(extension, Qt::CaseInsensitive)) {
            return true;
        }
    }
    return false;
}

bool FileUtils::isImageExtension(const QString &path) {
    QString extensions[] = {".jpg", ".jpeg", ".webp", ".png", ".bmp", ".tiff", ".gif"};
    for (const QString &extension : extensions) {
        if (path.endsWith(extension, Qt::CaseInsensitive)) {
            return true;
        }
    }
    return false;
}

QString FileUtils::getFileMd5(const QString &path) {
    QFile sourceFile(path);
    qint64 fileSize = sourceFile.size();
    const qint64 bufferSize = 10240;

    if (sourceFile.open(QIODevice::ReadOnly)) {
        char buffer[bufferSize];
        int bytesRead;
        int readSize = qMin(fileSize, bufferSize);

        QCryptographicHash hash(QCryptographicHash::Md5);

        while (readSize > 0 && (bytesRead = sourceFile.read(buffer, readSize)) > 0) {
            fileSize -= bytesRead;
            hash.addData(buffer, bytesRead);
            readSize = qMin(fileSize, bufferSize);
        }

        sourceFile.close();
        return QString(hash.result().toHex());
    }
    return QString();
}

bool QmlFileUtils::isVideoExtension(const QString &path) {
    return FileUtils::isVideoExtension(path);
}

bool QmlFileUtils::isAudioExtension(const QString &path) {
    return FileUtils::isAudioExtension(path);
}

bool QmlFileUtils::isEffectsFolder(const QString &path) {
    QUrl url(path);
    QFileInfo dirInfo(url.toLocalFile());
    if (!dirInfo.isDir()) {
        return false;
    }
    QDir dir(url.toLocalFile());
    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.count(); i++) {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.suffix() == "ofeffect") {
            return true;
        }
    }
    return false;
}

bool QmlFileUtils::isImageExtension(const QString &path) {
    return FileUtils::isImageExtension(path);
}

QString QmlFileUtils::urlToFile(const QUrl &url) {
    return toNativeSeparators(url.toLocalFile());
}

QString QmlFileUtils::toNativeSeparators(const QString &path) {
    return QDir::toNativeSeparators(path).replace(QString("") + QDir::separator() + QDir::separator(),
                                                  QDir::separator());
}

bool QmlFileUtils::exists(const QString &path) {
    return QFile::exists(path);
}

bool QmlFileUtils::makeDir(const QString &path) {
    QDir dir(path);
    if (dir.exists()) {
        return true;
    }
    return dir.mkpath(path);
}

void QmlFileUtils::openDir(const QString &path) {
    FileUtils::openDir(path);
}

double QmlFileUtils::getMediaDuration(const QString &path) {
    skymedia::SkyResource res(path.toUtf8().data());
    return res.getDuration();
}

QJsonArray FileUtils::readCsvFile(const QString &filePath) {
    QString content = read(filePath);
    if (content == "") {
        return QJsonArray();
    }
    QStringList contentArr = content.split("\n", QString::SkipEmptyParts);

    QJsonArray result;
    QStringList list;
    QStringList titles = parseCSVLine(contentArr.at(0));
    for (int i = 1; i < contentArr.length(); i++) {
        list = parseCSVLine(contentArr.at(i));
        QJsonObject obj;
        for (int j = 0; j < titles.length() && j < list.length(); ++j) {
            obj.insert(titles.at(j), list.at(j));
        }
        result.append(obj);
    }
    return result;
}

QStringList FileUtils::parseCSVLine(const QString &string) {
    enum State { Normal, Quote } state = Normal;
    QStringList fields;
    QString value;

    for (int i = 0; i < string.size(); i++) {
        const QChar current = string.at(i);

        // Normal state
        if (state == Normal) {
            // Comma
            if (current == ',') {
                // Save field
                fields.append(value.trimmed());
                value.clear();
            } else if (current == '"') {
                state = Quote;
                value += current;
            } else
                value += current;
        } else if (state == Quote) {
            // Another double-quote
            if (current == '"') {
                if (i < string.size()) {
                    // A double double-quote?
                    if (i + 1 < string.size() && string.at(i + 1) == '"') {
                        value += '"';
                        // Skip a second quote character in a row
                        i++;
                    } else {
                        state = Normal;
                        value += '"';
                    }
                }
            } else
                value += current;
        }
    }

    if (!value.isEmpty())
        fields.append(value.trimmed());

    // Quotes are left in until here; so when fields are trimmed, only whitespace outside of
    // quotes is removed.  The quotes are removed here.
    for (int i = 0; i < fields.size(); ++i)
        if (fields[i].length() >= 1 && fields[i].left(1) == '"') {
            fields[i] = fields[i].mid(1);
            if (fields[i].length() >= 1 && fields[i].right(1) == '"')
                fields[i] = fields[i].left(fields[i].length() - 1);
        }
    return fields;
}
