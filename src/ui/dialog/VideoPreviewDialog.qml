import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import FramelessWindow 1.0
import "../widget"

FramelessWindow {
    id: videoPreviewDialog
    flags: Qt.WindowStaysOnTopHint
    width: (screen.height * 3 / 4 - 34 - 34) * 9 / 16
    height: screen.height * 3 / 4
    minimumWidth: 400
    minimumHeight: 500
    allowTouchWidget: [titleRect]
    signal release(QtObject window)
    Rectangle {
        id: mainRect
        anchors.fill: parent
        color: "#15171C"
        border.width: 2
        border.color: "#000000"
        radius: 4
        TitleBar {
            id: titleRect
            width: parent.width - 4
            height: 34
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 2
            anchors.leftMargin: 2
            color: "#000000"
            maxEnabled: true
            minEnabled: true
            onCloseClicked: {
                videoPreviewDialog.close()
            }
            onMaxClicked: {
                showMaximized()
            }
            onMinClicked: {
                showMinimized()
            }
        }
        Rectangle {
            id: videoPreviewWindow
            objectName: "videoPreviewWindow"
            width: parent.width - 4
            anchors.top: titleRect.bottom
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 34
            anchors.bottomMargin: 2
            color: "#15171C"
            property alias window: videoPreviewDialog
            function close() {
                videoPreviewDialog.close()
            }
        }
    }
    onClosing: {
        release(this)
    }
}
