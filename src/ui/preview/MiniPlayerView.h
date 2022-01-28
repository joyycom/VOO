#pragma once
#include <QImage>
#include <QQuickPaintedItem>
#include <QQuickWindow>

class MiniPlayerView : public QQuickPaintedItem {
    Q_OBJECT
public:
    MiniPlayerView(QQuickItem *parent = 0);
    ~MiniPlayerView();

    // QQuickPaintedItem interface
public:
    static void show();
    static void hide();
    Q_INVOKABLE void setWindow(QQuickWindow *window);
    virtual void paint(QPainter *painter) override;
Q_SIGNALS:
    void previewChanged(const QImage &image);

public slots:
    void onPreviewChanged(const QImage &image);

private:
    QImage m_image;
};
