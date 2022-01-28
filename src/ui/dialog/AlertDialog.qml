import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import FramelessWindow 1.0
import "../widget"

FramelessWindow {
    property var content
    property var button1Text
    property var button2Text
    property var button3Text
    signal button1Event
    signal button2Event
    signal button3Event
    id: window
    modality: Qt.ApplicationModal
    width: 360
    height: titleRect.height + contentColumn.height + buttons.height
            + buttons.anchors.topMargin + buttons.anchors.bottomMargin
    resizable: false
    onlyAllowListMovable: true
    allowTouchWidget: [titleRect]
    onClosing: {
        destroy()
    }
    Rectangle {
        color: "#15171C"
        radius: 4
        border.color: "#000000"
        border.width: 2
        anchors.fill: parent
        TitleBar {
            id: titleRect
            width: parent.width - 4
            height: 34
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 2
            anchors.leftMargin: 2
            color: "#000000"
            onCloseClicked: {
                window.close()
            }
        }
        ColumnLayout {
            id: contentColumn
            width: parent.width
            anchors.top: titleRect.bottom
            spacing: 0
            TextEdit {
                id: contentLabel
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                Layout.topMargin: 25
                text: content
                font.pixelSize: 12
                color: "#BFBFBF"
                readOnly: true
                wrapMode: "WrapAtWordBoundaryOrAnywhere"
            }
        }
        Item {
            id: buttons
            width: parent.width
            height: (button1.visible || button2.visible
                     || button3.visible) ? 26 : 0
            anchors.right: parent.right
            anchors.top: contentColumn.bottom
            anchors.rightMargin: 20
            anchors.topMargin: 25
            anchors.bottomMargin: 20
            clip: true
            Button {
                id: button3
                width: 70
                height: 26
                anchors.right: parent.right
                visible: typeof (button3Text) !== "undefined"
                text: button3Text
                onClicked: {
                    button3Event()
                }
            }
            Button {
                id: button2
                width: 70
                height: 26
                anchors.right: button3.visible ? button3.left : parent.right
                anchors.rightMargin: button3.visible ? 10 : 0
                visible: typeof (button2Text) !== "undefined"
                text: button2Text
                onClicked: {
                    button2Event()
                }
            }
            Button {
                id: button1
                width: 70
                height: 26
                anchors.right: (button2.visible
                                || button3.visible) ? button2.left : parent.right
                anchors.rightMargin: (button2.visible
                                      || button3.visible) ? 10 : 0
                visible: typeof (button1Text) !== "undefined"
                text: button1Text
                style: 1
                onClicked: {
                    button1Event()
                }
            }
        }
    }
}
