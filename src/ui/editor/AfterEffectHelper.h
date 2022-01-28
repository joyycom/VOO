#ifndef AfterEffectHelper_H
#define AfterEffectHelper_H

#include <QFileSystemWatcher>
#include <QString>
#include <QTimer>



class AfterEffectHelper:public QObject
{
    Q_OBJECT
public:

    static AfterEffectHelper* instance();

    bool importFromAfterFx();

signals:
    void importFromAeResult(const int code,const QString& msg, const QString& targetFile);

public slots:
    void checkResultFileExits();

private:
    AfterEffectHelper();

    ~AfterEffectHelper();

    void saveExportScriptConfig();

    void copyAeScriptToTargetDir();


    void runAEScript(QString exePath);

    QString findRunningAfterFxPath();

    void startWatchResultFile();

    void stopWatchResultFile();

    bool m_isImportFromAE = false;

    QTimer * m_timer = nullptr;

};

#endif // AfterEffectHelper_H
