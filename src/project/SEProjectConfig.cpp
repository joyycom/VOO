#include "SEProjectConfig.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QDebug>
#include "src/base/utils/fileutils.h"

static qint64 dirSize(QString dirPath) {
    qint64 size = 0;
    QDir dir(dirPath);
    // calculate total size of current directories' files
    QDir::Filters fileFilters = QDir::Files | QDir::System | QDir::Hidden;
    for (QString filePath : dir.entryList(fileFilters)) {
        QFileInfo fi(dir, filePath);
        size += fi.size();
    }
    // add size of child directories recursively
    QDir::Filters dirFilters = QDir::Dirs | QDir::NoDotAndDotDot | QDir::System | QDir::Hidden;
    for (QString childDirPath : dir.entryList(dirFilters))
        size += dirSize(dirPath + QDir::separator() + childDirPath);
    return size;
}

static QString formatSize(qint64 size) {
    QStringList units = {"B", "KB", "MB", "GB", "TB", "PB"};
    int i;
    double outputSize = size;
    for (i = 0; i < units.size() - 1; i++) {
        if (outputSize < 1024)
            break;
        outputSize = outputSize / 1024;
    }
    return QString("%0%1").arg(outputSize, 0, 'f', 0).arg(units[i]);
}

SEProjectConfig::SEProjectConfig(QString root_dir, QString skyFilePath) {
    QDir dir(root_dir);
    if (!dir.exists()) {
        qInfo("open root_dir fail, not found!");
        return;
    }
    QFileInfo skyFile(skyFilePath);
    if (skyFile.exists()) {
        QString context = FileUtils::readJson(skyFile.absoluteFilePath());
        auto jsonObject = QJsonDocument::fromJson(context.toUtf8()).object();
        auto timelineConfig = jsonObject.value("timeline").toObject();
        double audioDuration = timelineConfig.value("audioDuration").toDouble();
        double videoDuration = timelineConfig.value("videoDuration").toDouble();
        QString timeFormat = "mm:ss";
        if (videoDuration > 3600) {
            timeFormat = "hh:mm:ss";
        }
        m_duration = QDateTime::fromTime_t(fmax(audioDuration, videoDuration)).toUTC().toString(timeFormat);
    } else {
        m_duration = "00:00";
    }

    m_size = formatSize(dirSize(root_dir));
}

void SEProjectConfig::save() {
}

QString SEProjectConfig::size() const {
    return m_size;
}
QString SEProjectConfig::duration() const {
    return m_duration;
}
