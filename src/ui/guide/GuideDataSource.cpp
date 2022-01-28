#include "GuideDataSource.h"

#include <QDebug>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include <QUrl>
#include <QGuiApplication>

#include "project/SEProject.h"
#include "base/http/BaseRsp.h"
#include "base/http/SEHttpClient.h"
#include "base/utils/SharedPref.h"
#include "base/utils/fileutils.h"
#include "base/utils/threadpools.h"
#include "base/utils/qmlutilities.h"
#include "timeline/venus/VenusManager.h"
#include "extension/VooExtension.h"
#include "ui/editor/AfterEffectHelper.h"

GuideDataSource::GuideDataSource(QObject* parent) : QObject(parent) {
    VooExtension::windowChanged(Guide);
    QJsonArray jo = SharedPref::instance()->readJsonArray("cache_history_project");
    for (int i = 0; i < jo.size(); i++) {
        ProjectHistoryModel* model = new ProjectHistoryModel(jo.at(i).toObject());
        m_projectHistoryListModel->addDataToEnd(model);
    }

    connect(QmlUtilities::sharedEngine(), &QQmlApplicationEngine::objectCreated, this,
            &GuideDataSource::onObjectCreated);

    AfterEffectHelper* helper = AfterEffectHelper::instance();
    connect(helper, &AfterEffectHelper::importFromAeResult, this,
            &GuideDataSource::onImportResultFromAE);

    QStringList arguments = QGuiApplication::arguments();
    if(arguments.size() > 1) {
        QString projFile = arguments.at(1);
        openProjFile(projFile);
    }
}

GuideDataSource::~GuideDataSource() {
    VooExtension::windowChanged(Null);
    disconnect(QmlUtilities::sharedEngine(), &QQmlApplicationEngine::objectCreated, this,
               &GuideDataSource::onObjectCreated);
}

ProjectHistoryListModel *GuideDataSource::projectHistorys() {
    return m_projectHistoryListModel;
}

void GuideDataSource::loadData() {
    emit historyListChanged(m_projectHistoryListModel);
}

void GuideDataSource::updateConfig(QVariant localPath, QVariant save, QVariant title) {
    QString projectFile = SEProjectProj::findProjectFile(localPath.toString());
    ProjectHistoryModel *model = new ProjectHistoryModel(projectFile);
    if (!title.isNull()) {
        model->setName(title.toString());
    }
    if (save.isNull() || save.toBool()) {
        m_projectHistoryListModel->replace(m_lastModel, model);
    } else {
        m_lastModel->remove();
        int index = m_projectHistoryListModel->indexOf(m_lastModel);
        m_projectHistoryListModel->removeAt(index);
    }
    emit historyListChanged(m_projectHistoryListModel);
    m_projectHistoryListModel->resetModel();
    storeHistory();
}

void GuideDataSource::openProjFile(QVariant proj) {
    QUrl projUrl = proj.toUrl();
    openProjFile(projUrl.toLocalFile());
}

void GuideDataSource::openProjFile(QString projFile) {
    projFile = QDir::toNativeSeparators(projFile);
    if (!QFile::exists(projFile)) {
        qDebug() << "openProjFile fail , projFile " << projFile << " not exists";
        return;
    }
    int findIndex = -1;
    for (int i = 0; i < m_projectHistoryListModel->dataCount(); i++) {
        if (m_projectHistoryListModel->dataWith(i)->localpath() == projFile) {
            findIndex = i;
            break;
        }
    }
    m_lastModel = new ProjectHistoryModel(projFile);
    if (findIndex >= 0) {
        m_projectHistoryListModel->update(findIndex, m_lastModel);
        m_projectHistoryListModel->moveToTop(findIndex);
    } else {
        m_projectHistoryListModel->addData(m_lastModel);
    }

    SEProject *proj = new SEProject(m_lastModel->localpath());
    SEProject::current()->updateProject(proj);
    // move & save
    emit historyListChanged(m_projectHistoryListModel);
    storeHistory();
    showEditPage();
}
void GuideDataSource::newProject() {
    SEProject::createProject();

    // add data & save
    m_lastModel = new ProjectHistoryModel(SEProject::current()->projectPath());
    m_projectHistoryListModel->addData(m_lastModel);
    storeHistory();
    //    QQmlComponent *c = new QQmlComponent(QTShared::get_instance()->engine());
    //    c->loadUrl(QStringLiteral("qrc:/ui/main.qml"));
    //    if (c->isError()) {
    //        qWarning() << "component->isError()="<< c->errors();
    //    } else {
    //        QQuickItem *i = qobject_cast<QQuickItem*>(c->create());
    //        QQmlEngine::setObjectOwnership(i, QQmlEngine::CppOwnership);
    //        i->setParent(this);
    //        i->setVisible(true);
    //    }

    //    i->setParentItem(_view.rootObject());)

    //    var component = Qt.createComponent("AuthorizationBrowser.qml");
    //    win = component.createObject(root);
    //    win.show();
    showEditPage();
}
void GuideDataSource::openProject(int index) {
    m_lastModel = m_projectHistoryListModel->dataWith(index);
    ProjectHistoryModel *model = m_projectHistoryListModel->dataWith(index);
    if (!QFile::exists(model->localpath())) {
        qInfo("openProject fail, file not found!");
        showGuide();
        return;
    }

    SEProject *proj = new SEProject(model->localpath());
    SEProject::current()->updateProject(proj);

    // move & save
    m_projectHistoryListModel->moveToTop(index);
    emit historyListChanged(m_projectHistoryListModel);
    storeHistory();
    showEditPage();
}

void GuideDataSource::copyProject(int index) {
    ThreadPools::instance()->exec([=] {
        //创建工程
        QString projectFile = SEProject::createProjectFile();
        ProjectHistoryModel* oldModel = m_projectHistoryListModel->dataWith(index);
        ProjectHistoryModel* model = new ProjectHistoryModel(oldModel->toJsonObject());
        //复制数据
        QString oldDir = QFileInfo(oldModel->localpath()).dir().path();
        QDir newDir = QFileInfo(projectFile).dir();
        QString newDirPath = newDir.path();
        FileUtils::copyDirectoryFiles(oldDir, newDirPath, true);
        model->updateDir(newDirPath);
        model->setName(newDir.dirName());
        //存储
        ThreadPools::instance()->mainThread(
            [=] {
                m_projectHistoryListModel->addData(model);
                storeHistory();
            },
            0);
    });
}

void GuideDataSource::renameProject(int index, QString newName) {
    ProjectHistoryModel *model = m_projectHistoryListModel->dataWith(index);
    model->setName(newName);
    emit historyListChanged(m_projectHistoryListModel);
    m_projectHistoryListModel->resetModel();
    storeHistory();
}

void GuideDataSource::importProjFromAE(){
    if(AfterEffectHelper::instance()->importFromAfterFx()){
        showToast(tr("Waiting for import to complete!"));
    }else{
        showToast(tr("Check AE is Running!"));
    }
}

void GuideDataSource::removeProject(QList<int> choosedIndex) {
    for (int i = m_projectHistoryListModel->dataCount() - 1; i >= 0; i--) {
        if (choosedIndex.contains(i)) {
            m_projectHistoryListModel->dataWith(i)->remove();
            m_projectHistoryListModel->removeAt(i);
        }
    }
    storeHistory();
}

int GuideDataSource::projectHistoryCount() {
    return m_projectHistoryListModel->dataCount();
}

void GuideDataSource::showEditPage() {
    emit hideGuide();
    ThreadPools::instance()->mainThread([=] {
        emit hideGuide();
    }, 0);
    const QUrl url(QStringLiteral("qrc:/ui/EditorWindow.qml"));
    QmlUtilities::sharedEngine()->load(url);
}

void GuideDataSource::storeHistory() {
    QJsonArray ja;
    for (int i = 0; i < m_projectHistoryListModel->dataCount(); i++) {
        ja.push_back(m_projectHistoryListModel->dataWith(i)->toJsonObject());
    }
    SharedPref::instance()->setJsonArray("cache_history_project", ja);
}

void GuideDataSource::setEditorPage(QObject *object) {
    VooExtension::windowChanged(Editor);
    QObject::connect(object, SIGNAL(newProject()), this, SLOT(newProject()));
    QObject::connect(object, SIGNAL(openProjFile(QVariant)), this, SLOT(openProjFile(QVariant)));
    QObject::connect(object, SIGNAL(updateConfig(QVariant, QVariant, QVariant)), this, SLOT(updateConfig(QVariant, QVariant, QVariant)));
    QObject::connect(object, SIGNAL(showGuide()), this, SIGNAL(showGuide()));
    QObject::connect(object, SIGNAL(release(QObject *)), this, SLOT(onEditPageRelease(QObject *)));
    QObject::connect(object, SIGNAL(exitApp()), this, SIGNAL(exitApp()));
}

void GuideDataSource::onObjectCreated(QObject *object, const QUrl &url) {
    const QUrl editorUrl(QStringLiteral("qrc:/ui/EditorWindow.qml"));
    if (url == editorUrl) {
        setEditorPage(object);
	}
}

void GuideDataSource::onEditPageRelease(QObject *object) {
    VooExtension::windowChanged(Guide);
    QObject::disconnect(object, SIGNAL(newProject()), this, SLOT(newProject()));
    QObject::disconnect(object, SIGNAL(openProjFile(QVariant)), this, SLOT(openProjFile(QVariant)));
    QObject::disconnect(object, SIGNAL(updateConfig(QVariant, QVariant, QVariant)), this, SLOT(updateConfig(QVariant, QVariant, QVariant)));
    QObject::disconnect(object, SIGNAL(showGuide()), this, SIGNAL(showGuide()));
    QObject::disconnect(object, SIGNAL(release(QObject *)), this, SLOT(onEditPageRelease(QObject *)));
    object->deleteLater();
}


void GuideDataSource::onImportResultFromAE(int code,QString msg,QString filePath){
    if(code == 0){
        openProjFile(filePath);
    }else{
        showToast(msg);
    }
}
