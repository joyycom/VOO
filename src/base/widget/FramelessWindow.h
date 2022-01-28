#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include <QDebug>
#include <QQuickItem>
#include <QQuickWindow>
#include <QScreen>
#include "base/utils/threadpools.h"

class FramelessWindow : public QQuickWindow, public QQmlParserStatus {
    Q_OBJECT
    Q_PROPERTY(QRect screen READ screenRect)
    Q_PROPERTY(bool movable READ movable WRITE setMovable)
    Q_PROPERTY(bool resizable READ resizable WRITE setResizable)
    Q_PROPERTY(bool onlyAllowListMovable READ onlyAllowListMovable WRITE setOnlyAllowListMovable)
    Q_PROPERTY(QVariantList allowTouchWidget READ allowTouchWidget WRITE setAllowTouchWidget)
public:
    explicit FramelessWindow(QWindow *parent = nullptr);

    // QQmlParserStatus interface
public:
    virtual void classBegin() override{};
    virtual void componentComplete() override {
#ifdef Q_OS_WIN
        setFlags(flags() | Qt::Window | Qt::WindowMinimizeButtonHint | Qt::FramelessWindowHint);
#endif
        auto baseSize = size();
        setBaseSize(baseSize);
        setVisible(true);
        auto geo = calcCenterGeo(screen()->availableGeometry(), baseSize);
        if (minimumWidth() > geo.width() || minimumHeight() > geo.height()) {
            setMinimumSize(geo.size());
        }
        setGeometry(geo);
        update();
    };

protected:
#ifdef Q_OS_OSX
    virtual void showEvent(QShowEvent *) override;
#endif
    virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

private:
    QRect calcCenterGeo(const QRect &screenGeo, const QSize &normalSize) {
        int w = normalSize.width();
        int h = normalSize.height();
        int x = screenGeo.x() + (screenGeo.width() - w) / 2;
        int y = screenGeo.y() + (screenGeo.height() - h) / 2;
        if (screenGeo.width() < w) {
            x = screenGeo.x();
            w = screenGeo.width();
        }
        if (screenGeo.height() < h) {
            y = screenGeo.y();
            h = screenGeo.height();
        }
        return {x, y, w, h};
    }
#ifdef Q_OS_WIN
    LRESULT calculateBorder(const QPointF &pt);
#endif
    QRect screenRect() const {
        return screen()->availableGeometry();
    }
    bool movable() const {
        return m_movable;
    }
    void setMovable(bool arg) {
        if (m_movable != arg) {
            m_movable = arg;
        }
    }
    bool resizable() const {
        return m_resizable;
    }
    void setResizable(bool arg) {
        if (m_resizable != arg) {
            m_resizable = arg;
        }
    }
    bool onlyAllowListMovable() const {
        return m_onlyAllowListMovable;
    }
    void setOnlyAllowListMovable(bool arg) {
        if (m_onlyAllowListMovable != arg) {
            m_onlyAllowListMovable = arg;
        }
    }
    QVariantList allowTouchWidget() {
        return m_allowTouchWidgetList;
    }

    void setAllowTouchWidget(QVariantList list) {
        m_allowTouchWidgetList = list;
    }

    bool childContainsMouse(QQuickItem *contentItem, qreal x, qreal y) {
        QQuickItem *item = contentItem->childAt(x, y);
        if (item) {
            auto map = item->mapFromItem(contentItem, QPointF(x, y));
            QQuickItem *childItem = item->childAt(map.x(), map.y());
            if (childItem == nullptr) {
                for (int i = 0; i < m_allowTouchWidgetList.length(); i++) {
                    QQuickItem *allowTouchWidget = m_allowTouchWidgetList.at(i).value<QQuickItem *>();
                    if (allowTouchWidget == item) {
                        return false;
                    }
                }
                return m_onlyAllowListMovable;
            }
            bool hasItem = true;
            while (childItem != nullptr) {
                hasItem = true;
                for (int i = 0; i < m_allowTouchWidgetList.length(); i++) {
                    QQuickItem *allowTouchWidget = m_allowTouchWidgetList.at(i).value<QQuickItem *>();
                    if (allowTouchWidget == childItem) {
                        hasItem = false;
                    }
                }
                map = childItem->mapFromItem(item, map);
                item = childItem;
                childItem = childItem->childAt(map.x(), map.y());
            }
            return hasItem;
        } else {
            return m_onlyAllowListMovable;
        }
    }
    bool m_movable = true;
    bool m_resizable = true;
    bool m_onlyAllowListMovable = false;
    QVariantList m_allowTouchWidgetList;
};

#endif
