import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import FramelessWindow 1.0
import SkyMedia 1.0
import "../widget"

FramelessWindow {
    property var frameScale
    id: root
    flags: Qt.WindowStaysOnTopHint
    width: 400
    height: 220
    minimumWidth: 100
    minimumHeight: 100
    onlyAllowListMovable: true
    allowTouchWidget: [backItem, contentRect, titleRect]
    signal release(QtObject window)
    Component.onCompleted: {
        x = screen.x + screen.width - width
        y = screen.y + screen.height - height
        contentRect.setWindow(root)
    }
    Rectangle {
        anchors.fill: parent
        color: "#000000"
        Item {
            id: backItem
            anchors.fill: parent
        }
        MiniPlayerView {
            id: contentRect
            height: Math.min(parent.width / frameScale, parent.height)
            width: height * frameScale
            anchors.centerIn: parent
        }
        TitleBar {
            id: titleRect
            width: parent.width
            height: 42
            anchors.top: parent.top
            anchors.left: parent.left
            onCloseClicked: {
                close()
            }
        }
    }
    onClosing: {
        release(this)
    }
}
