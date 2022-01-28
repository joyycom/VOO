#include "Dispatch_Window.h"
#include <QDir>
#include "project/SEProject.h"
#include "ui/preview/MiniPlayerView.h"

void ReloadEffect::exec(const QJsonValue &data) {
    if (data.isString()) {
        QString path = data.toString();
        if (!SEProject::isQuit()) {
            SkyTimeline *timeline = SEProject::current()->dom()->timeline();
            SkyObjectList objectList = timeline->getAllSubObjects();
            for (int i = 0; i < objectList.size(); i++) {
                SkyObject *obj = objectList[i];
                if (obj->getObjectType() == SkyObjectType_Effect || obj->getObjectType() == SkyObjectType_Transition) {
                    SkyEffect *effect = (SkyEffect *)obj;
                    SkyVariant resPath = effect->getParam("effectPath");
                    QFileInfo effectFileInfo(resPath.toString());
                    if (effectFileInfo.operator==(QFileInfo(path)) ||
                        QFileInfo(effectFileInfo.dir().path()).operator==(QFileInfo(path))) {
                        effect->reload();
                    }
                }
            }
            SEProject::current()->dom()->resourcePreviewController()->repreviewResource();
        }
    }
}

void ShowMiniPlayer::exec(const QJsonValue &data) {
    if (!SEProject::isQuit()) {
        MiniPlayerView::show();
    }
}

void HideMiniPlayer::exec(const QJsonValue &data) {
    MiniPlayerView::hide();
}
