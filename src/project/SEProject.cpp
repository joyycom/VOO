#include "SEProject.h"
#include <QUrl>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <src/base/utils/AppPath.h>
#include <base/utils/threadpools.h>

// static method
SEProject *SEProject::m_currentInstance = NULL;
SkyImageProvider *s_imageLoader = nullptr;
SEProject *SEProject::current() {
    return m_currentInstance;
}
void SEProject::updateProject(SEProject *project) {
    SEProject *preProject = m_currentInstance;
    qInfo() << "current Project updated! " << preProject << " -> " << project;
    if (preProject != nullptr) {
        delete preProject;
    }
    m_currentInstance = project;
}

QString SEProject::createProjectFile() {
    QString rootDirPath = AppPath::tempProjectPath(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    QDir rootDir(rootDirPath);
    rootDir.mkpath(rootDirPath);
    QString projectFile = SEProjectProj::projFilePath(rootDirPath);
    return projectFile;
}

void SEProject::createProject() {
    QString projectFile = createProjectFile();
    SEProjectProj::createFile(projectFile);
    SEProject *project = new SEProject(projectFile);
    project->proj()->setIsTemp(true);
    updateProject(project);
}

// instance method
SEProject::SEProject(QString projectFile) {
    qDebug() << "init SEProject";
    m_projectPath = projectFile;
    m_proj = new SEProjectProj(projectFile);
    m_config = new SEProjectConfig(m_proj->projectDir(), m_proj->entryFile());
    m_dom = new SEProjectDom();
    m_rootPath = QDir::toNativeSeparators(m_proj->projectDir());
    m_title = m_proj->title();
    m_quit = false;
}

SEProject::~SEProject() {
    if (m_proj != nullptr) {
        delete m_proj;
        m_proj = nullptr;
    }
    if (m_config != nullptr) {
        delete m_config;
        m_config = nullptr;
    }
    if (m_dom != nullptr) {
        delete m_dom;
        m_dom = nullptr;
    }
    if (this == m_currentInstance) {
        m_currentInstance = nullptr;
    }
}

void SEProject::save(bool quit) {
    m_config->save();
    if (quit) {
        m_dom->timelineController()->timelineModel()->removeBlankTrack();
    }
    if (!m_quit) {
        m_dom->timelineController()->saveInstanceState();
        m_proj->save(m_rootPath);
        m_dom->save(m_rootPath);
    }
    if (quit) {
        m_dom->previewController()->getSkyTimelineView()->stopTimer();
        m_dom->timelineController()->destroyTimelineModel();
        if (s_imageLoader) {
            s_imageLoader->clear();
        }
        m_quit = true;
    }
}

void SEProject::saveAs(QString newPath) {
    QString projFile = SEProjectProj::findProjectFile(newPath);
    if (!projFile.isEmpty()) {
        m_proj = new SEProjectProj(projFile);
    }
    m_rootPath = newPath;
}

void SEProject::autoSave() {
    if (m_dom == nullptr || m_quit) {
        return;
    }
    m_dom->timelineController()->saveInstanceState();
    ThreadPools::instance()->exec([=] {
        if (m_dom == nullptr || m_proj == nullptr || m_dom->timeline() == nullptr) {
            return;
        }
        QTime begin = QTime::currentTime();
        m_proj->save(m_rootPath);
        m_dom->save(m_rootPath);
        QTime time = QTime::currentTime();
        qDebug() << "autoSave success!" << time.toString() << " Spend:" << time.msecsTo(begin);
    });
}

void SEProject::restore() {
    m_dom->restore(m_rootPath);
}

void SEProject::setImageLoader(SkyImageProvider *imageLoader) {
    s_imageLoader = imageLoader;
}

bool SEProject::isQuit() {
    if (m_currentInstance != nullptr) {
        return m_currentInstance->m_quit;
    }
    return true;
}

QString SEProject::title() const {
    return m_title;
}

QString SEProject::rootPath() const {
    return m_rootPath;
}

QString SEProject::projectPath() const {
    return m_projectPath;
}

SEProjectProj *SEProject::proj() const {
    return m_proj;
}

SEProjectDom *SEProject::dom() const {
    return m_dom;
}

SEProjectConfig *SEProject::config() const {
    return m_config;
}

void SEProject::setTitle(QString title) {
    m_title = title;
    m_proj->setTitle(title);
    m_proj->save(m_rootPath);
}
