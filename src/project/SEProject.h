#pragma once

#include "SEProjectProj.h"
#include "SEProjectConfig.h"
#include "SEProjectDom.h"
#include "timeline/image/SkyImageProvider.h"

#define SECurrentProject SEProject::current()

class SEProject {
public:
    SEProject(QString projectFile);
    ~SEProject();

    // static
    static SEProject *current();
    static void updateProject(SEProject *project);
    static QString createProjectFile();
    static void createProject();
    static void setImageLoader(SkyImageProvider *imageLoader);
    static bool isQuit();

    // instance
    QString title() const;
    QString rootPath() const;
    QString projectPath() const;
    SEProjectProj *proj() const;
    SEProjectDom *dom() const;
    SEProjectConfig *config() const;
    void setTitle(QString title);
    void save(bool quit = false);
    void saveAs(QString newPath);
    void autoSave();
    void restore();

private:
    static SEProject *m_currentInstance;
    SEProjectProj *m_proj = nullptr;
    SEProjectDom *m_dom = nullptr;
    SEProjectConfig *m_config = nullptr;
    QString m_title;
    QString m_rootPath;
    QString m_projectPath;
    bool m_quit = false;
};
