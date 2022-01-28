#ifndef SEPROJECTPROJ_H
#define SEPROJECTPROJ_H

#include <QDateTime>
#include <QJsonObject>

#define PROJ_SUFFIX "proj"

enum ProjType { Timeline, Effect, File, UnKnow };
static const char *ProjType_str[] = {"Timeline", "Effect", "File"};

class SEProjectProj {
public:
    SEProjectProj(QString projectFile);
    void save(QString root_dir);
    void realSave(QString root_dir);
    ProjType projTypeWithString(QString c);
    QString stringWithProjType(ProjType projType);
    static void createFile(QString projectFile);
    QString entryFile();

    QString id() const;
    QString title() const;
    QString remoteId() const;
    QString version() const;
    QString anchor() const;
    QString summary() const;
    ProjType type() const;
    QString thumb() const;
    QString inputList() const;
    QString inputConfig() const;
    QDateTime createTimestamp() const;
    QDateTime lastChangedTimestamp() const;
    QString projectDir() const;
    bool isDebug() const;
    bool isTemp() const;
    QJsonValue getExtraData(const QString& key) const;

    void setTitle(QString title);
    void setAnchor(QString s);
    void setSummary(QString s);
    void setType(ProjType t);
    void setThumb(QString s);
    void setFile(QString filePath);
    void setRemoteId(QString remoteId);
    void updateCreateTimestamp();
    void setExtraData(const QString& key,const QJsonValue& value);
    void setIsTemp(bool isTemp);

    static QString projFilePath(QString root_dir);
    static QString findProjectFile(QString root_dir);

private:
    QString m_id = "";
    QString m_title;
    QString m_remoteId;
    QString m_version;
    QString m_anchor;
    QString m_summary;
    ProjType m_type;
    QString m_thumb;
    QString m_inputList;
    QString m_skyFile;
    QString m_ofFile;
    QString m_resFile;
    QString m_inputConfig;
    QDateTime m_createTimestamp;
    QDateTime m_lastChangedTimestamp;
    QJsonObject m_extraData;
    bool m_isDebug = false;
    bool m_isTemp = false;
    QString m_rootPath;
};

#endif // SEPROJECTPROJ_H
