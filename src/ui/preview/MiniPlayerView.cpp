#include "MiniPlayerView.h"

#include <QPainter>

#include "base/utils/qmlutilities.h"
#include "base/utils/threadpools.h"
#include "project/SEProject.h"

MiniPlayerView::MiniPlayerView(QQuickItem *parent) {
    setFlag(ItemHasContents, true);
    connect(this, &MiniPlayerView::previewChanged, this, &MiniPlayerView::onPreviewChanged);
    auto preview = SEProject::current()->dom()->previewController();
    preview->setMiniPlayerView(this);
    ThreadPools::instance()->mainThread([=] {
        auto timeline = preview->getSkyTimelineView();
        if (timeline) {
            auto image = timeline->snapFrame();
            m_image = image.convertToFormat(QImage::Format_RGB888);
        }
    });
}

MiniPlayerView::~MiniPlayerView() {
    if (!SEProject::isQuit()) {
        auto preview = SEProject::current()->dom()->previewController();
        preview->setMiniPlayerView(nullptr);
    }
    disconnect(this, &MiniPlayerView::previewChanged, this, &MiniPlayerView::onPreviewChanged);
    hide();
}

void MiniPlayerView::onPreviewChanged(const QImage &image) {
    if (isVisible() && !image.isNull()) {
        m_image = image.convertToFormat(QImage::Format_RGB888);
        update();
    }
}

QQuickWindow *m_miniPlayerWindow = nullptr;
void MiniPlayerView::setWindow(QQuickWindow *window) {
    m_miniPlayerWindow = window;
}

void MiniPlayerView::show() {
    if (m_miniPlayerWindow) {
        return;
    }
    const QUrl url(QStringLiteral("qrc:/ui/dialog/MiniPlayerDialog.qml"));
    QmlUtilities::sharedEngine()->load(url);
}

void MiniPlayerView::hide() {
    if (m_miniPlayerWindow) {
        m_miniPlayerWindow->close();
        m_miniPlayerWindow = nullptr;
    }
}

void MiniPlayerView::paint(QPainter *painter) {
    if (!isVisible()) {
        return;
    }
    painter->drawImage(this->boundingRect(), m_image);
}
