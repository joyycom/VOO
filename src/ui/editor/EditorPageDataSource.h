#ifndef EditorPageDataSource_h
#define EditorPageDataSource_h

#include <QObject>
#include <QTimer>

const static int ExportVideoInit = 1;
const static int ExportVideoProgress = 2;
const static int ExportVideoFinish = 3;
const static int ExportVideoCancel = 4;
const static int ExportVideoError = 99;

class SEProject;
class EditorPageDataSource : public QObject {
    Q_OBJECT
public:
    explicit EditorPageDataSource(QObject *parent = nullptr);
    ~EditorPageDataSource();

    enum ProgressDialogType { TimelineRewindProgressDialog = 1 };

    Q_INVOKABLE void onLoadCompleted();

    Q_INVOKABLE void cancelProgressDialog(int type);

    void notifyShowProgressDialog();

    void notifyProgressUpdate(double progress);

    void notifyHideProgressDialog();

    Q_INVOKABLE QString appVersion();

    Q_INVOKABLE void checkVersionUpdate();

    Q_INVOKABLE void showVideoPreviewDialog();

    Q_INVOKABLE void importFromAE();

    Q_INVOKABLE void setTitle(QString title);

    Q_INVOKABLE bool hasClip();

    Q_INVOKABLE bool isTempProject();

    Q_INVOKABLE void openThirdLicense();

signals:
    void autoSaveSuccess(QString tips);

    void showProgressDialog();

    void updateProgressDialog(double progress);

    void hideProgressDialog();

    void versionUpdateResult(bool update, bool force, QString version, QString desc, QString downloadUrl);

    void videoPreviewWindowCreated(QObject* window);
    void videoPreviewWindowDestroyed();
    void miniPlayWindowCreated(QObject* window);
    void miniPlayWindowDestroyed();

    void closeEditorPage();

public slots:
    bool canShowLab();
    QString currentProjectTitle();
    void save(bool quit = false);
    void saveAs(QUrl fileUrl);
    QString localPath();
    void pack();
    void undo();
    void redo();
    void onTimerFired();
    void onObjectCreated(QObject *object, const QUrl &url);
    void onVideoPreviewDialogRelease(QObject *object);
    void onMiniPlayerDialogRelease(QObject *object);

private:
    void initAutoSaveTimer();
    void findVideoPreviewWindow(QObject *object);
    QTimer *m_autoSaveTimer = nullptr;
    SEProject *m_currentProject;
    QString m_localPath;
};

#endif
