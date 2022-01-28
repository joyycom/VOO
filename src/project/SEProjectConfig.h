#ifndef SEPROJECTCONFIG_H
#define SEPROJECTCONFIG_H

#include <QString>

#define SKYTIMELINE_SUFFIX "sky"
class SEProjectConfig {
public:
    SEProjectConfig(QString root_dir, QString skyFilePath);

    QString size() const;
    QString duration() const;
    void save();

private:
    QString m_size;
    QString m_duration;
    //    qint64 dirSize(QString dirPath);
    //    QString formatSize(qint64 size);
};

#endif // SEPROJECTCONFIG_H
