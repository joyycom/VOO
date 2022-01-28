#include "FramelessWindow.h"

#ifdef Q_OS_OSX
#    include <Cocoa/Cocoa.h>
#    include <objc/objc.h>
FramelessWindow::FramelessWindow(QWindow* parent) : QQuickWindow(parent) {
    setFlags(flags() | Qt::Window | Qt::WindowMinimizeButtonHint | Qt::FramelessWindowHint);
    setColor(Qt::transparent);
}

void FramelessWindow::showEvent(QShowEvent*) {
    //隐藏标题栏
    setFlags(flags() & ~Qt::FramelessWindowHint);
    if (!m_resizable) {
        setFlags((flags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);
    }
    NSView* view = (NSView*)winId();
    NSWindow* wnd = [view window];
    [wnd setMovableByWindowBackground:YES];
    wnd.opaque = false;
    wnd.backgroundColor = NSColor.clearColor;
    wnd.titlebarAppearsTransparent = YES;
    wnd.titleVisibility = NSWindowTitleHidden;
    wnd.styleMask |= NSFullSizeContentViewWindowMask;

    //    // Close Button
    //    NSButton* closeButton = [wnd standardWindowButton:NSWindowCloseButton];
    //    qDebug() << "wwww,y:" << closeButton.frame.origin.y;
    //    [closeButton setFrameOrigin:NSMakePoint(closeButton.frame.origin.x, closeButton.frame.origin.y - 2)];

    //    // Minimize Button
    //    NSButton* minimizeButton = [wnd standardWindowButton:NSWindowMiniaturizeButton];
    //    [minimizeButton setFrameOrigin:NSMakePoint(minimizeButton.frame.origin.x,
    //    minimizeButton.frame.origin.y - 2)];

    //    // Zoom Button
    //    NSButton* zoomButton = [wnd standardWindowButton:NSWindowZoomButton];
    //    [zoomButton setFrameOrigin:NSMakePoint(zoomButton.frame.origin.x, zoomButton.frame.origin.y - 2)];

    [[wnd standardWindowButton:NSWindowZoomButton] setHidden:YES];
    [[wnd standardWindowButton:NSWindowMiniaturizeButton] setHidden:YES];
    [[wnd standardWindowButton:NSWindowCloseButton] setHidden:YES];
}
bool FramelessWindow::nativeEvent(const QByteArray& eventType, void* message, long* result) {
    NSEvent* msg = static_cast<NSEvent*>(message);
    switch (msg.type) {
        case NSEventTypeMouseMoved: {
            NSView* view = (NSView*)winId();
            NSWindow* wnd = [view window];
            NSPoint curPoint = [view convertPoint:[msg locationInWindow] fromView:nil];
            BOOL movable = m_movable && !childContainsMouse(contentItem(), curPoint.x, curPoint.y);
            [wnd setMovable:movable];
            break;
        }
        default: break;
    }
    return QQuickWindow::nativeEvent(eventType, message, result);
}
#endif
