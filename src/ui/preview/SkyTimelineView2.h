#pragma once
#include <QImage>
#include <QQuickPaintedItem>
class SkyTimelineView2 : public QQuickPaintedItem {
    Q_OBJECT
public:
    SkyTimelineView2(QQuickItem *parent = 0);
    ~SkyTimelineView2();

    // QQuickPaintedItem interface
public:
    virtual void paint(QPainter *painter) override;
Q_SIGNALS:
    void previewChanged(const QImage &image);

public slots:
    void onPreviewChanged(const QImage &image);

private:
    QImage m_image;
};
