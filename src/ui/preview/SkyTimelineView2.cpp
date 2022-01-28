#include "SkyTimelineView2.h"

#include <QPainter>

#include "base/utils/threadpools.h"
#include "project/SEProject.h"

SkyTimelineView2::SkyTimelineView2(QQuickItem *parent) {
    setFlag(ItemHasContents, true);
    connect(this, &QQuickItem::visibleChanged, this, [=]() {
        if (isVisible() && m_image.isNull()) {
            auto preview = SEProject::current()->dom()->previewController();
            preview->setSkyTimelineView2(this);
            auto image = preview->getSkyTimelineView()->snapFrame();
            m_image = image.convertToFormat(QImage::Format_RGB888);
        }
    });
    connect(this, &SkyTimelineView2::previewChanged, this, &SkyTimelineView2::onPreviewChanged);
}

SkyTimelineView2::~SkyTimelineView2() {
    if (!SEProject::isQuit()) {
        auto preview = SEProject::current()->dom()->previewController();
        preview->setSkyTimelineView2(nullptr);
    }
    disconnect(this, &SkyTimelineView2::previewChanged, this, &SkyTimelineView2::onPreviewChanged);
}

void SkyTimelineView2::onPreviewChanged(const QImage &image) {
    if (isVisible() && !image.isNull()) {
        m_image = image.convertToFormat(QImage::Format_RGB888);
        update();
    }
}

void SkyTimelineView2::paint(QPainter *painter) {
    if (!isVisible()) {
        return;
    }
    painter->drawImage(this->boundingRect(), m_image);
}
