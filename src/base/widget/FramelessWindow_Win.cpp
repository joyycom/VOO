#include "FramelessWindow.h"

#ifdef Q_OS_WIN
#    include <dwmapi.h>
#    pragma comment(lib, "Dwmapi.lib")
#    ifndef GET_X_LPARAM
#        define GET_X_LPARAM(lParam) ((int)(short)LOWORD(lParam))
#    endif
#    ifndef GET_Y_LPARAM
#        define GET_Y_LPARAM(lParam) ((int)(short)HIWORD(lParam))
#    endif

static bool isCompositionEnabled() {
    BOOL composition_enabled = FALSE;
    bool success = ::DwmIsCompositionEnabled(&composition_enabled) == S_OK;
    return composition_enabled && success;
}

FramelessWindow::FramelessWindow(QWindow* parent) : QQuickWindow(parent) {
    setFlags(flags() | Qt::Window | Qt::WindowMinimizeButtonHint | Qt::FramelessWindowHint);
    setColor(Qt::transparent);
}

bool FramelessWindow::nativeEvent(const QByteArray& eventType, void* message, long* result) {
    MSG* msg = static_cast<MSG*>(message);
    switch (msg->message) {
        case WM_NCCALCSIZE: {
            *result = 0;
            return true;
        }
        case WM_NCACTIVATE: {
            if (!isCompositionEnabled()) {
                // Prevents window frame reappearing on window activation
                // in "basic" theme, where no aero shadow is present.
                *result = 1;
                return true;
            }
            break;
        }
        case WM_NCHITTEST: {
            int x = GET_X_LPARAM(msg->lParam);
            int y = GET_Y_LPARAM(msg->lParam);
            QPointF origin = mapToGlobal(QPoint(0, 0));
            auto screenGeo = screen()->availableGeometry();
            QPointF pt((x - screenGeo.x()) / devicePixelRatio() - (origin.x() - screenGeo.x()),
                       (y - screenGeo.y()) / devicePixelRatio() - (origin.y() - screenGeo.y()));
            *result = calculateBorder(pt);
            if (*result == HTCLIENT) {
                if (m_movable) {
                    bool hasMouse = childContainsMouse(contentItem(), pt.x(), pt.y());
                    if (!hasMouse) {
                        *result = HTCAPTION;
                    }
                }
            } else if (!m_resizable) {
                *result = HTCAPTION;
            }
            return true;
        }
        default: break;
    }
    return QQuickWindow::nativeEvent(eventType, message, result);
}

LRESULT FramelessWindow::calculateBorder(const QPointF& pt) {
    if (::IsZoomed((HWND)this->winId())) {
        return HTCLIENT;
    }
    int borderSize = 2;
    int cx = this->size().width();
    int cy = this->size().height();

    QRectF rectTopLeft(0, 0, borderSize, borderSize);
    if (rectTopLeft.contains(pt)) {
        return HTTOPLEFT;
    }

    QRectF rectLeft(0, borderSize, borderSize, cy - borderSize * 2);
    if (rectLeft.contains(pt)) {
        return HTLEFT;
    }

    QRectF rectTopRight(cx - borderSize, 0, borderSize, borderSize);
    if (rectTopRight.contains(pt)) {
        return HTTOPRIGHT;
    }

    QRectF rectRight(cx - borderSize, borderSize, borderSize, cy - borderSize * 2);
    if (rectRight.contains(pt)) {
        return HTRIGHT;
    }

    QRectF rectTop(borderSize, 0, cx - borderSize * 2, borderSize);
    if (rectTop.contains(pt)) {
        return HTTOP;
    }

    QRectF rectBottomLeft(0, cy - borderSize, borderSize, borderSize);
    if (rectBottomLeft.contains(pt)) {
        return HTBOTTOMLEFT;
    }

    QRectF rectBottomRight(cx - borderSize, cy - borderSize, borderSize, borderSize);
    if (rectBottomRight.contains(pt)) {
        return HTBOTTOMRIGHT;
    }

    QRectF rectBottom(borderSize, cy - borderSize, cx - borderSize * 2, borderSize);
    if (rectBottom.contains(pt)) {
        return HTBOTTOM;
    }
    return HTCLIENT;
}

#endif
