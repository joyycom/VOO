#include "AfterEffectHelper.h"
#include "EditorPageDataSource.h"

#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QProcess>
#include <QTextCodec>
#include <QTimer>
#include <QUrl>
#include <QVersionNumber>

#include "BuildConfig.h"
#include "base/http/BaseRsp.h"
#include "base/http/SEHttpClient.h"
#include "base/utils/AppConstance.h"
#include "base/utils/SharedPref.h"
#include "base/utils/threadpools.h"
#include "base/utils/qmlutilities.h"
#include "project/SkyPack.h"
#include "ui/export/ExportDialogDataSource.h"
#include "ui/preview/SkyTimelineView.h"

EditorPageDataSource::EditorPageDataSource(QObject *parent) : QObject(parent) {
    m_currentProject = SEProject::current();
    m_localPath = SEProject::current()->rootPath();
    connect(QmlUtilities::sharedEngine(),
            &QQmlApplicationEngine::objectCreated,
            this,
            &EditorPageDataSource::onObjectCreated);
}

EditorPageDataSource::~EditorPageDataSource() {
    if (m_autoSaveTimer != nullptr) {
        m_autoSaveTimer->stop();
        m_autoSaveTimer->destroyed();
        m_autoSaveTimer = nullptr;
    }
    ThreadPools::instance()->exec([=, currentProject = m_currentProject] {
        if (currentProject == SEProject::current()) {
            delete currentProject;
            m_currentProject = nullptr;
        }
    });
    disconnect(QmlUtilities::sharedEngine(),
               &QQmlApplicationEngine::objectCreated,
               this,
               &EditorPageDataSource::onObjectCreated);
}

QString EditorPageDataSource::currentProjectTitle() {
    if (SEProject::current() != nullptr) {
        return SEProject::current()->title();
    } else {
        return "";
    }
}

void EditorPageDataSource::save(bool quit) {
    SEProject::current()->save(quit);
}

void EditorPageDataSource::saveAs(QUrl fileUrl) {
    ThreadPools::instance()->exec([this, fileUrl] {
        SEProject::current()->save(false);
        m_localPath = QFileInfo(fileUrl.toLocalFile(), SEProject::current()->title()).filePath();
        QDir dir(m_localPath);
        if (dir.exists()) {
            dir.removeRecursively();
        }
        FileUtils::copyDirectoryFiles(SEProject::current()->rootPath(), m_localPath, true);
        QDir dir2(SEProject::current()->rootPath());
        if (dir2.exists()) {
            dir2.removeRecursively();
        }
        SEProject::current()->saveAs(m_localPath);
    });
}

void EditorPageDataSource::undo() {
    SEProject::current()->dom()->timelineController()->undo();
}

void EditorPageDataSource::redo() {
    SEProject::current()->dom()->timelineController()->redo();
}

void EditorPageDataSource::pack() {
}

QString EditorPageDataSource::localPath() {
    return m_localPath;
}

void EditorPageDataSource::initAutoSaveTimer() {
    if (m_autoSaveTimer == nullptr) {
        m_autoSaveTimer = new QTimer(this);
        m_autoSaveTimer->setInterval(30000);
        QObject::connect(m_autoSaveTimer, SIGNAL(timeout()), this, SLOT(onTimerFired()));
        m_autoSaveTimer->start();
    }
}

void EditorPageDataSource::onTimerFired() {
    if (ExportDialogDataSource::isPacking()) {
        return;
    }
    SEProject::current()->autoSave();
    QTime time = QTime::currentTime();
    emit autoSaveSuccess(time.toString());
}

void EditorPageDataSource::onObjectCreated(QObject *object, const QUrl &url) {
    const QUrl viewPreviewUrl(QStringLiteral("qrc:/ui/dialog/VideoPreviewDialog.qml"));
    const QUrl miniPlayerUrl(QStringLiteral("qrc:/ui/dialog/MiniPlayerDialog.qml"));
    if (url == viewPreviewUrl) {
        QObject::connect(object, SIGNAL(release(QObject *)), this, SLOT(onVideoPreviewDialogRelease(QObject *)));
        findVideoPreviewWindow(object);
    } else if (url == miniPlayerUrl) {
        QObject::connect(object, SIGNAL(release(QObject *)), this, SLOT(onMiniPlayerDialogRelease(QObject *)));
        emit miniPlayWindowCreated(object);
    }
}

void EditorPageDataSource::findVideoPreviewWindow(QObject *object) {
    for (QObject *child : object->children()) {
        if (child->objectName() == "videoPreviewWindow") {
            emit videoPreviewWindowCreated(child);
            return;
        } else {
            findVideoPreviewWindow(child);
        }
    }
}

void EditorPageDataSource::onVideoPreviewDialogRelease(QObject *object) {
    QObject::disconnect(object, SIGNAL(release(QObject *)), this, SLOT(onVideoPreviewDialogRelease(QObject *)));
    emit videoPreviewWindowDestroyed();
    object->deleteLater();
}

void EditorPageDataSource::onMiniPlayerDialogRelease(QObject *object) {
    QObject::disconnect(object, SIGNAL(release(QObject *)), this, SLOT(onMiniPlayerDialogRelease(QObject *)));
    emit miniPlayWindowDestroyed();
    object->deleteLater();
}

void EditorPageDataSource::onLoadCompleted() {
    ThreadPools::instance()->exec([=] {
        SEProject::current()->dom()->setEditPageSource(this);
        SEProject::current()->restore();
        initAutoSaveTimer();
    });
}

QString EditorPageDataSource::appVersion() {
    return AppConstance::appVerWithBuildNum();
}

void EditorPageDataSource::checkVersionUpdate() {
    SEHttpClient(AppConstance::serverDomain() + "/v1/sky/backend/getNewVersion").get([=](const QString &response) {
        BaseRsp rsp(response);
        if (rsp.isSuccess()) {
            QJsonObject jsonObject = rsp.data().toObject();
            QString _minVersion = jsonObject.take("minVersion").toString();
            QString _newVersion = jsonObject.take("newVersion").toString();
            QJsonArray descriptions = jsonObject.take("descriptions").toArray();
            QString downloadUrl = jsonObject.take("downloadUrl").toString();
            QVersionNumber currentVersion = QVersionNumber::fromString(appVersion());
            QVersionNumber minVersion = QVersionNumber::fromString(_minVersion);
            QVersionNumber newVersion = QVersionNumber::fromString(_newVersion);
            bool update = QVersionNumber::compare(newVersion, currentVersion) > 0;
            bool force = QVersionNumber::compare(minVersion, currentVersion) > 0;
            QString desc;
            for (int i = 0; i < descriptions.size(); i++) {
                desc.append(descriptions.at(i).toString());
                if (i < descriptions.size() - 1) {
                    desc.append("\n");
                }
            }
            emit versionUpdateResult(update, force, _newVersion, desc, downloadUrl);
        }
    });
}

void EditorPageDataSource::showVideoPreviewDialog() {
    const QUrl url(QStringLiteral("qrc:/ui/dialog/VideoPreviewDialog.qml"));
    QmlUtilities::sharedEngine()->load(url);
}

bool EditorPageDataSource::canShowLab() {
    return !AppConstance::isRelease();
}

void EditorPageDataSource::importFromAE() {
    if (AfterEffectHelper::instance()->importFromAfterFx()) {
        emit closeEditorPage();
    } else {
        SEProject::current()->dom()->timelineController()->showToast(tr("Make sure AE is running!"), 3000);
    }
}

void EditorPageDataSource::setTitle(QString title) {
    SEProject::current()->setTitle(title);
}

bool EditorPageDataSource::hasClip() {
    return SEProject::current()->dom()->timelineController()->hasClip();
}

bool EditorPageDataSource::isTempProject() {
    return SEProject::current()->proj()->isTemp();
}

void EditorPageDataSource::openThirdLicense(){
    ThreadPools::instance()->exec([=]{
        QProcess p;
        p.setReadChannel(QProcess::StandardOutput);
        p.setReadChannelMode(QProcess::MergedChannels);
        QString program = "";
        QStringList opt;
        QString licensePath = QCoreApplication::applicationDirPath().append("/LICENSE.TXT");
        #ifdef Q_OS_WIN
            program = "notepad.exe";
            opt<<licensePath;
        #else
            program = "open";
            opt<<"-t"<<licensePath;
        #endif
        p.start(program, opt);
        p.waitForFinished(-1);
    });

}

void EditorPageDataSource::cancelProgressDialog(int type) {
    if (type == TimelineRewindProgressDialog) {
        SEProject::current()->dom()->timelineController()->cancelRewindVideoClip();
    }
}

void EditorPageDataSource::notifyHideProgressDialog() {
    emit hideProgressDialog();
    qDebug() << "notifyHideProgressDialog";
}

void EditorPageDataSource::notifyProgressUpdate(double progress) {
    emit updateProgressDialog(progress);
    qDebug() << "notifyProgressUpdate:" << progress;
}

void EditorPageDataSource::notifyShowProgressDialog() {
    emit showProgressDialog();
    qDebug() << "notifyShowProgressDialog";
}
