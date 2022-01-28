#ifndef EXPORTDIALOGDATASOURCE_H
#define EXPORTDIALOGDATASOURCE_H

#include <QObject>
#include <QUrl>

#include "project/SkyPack.h"
#include "base/utils/SharedPref.h"
#define FORCE_POSIX
#include "thirds/mmkv/MMKV.h"

class ExportDialogDataSource : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString packSize NOTIFY packSizeChanged)
    Q_PROPERTY(double exportProgress NOTIFY exportProgressChanged)
    Q_PROPERTY(double exportFinish NOTIFY exportFinishChanged)
public:
    explicit ExportDialogDataSource(QObject *parent = nullptr);
    ~ExportDialogDataSource();

    static bool isPacking();

    Q_INVOKABLE void onQmlLoaded();

    Q_INVOKABLE void uploadLocalCover(QString coverPath);

    Q_INVOKABLE void uploadTimelineCover(int coverType);

    Q_INVOKABLE void setOutput(QString output);

    Q_INVOKABLE QString getOutput();

    Q_INVOKABLE void setVideoName(QString videoName);

    Q_INVOKABLE QString getVideoName();

    Q_INVOKABLE void setCoverType(int coverType);

    Q_INVOKABLE int getCoverType();

    Q_INVOKABLE void setCoverPath(QString coverPath);

    Q_INVOKABLE QString getCoverPath();

    Q_INVOKABLE void setOutputPackCheck(bool checked);

    Q_INVOKABLE bool getOutputPackCheck();

    Q_INVOKABLE void setOutputVMSCheck(bool checked);

    Q_INVOKABLE bool getOutputVMSCheck();

    Q_INVOKABLE void setTemplateOverwriteCheck(bool checked);

    Q_INVOKABLE bool getTemplateOverwriteCheck();

    Q_INVOKABLE void setTemplateCnName(QString cnName);

    Q_INVOKABLE QString getTemplateCnName();

    Q_INVOKABLE void setTemplateEnName(QString enName);

    Q_INVOKABLE QString getTemplateEnName();

    Q_INVOKABLE QVariantList getTemplateTypeData();

    Q_INVOKABLE QVariantList getTemplateTeamData();

    Q_INVOKABLE void setTemplateType(int index);

    Q_INVOKABLE int getTemplateType();

    Q_INVOKABLE void setTemplateTeam(int index);

    Q_INVOKABLE int getTemplateTeam();

    Q_INVOKABLE QString optimizeOutputFolder(QString outputAddress, QString outputFolder);

    Q_INVOKABLE QString getTitle();

    Q_INVOKABLE double getDuration();

    Q_INVOKABLE QString getBitRate();

    Q_INVOKABLE int getFps();

    Q_INVOKABLE QString getFrame();

    Q_INVOKABLE void updatePackSize();

    Q_INVOKABLE void cancel();

    Q_INVOKABLE bool hasClip();

    Q_INVOKABLE QVariantList inputDeviceList();

    Q_INVOKABLE QString getFileNameFromId(qint64 id);

    Q_INVOKABLE QString getJsonFromId(qint64 id);

    Q_INVOKABLE void startExport(QString savePath,
                                 QString videoName,
                                 int coverType,
                                 bool exportTemplate,
                                 bool doContribute,
                                 QVariantList inputDeviceList,
                                 QString templateCnName,
                                 QString templateEnName,
                                 int templateType,
                                 int templateTeam,
                                 bool templateOverwrite,
                                 QString eCommerceCfgPath);

    Q_INVOKABLE QString getE_CommerceCfgPath();

    Q_INVOKABLE void setE_CommerceCfgPath(QString cfgDirPath);

    Q_INVOKABLE QStringList getExportLogs() {
        return exportLogs;
    }

signals:
    void packSizeChanged(QString packSize);
    void coverChanged(QString cover);
    void exportProgressChanged(double progress);
    void contributeProgressChanged(double progress);
    void exportCancel();
    void exportError();
    void exportFinishChanged(QString outputDir);
    void updateCover(int coverType, QString coverPath);
    void templateTypeChanged(QVariantList list);
    void templateTeamChanged(QVariantList list);
public slots:
    void onExportLogInfo(int type, QString log);

private:
    void getHttpData();
    QString getCover();
    void removeTempFile();
    void uploadPack(QString savePath,
                    QString templateFile,
                    QString videoFile,
                    QJsonArray inputList,
                    QString zhName,
                    QString enName,
                    int templateType,
                    QString teamId,
                    bool overwrite);
    void contribute(QString savePath,
                    QString packUrl,
                    QString coverUrl,
                    QString videoUrl,
                    QJsonArray inputList,
                    QString zhName,
                    QString enName,
                    int templateType,
                    QString teamId,
                    bool overwrite);

private:
    SharedPref *m_sharedPref = nullptr;
    SkyPack *m_skyPack = nullptr;
    bool m_lastCover = false;
    bool m_lastVideo = false;
    bool m_lastPack = false;
    bool m_cancel = false;
    QString m_coverPath;
    QJsonArray m_inputList;
    QList<QString> teamIds;
    QList<InputDevice> m_inputDeviceList;
    QStringList exportLogs;
    QString m_packUrl, m_coverUrl, m_videoUrl;
    qint64 m_bytesSent1 = 0, m_bytesSent2 = 0, m_bytesSent3 = 0;
    int m_contributeUploadFlag = 0;
};

#endif // EXPORTDIALOGDATASOURCE_H
