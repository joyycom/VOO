#ifndef GuideDataSource_h
#define GuideDataSource_h

#include <QObject>
#include "ProjectHistoryModel.h"

class GuideDataSource : public QObject {
    Q_OBJECT
public:
    explicit GuideDataSource(QObject *parent = nullptr);
    ~GuideDataSource();

signals:
    void historyListChanged(ProjectHistoryListModel *data);
    void showGuide();
    void hideGuide();
    void showEdit();
    void exitApp();
    void showToast(QString toast);



public slots:
    void loadData();
    void newProject();
    void openProjFile(QVariant proj);
    void openProjFile(QString projFile);
    void openProject(int index);
    void copyProject(int index);
    void renameProject(int index, QString newName);
    void updateConfig(QVariant localPath, QVariant save = true, QVariant title = "");
    void removeProject(QList<int> choosedIndex);
    int projectHistoryCount();
    void setEditorPage(QObject *object);
    void onObjectCreated(QObject *object, const QUrl &url);
    void onEditPageRelease(QObject *object);

    void importProjFromAE();

    void onImportResultFromAE(int code,QString msg,QString filePath);

    ProjectHistoryListModel *projectHistorys();

private:
    ProjectHistoryListModel *m_projectHistoryListModel = new ProjectHistoryListModel;
    ProjectHistoryModel *m_lastModel = nullptr;
    void storeHistory();
    void showEditPage();
};

#endif
